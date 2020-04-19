/**
 ******************************************************************************
 * @file    fp_vision_app.h
 * @author  MCD Application Team
 * @brief   Header for fp_vision_app.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FP_VISION_APP_H
#define __FP_VISION_APP_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "ai_interface.h"
#include "img_preprocess.h"
#include "stm32h747i_discovery.h"
#include "stm32h747i_discovery_camera.h"
#include "stm32h747i_discovery_lcd_patch.h"
#include "stm32h747i_discovery_sdram.h"

  /* Exported types ------------------------------------------------------------*/
  /* Exported constants --------------------------------------------------------*/
  /* External variables --------------------------------------------------------*/
  extern const char* g_food_classes[];
  extern uint32_t nn_inference_time;
  extern uint32_t img_preprocess_time;

  extern uint8_t* camera_frame_buff;
  extern uint8_t* rgb24_image_buff;
  extern uint8_t* rgb24_scaled_image_buff;
  extern uint8_t* nn_input_buff;
  extern ai_float nn_output_buff[];
  extern uint8_t* activation_buff;
  extern uint8_t pixel_conv_lut[];


/*******************/
/***Misc defines****/
/*******************/
#define VGA_640_480_RES 1
#define QVGA_320_240_RES 2

  /*The camera resolution, CAMERA_CAPTURE_RES, is configured in the preprocessor project's option:
   * 1: VGA_640_480_RES
   * 2: QVGA_320_240_RES
   */

#define FULL_EXTERNAL 1
#define SPLIT_INT_EXT 2
#define FULL_INTERNAL_FPS_OPT 3
#define FULL_INTERNAL_MEM_OPT 4

/*The memory layout, MEMORY_SCHEME, is configured in the preprocessor project's option:
 * 1: Full external
 * 2: Split
 * 3: Full internal w/ FPS optim
 * 4: Full internal w/ MEM optim
 */
/* Check allowed combination of memory layout scheme and camera resolution*/
#if CAMERA_CAPTURE_RES == VGA_640_480_RES
#if MEMORY_SCHEME == FULL_INTERNAL_FPS_OPT || MEMORY_SCHEME == FULL_INTERNAL_MEM_OPT
#error Memory scheme not allowed in this application
#endif
#elif CAMERA_CAPTURE_RES == QVGA_320_240_RES
 #if MEMORY_SCHEME == FULL_EXTERNAL || MEMORY_SCHEME == SPLIT_INT_EXT
  #error Memory scheme not allowed in this application
 #endif
#endif

#define WELCOME_MSG_0            "Food image classification"
#define WELCOME_MSG_1            "8-bit Quantized model"
#define WELCOME_MSG_2            "Standard version"

#if CAMERA_CAPTURE_RES == VGA_640_480_RES
#define WELCOME_MSG_3 "VGA"
#elif CAMERA_CAPTURE_RES == QVGA_320_240_RES
#define WELCOME_MSG_3 "QVGA"
#endif

#if MEMORY_SCHEME == FULL_EXTERNAL
#define WELCOME_MSG_4 "Full external"
#elif MEMORY_SCHEME == SPLIT_INT_EXT
#define WELCOME_MSG_4 "Split ext/int"
#elif MEMORY_SCHEME == FULL_INTERNAL_MEM_OPT
#define WELCOME_MSG_4 "Full int Memory optim"
#elif MEMORY_SCHEME == FULL_INTERNAL_FPS_OPT
#define WELCOME_MSG_4 "Full int FPS optim"
#else
#error Please check definition of MEMORY_SCHEME define
#endif

#define NN_OUPUT_CLASS_NUMBER AI_NET_OUTPUT_SIZE
#define NN_OUTPUT_CLASS_LIST g_food_classes
#define NN_OUTPUT_BUFFER nn_output_buff
#define NN_TOP_N_DISPLAY 1
#define NN_TOP_N_DISPLAY_STRING_1 "Top-"
#define NN_TOP_N_DISPLAY_STRING_2 xstr(NN_TOP_N_DISPLAY)
#define NN_TOP_N_DISPLAY_MSG NN_TOP_N_DISPLAY_STRING_1 NN_TOP_N_DISPLAY_STRING_2
#define NN_OUTPUT_DISPLAY_REFRESH_RATE 1
#define NN_INFERENCE_TIME (&nn_inference_time)
#define IMG_PREPROCESS_TIME (&img_preprocess_time)

#define NN_GOOD_RES 70
#define NN_BAD_RES 55

#define RGB_565_BPP 2
#define RGB_888_BPP 3

#define MAX(x, y) ((x > y) ? x : y)
#define xstr(s) str(s)
#define str(s) #s

/**************************/
/***LCD related defines****/
/**************************/
#define LCD_BRIGHTNESS_MIN 0
#define LCD_BRIGHTNESS_MAX 100
#define LCD_BRIGHTNESS_MID 50
#define LCD_BRIGHTNESS_STEP 10

#define DISPLAY_INFER_TIME_LINE 16
#define DISPLAY_IMG_PREPROCESS_LINE (DISPLAY_INFER_TIME_LINE + 1)
#define DISPLAY_FPS_LINE (DISPLAY_INFER_TIME_LINE + 2)
#define DISPLAY_ACQU_MODE_LINE 19
#define DISPLAY_TOP_N_LAST_LINE (DISPLAY_INFER_TIME_LINE - 4)

/****************************/
/***CAMERA related defines***/
/****************************/
#if CAMERA_CAPTURE_RES == VGA_640_480_RES
#define CAMERA_RESOLUTION RESOLUTION_R640x480
#define CAM_RES_WIDTH 640
#define CAM_RES_HEIGHT 480
#elif CAMERA_CAPTURE_RES == QVGA_320_240_RES
#define CAMERA_RESOLUTION RESOLUTION_R320x240
#define CAM_RES_WIDTH 320
#define CAM_RES_HEIGHT 240
#else
#error Please check definition of CAMERA_CAPTURE_RES define
#endif

#define CAMERA_CONTRAST_MIN CAMERA_CONTRAST_LEVEL0
#define CAMERA_CONTRAST_MAX CAMERA_CONTRAST_LEVEL4
#define CAMERA_BRIGHTNESS_MIN CAMERA_BRIGHTNESS_LEVEL0
#define CAMERA_BRIGHTNESS_MAX CAMERA_BRIGHTNESS_LEVEL4

/*****************************/
/***Buffers related defines***/
/*****************************/
#if CAMERA_CAPTURE_RES == VGA_640_480_RES
#define CAM_FRAME_BUFFER_SIZE (CAM_RES_WIDTH * CAM_RES_HEIGHT * RGB_565_BPP) * 2
#elif CAMERA_CAPTURE_RES == QVGA_320_240_RES
#define CAM_FRAME_BUFFER_SIZE (CAM_RES_WIDTH * CAM_RES_HEIGHT * RGB_565_BPP)
#else
#error Please check definition of CAMERA_CAPTURE_RES define
#endif

#define IMG_RGB24_BUFFER_SIZE (CAM_RES_WIDTH * CAM_RES_HEIGHT * RGB_888_BPP)
#define IMG_RGB24_SCALED_BUFFER_SIZE AI_NET_INPUT_SIZE
#define AI_INPUT_BUFFER_SIZE AI_NET_INPUT_SIZE_BYTES
#define AI_ACTIVATION_BUFFER_SIZE AI_ACTIVATION_SIZE_BYTES
  
/******************************/
/***Resizing related defines***/
/******************************/
#if RESIZING_ALGO == RESIZING_NEAREST_NEIGHBOR
#define RESIZING_OVERLAY_MARGIN 0 /*No margin required since the nearest neighbor algorithm can fully execute "in place"*/
#elif RESIZING_ALGO == RESIZING_BILINEAR
#define RESIZING_OVERLAY_MARGIN (CAM_RES_WIDTH * RGB_888_BPP * 2) /*Margin required since the bilinear algorithm cannot fully execute "in place"*/
#else
#error Please check definition of RESIZING_ALGO define
#endif

#if MEMORY_SCHEME == FULL_EXTERNAL
#define AI_FP_GLOBAL_BUFFER_SIZE                                                                         \
  (CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE + IMG_RGB24_SCALED_BUFFER_SIZE + AI_INPUT_BUFFER_SIZE + \
   AI_ACTIVATION_BUFFER_SIZE)
#elif MEMORY_SCHEME == SPLIT_INT_EXT
#define AI_FP_GLOBAL_BUFFER_SIZE \
  (CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE + IMG_RGB24_SCALED_BUFFER_SIZE + AI_INPUT_BUFFER_SIZE)
#elif MEMORY_SCHEME == FULL_INTERNAL_MEM_OPT
#define AI_FP_GLOBAL_BUFFER_SIZE (IMG_RGB24_BUFFER_SIZE + RESIZING_OVERLAY_MARGIN) /*AI_FP_GLOBAL_BUFFER_SIZE is equal to the MAX size between the IMG_RGB24_BUFFER_SIZE and the AI_ACTIVATION_BUFFER_SIZE*/
#elif MEMORY_SCHEME == FULL_INTERNAL_FPS_OPT
#define AI_FP_GLOBAL_BUFFER_SIZE (CAM_FRAME_BUFFER_SIZE + IMG_RGB24_BUFFER_SIZE + RESIZING_OVERLAY_MARGIN) /*AI_FP_GLOBAL_BUFFER_SIZE is equal to CAM_FRAME_BUFFER_SIZE + the MAX size between the IMG_RGB24_BUFFER_SIZE and the AI_ACTIVATION_BUFFER_SIZE*/
#else
#error Please check definition of MEMORY_SCHEME define
#endif

  /* Exported functions ------------------------------------------------------- */
  void AI_Init(void);
  void AI_Deinit(void);
  void AI_Run(void);
  void ai_pixel_conversion(void *, void *, uint32_t, uint32_t,uint8_t *);

#ifdef __cplusplus
}
#endif

#endif /*__FP_VISION_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
