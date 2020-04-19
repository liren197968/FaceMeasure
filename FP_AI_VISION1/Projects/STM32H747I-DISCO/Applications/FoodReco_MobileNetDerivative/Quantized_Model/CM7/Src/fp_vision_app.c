/**
 ******************************************************************************
 * @file    fp_vision_app.c
 * @author  MCD Application Team
 * @brief   FP VISION Application configuration
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "fp_vision_app.h"
#include <stdio.h>
#include <string.h>

/** @addtogroup STM32H747I-DISCO_Applications
 * @{
 */

/** @addtogroup FoodReco_MobileNetDerivative
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Global variables ----------------------------------------------------------*/
uint32_t nn_inference_time;
uint32_t img_preprocess_time;

#if MEMORY_SCHEME == FULL_EXTERNAL
  #if defined ( __ICCARM__ )
    #pragma location="Vision_App_Complete"
    #pragma data_alignment=16
  #elif defined ( __CC_ARM )
    __attribute__((section(".Vision_App_Complete"), zero_init))
    __attribute__ ((aligned (16)))
  #elif defined ( __GNUC__ )
    __attribute__((section(".Vision_App_Complete")))
    __attribute__ ((aligned (16)))
  #else
    #error Unknown compiler
  #endif
  uint8_t ai_fp_global_memory[AI_FP_GLOBAL_BUFFER_SIZE];
#elif MEMORY_SCHEME == SPLIT_INT_EXT
  #if defined ( __ICCARM__ )
    #pragma location="Vision_App_ImgPipeline"
    #pragma data_alignment=16
  #elif defined ( __CC_ARM )
    __attribute__((section(".Vision_App_ImgPipeline"), zero_init))
    __attribute__ ((aligned (16)))
  #elif defined ( __GNUC__ )
    __attribute__((section(".Vision_App_ImgPipeline")))
    __attribute__ ((aligned (16)))
  #else
    #error Unknown compiler
  #endif
  uint8_t ai_fp_global_memory[AI_FP_GLOBAL_BUFFER_SIZE];
  #if defined ( __ICCARM__ )
    #pragma location="Vision_App_Inference"
  #elif defined ( __CC_ARM )
    __attribute__((section(".Vision_App_Inference"), zero_init))
  #elif defined ( __GNUC__ )
    __attribute__((section(".Vision_App_Inference")))
  #else
    #error Unknown compiler
  #endif
 uint8_t ai_fp_activation_memory[AI_ACTIVATION_BUFFER_SIZE];
#elif MEMORY_SCHEME == FULL_INTERNAL_FPS_OPT || MEMORY_SCHEME == FULL_INTERNAL_MEM_OPT
  #if defined ( __ICCARM__ )
    #pragma location="Vision_App_SingleOverlay"
    #pragma data_alignment=16
  #elif defined ( __CC_ARM )
    __attribute__((section(".Vision_App_SingleOverlay"), zero_init))
    __attribute__ ((aligned (16)))
  #elif defined ( __GNUC__ )
    __attribute__((section(".Vision_App_SingleOverlay")))
    __attribute__ ((aligned (16)))
  #else
    #error Unknown compiler
  #endif
  uint8_t ai_fp_global_memory[AI_FP_GLOBAL_BUFFER_SIZE];
#else
 #error Please check definition of MEMORY_SCHEME define
#endif

ai_float nn_output_buff[AI_NET_OUTPUT_SIZE] = {0};

#if MEMORY_SCHEME == FULL_EXTERNAL || MEMORY_SCHEME == SPLIT_INT_EXT
uint8_t* camera_frame_buff = ai_fp_global_memory;
uint8_t* rgb24_image_buff = ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE;
uint8_t* rgb24_scaled_image_buff = ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE;
uint8_t* nn_input_buff =
    ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE + IMG_RGB24_SCALED_BUFFER_SIZE;
#if MEMORY_SCHEME == FULL_EXTERNAL
uint8_t* activation_buff = ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE +
                           IMG_RGB24_SCALED_BUFFER_SIZE + AI_INPUT_BUFFER_SIZE;
#elif MEMORY_SCHEME == SPLIT_INT_EXT
uint8_t* activation_buff = ai_fp_activation_memory;
#endif
#elif MEMORY_SCHEME == FULL_INTERNAL_MEM_OPT
uint8_t* camera_frame_buff = ai_fp_global_memory + (IMG_RGB24_BUFFER_SIZE - CAM_FRAME_BUFFER_SIZE) + RESIZING_OVERLAY_MARGIN;
uint8_t* rgb24_image_buff = ai_fp_global_memory + RESIZING_OVERLAY_MARGIN;
uint8_t* rgb24_scaled_image_buff = ai_fp_global_memory;
uint8_t* nn_input_buff = NULL;/*since input buffer is allocated within activation buffer ==>  only activation buffer size to be considered*/
uint8_t* activation_buff = ai_fp_global_memory;
#elif MEMORY_SCHEME == FULL_INTERNAL_FPS_OPT
uint8_t* camera_frame_buff = ai_fp_global_memory;
uint8_t* rgb24_image_buff = ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE + RESIZING_OVERLAY_MARGIN;
uint8_t* rgb24_scaled_image_buff = ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE;
uint8_t* nn_input_buff = NULL;/*since input buffer is allocated within activation buffer ==> only activation buffer size to be considered*/
uint8_t* activation_buff = ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE;
#else
#error Please check definition of MEMORY_SCHEME define
#endif

uint8_t pixel_conv_lut[256];

const char* g_food_classes[AI_NET_OUTPUT_SIZE] = {
    "Apple Pie",      "Beer",    "Caesar Salad", "Cappuccino", "Cheesecake",          "Chicken Wings",
    "Chocolate Cake", "Coke",    "Cup Cakes",    "Donuts",     "French Fries",        "Hamburger",
    "Hot Dog",        "Lasagna", "Pizza",        "Risotto",    "Spaghetti Bolognese", "Steak"};

/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

/**
  * @brief  Performs adequate preprocessing on the captured frame image
  * @param  src_image_ptr Pointer to the captured frame image
  * @retval None
  */
static void ai_compute_pix_conv_tab(uint8_t *lut)
{
  /*Retrieve the quantization scheme used to quantize the neural netwrok*/
  switch(ai_get_quantization_scheme())
  {
  case AI_FXP_Q:
    /*Pixel value convertion and normalisation*/
    Precompute_8FXP(lut, ai_get_input_quantized_format());
    break;
    
  case AI_UINT_Q:
    /*Pixel value convertion and normalisation*/
    Precompute_8IntU(lut, ai_get_input_scale_offset(), ai_get_input_zero_point());
    break;
    
  case AI_SINT_Q:
    /*Pixel value convertion and normalisation*/
    Precompute_8IntS(lut, ai_get_input_scale_offset(), ai_get_input_zero_point());
    break;
    
  default:
    break;
  } 
}

/**
* @brief  Performs pixel conversion from 8-bits integer to 8-bits quantized format expected by NN input with normalization in the range [0,+1]
* @param  pSrc            Pointer to source buffer
* @param  pDst            Pointer to destination buffer
* @param  pixels          Number of pixels
* @param  rb_swap         Swap (set to 1) or not (set to 0) the r and b components
* @param  lut             Pointer to the LUT to be used for the pixel conversion
* @retval void     None
*/
void ai_pixel_conversion(void *pSrc, void *pDst, uint32_t pixels, uint32_t rb_swap, uint8_t *lut)
{
  struct rgb_Src
  {
    uint8_t r, g, b;
  };
  
  struct rgb_Dst
  {
    uint8_t r, g, b;
  };
  
  uint8_t tmp_r;
  
  struct rgb_Src *pivot = (struct rgb_Src *) pSrc;
  struct rgb_Dst *dest = (struct rgb_Dst *) pDst;
  
  if(rb_swap == 1)
  {
    for (int i = pixels-1; i >= 0; i--)
    {
      tmp_r=pivot[i].r;
      
      dest[i].r = lut[pivot[i].b];
      dest[i].b = lut[tmp_r];
      dest[i].g = lut[pivot[i].g];
    }
  }
  else /*if(rb_swap == 0)*/
  {
    for (int i = pixels-1; i >= 0; i--)
    {
      dest[i].r = lut[pivot[i].r];
      dest[i].b = lut[pivot[i].b];
      dest[i].g = lut[pivot[i].g];
    }
  }
}

/**
  * @brief  Initializes the generated C model for a neural network
  * @param  None
  * @retval None
  */
void AI_Init(void)
{
#ifdef AI_NETWORK_INPUTS_IN_ACTIVATIONS
  void *input_data_ptr;
  
  input_data_ptr =ai_init((void*)(activation_buff));
  
  if(input_data_ptr!= NULL)
    nn_input_buff=input_data_ptr;
#else
  if (nn_input_buff == NULL)
  {
    while(1);
  }
  
  ai_init((void*)(activation_buff));
#endif
  
  ai_compute_pix_conv_tab(pixel_conv_lut);
}


/**
 * @brief  Performs adequate preprocessing on the captured frame image
 * @param  src_image_ptr Pointer to the captured frame image
 * @retval None
 */
static void ai_input_transform(void* src_image_ptr)
{
  /*Resizing*/
  ImageResize((uint8_t*)(src_image_ptr), CAM_RES_WIDTH, CAM_RES_HEIGHT, RGB_888_BPP,
              0, 0, 0, 0, (uint8_t*)(rgb24_scaled_image_buff),
              ai_get_input_width(), ai_get_input_height());
  
  /*Pixel value convertion and normalisation*/
  ai_pixel_conversion((void*)(rgb24_scaled_image_buff), (void*)(nn_input_buff), ai_get_input_width() * ai_get_input_height(), 1, pixel_conv_lut);
}

/**
 * @brief  De-initializes the generated C model for a neural network
 * @param  None
 * @retval None
 */
void AI_Deinit(void) { ai_deinit(); }

/**
 * @brief  Run an inference of the generated C model for a neural network
 * @param  None
 * @param  None
 * @retval None
 */
void AI_Run(void)
{
  uint32_t Tinf1;
  uint32_t Tinf2;
  uint32_t Tprep1;
  uint32_t Tprep2;

  /*Coherency purpose: invalidate the IMG_RGB24_BUFFER area in L1 D-Cache before CPU reading*/
  uint32_t mask = ~(uint32_t)(32 - 1);
  void* mem = (void*)rgb24_image_buff;
  void* ptr = (void*)((((uint32_t)mem + 32 - 1) & mask) /*-32*/);

  SCB_InvalidateDCache_by_Addr(ptr, IMG_RGB24_BUFFER_SIZE);

  Tprep1 = HAL_GetTick();

  /*Apply pre-process to the input image*/
  ai_input_transform((void*)rgb24_image_buff);

  Tprep2 = HAL_GetTick();

  img_preprocess_time = ((Tprep2 > Tprep1) ? (Tprep2 - Tprep1) : ((1 << 24) - Tprep1 + Tprep2));

  Tinf1 = HAL_GetTick();

  /*Run neural network inference*/
  ai_run((void*)nn_input_buff, (void*)nn_output_buff);

  Tinf2 = HAL_GetTick();

  nn_inference_time = ((Tinf2 > Tinf1) ? (Tinf2 - Tinf1) : ((1 << 24) - Tinf1 + Tinf2));
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
