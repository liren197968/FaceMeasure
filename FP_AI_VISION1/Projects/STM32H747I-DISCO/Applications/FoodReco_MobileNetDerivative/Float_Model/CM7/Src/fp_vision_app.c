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
#else
 #error Please check definition of MEMORY_SCHEME define
#endif

#if WEIGHT_QSPI == 1 && WEIGHT_EXEC_EXTRAM == 1
  #if MEMORY_SCHEME == FULL_EXTERNAL
    #if defined ( __ICCARM__ )
      #pragma location="Vision_App_Complete"
    #elif defined ( __CC_ARM )
      __attribute__((section(".Vision_App_Complete"), zero_init))
    #elif defined ( __GNUC__ )
      __attribute__((section(".Vision_App_Complete")))
    #else
      #error Unknown compiler
    #endif
  #elif MEMORY_SCHEME == SPLIT_INT_EXT
    #if defined ( __ICCARM__ )
      #pragma location="Vision_App_ImgPipeline"
    #elif defined ( __CC_ARM )
      __attribute__((section(".Vision_App_ImgPipeline"), zero_init))
    #elif defined ( __GNUC__ )
      __attribute__((section(".Vision_App_ImgPipeline")))
    #else
      #error Unknown compiler
    #endif
  #endif
  uint8_t weights_table_sdram[AI_WEIGHT_SIZE_BYTES];
#endif

uint8_t* camera_frame_buff=ai_fp_global_memory;
uint8_t* rgb24_image_buff= ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE;
uint8_t* rgb24_scaled_image_buff= ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE;
uint8_t* nn_input_buff = ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE + IMG_RGB24_SCALED_BUFFER_SIZE;
ai_float nn_output_buff[AI_NET_OUTPUT_SIZE]= {0};

#if MEMORY_SCHEME == FULL_EXTERNAL
 uint8_t* activation_buff = ai_fp_global_memory + CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE + IMG_RGB24_SCALED_BUFFER_SIZE + AI_INPUT_BUFFER_SIZE;
#elif MEMORY_SCHEME == SPLIT_INT_EXT
 uint8_t* activation_buff=ai_fp_activation_memory;
#else
 #error Please check definition of MEMORY_SCHEME define
#endif

const char *g_food_classes[] = {
  "Apple Pie", "Beer", "Caesar Salad", "Cappuccino",
  "Cheesecake", "Chicken Wings", "Chocolate Cake", "Coke",
  "Cup Cakes", "Donuts", "French Fries", "Hamburger",
  "Hot Dog", "Lasagna", "Pizza", "Risotto",
  "Spaghetti Bolognese", "Steak"
};

/* Private function prototypes -----------------------------------------------*/
/* Functions Definition ------------------------------------------------------*/

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
  
  /*Pixel value normalisation*/
  RGB24_to_Float_Asym((void*)(rgb24_scaled_image_buff), (void*)(nn_input_buff), ai_get_input_width() * ai_get_input_height(), 1);
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
