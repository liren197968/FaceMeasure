/**
 ******************************************************************************
 * @file    main.c
 * @author  MCD Application Team
 * @brief   This is the main program for the application running on Cortex-M7
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

/**
 * @mainpage FP-AI-VISION1 computer vision function pack for STM32H7
 *
 * @image html st_logo.png
 *
 * <b>Introduction</b>
 *
 * This firmware package includes Components Device Drivers, Board Support Package
 * and example application for the following STMicroelectronics elements:
 * - STM32H747I-DISCO boards
 * - Middleware library for neural network implementation generated thanks to X-CUBE-AI tool
 * - Middleware library for image preprocessing
 *
 * <b>Software architecture</b>

 * @image html FP-AI-VISION1-Software_Architecture.svg
 *
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "stdlib.h"

/** @addtogroup STM32H747I-DISCO_Applications
 * @{
 */

/** @addtogroup FoodReco_MobileNetDerivative
 * @{
 */

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void MPU_Config(void);
static void Error_Handler(void);
static void Joystick_Run(void);

static void Bubblesort(float *, int *, int);
static void CAMERA_Init(void);
static void AI_Output_Display(void);
static void LED_Init(void);
static void CameraBuff2LcdBuff_Copy(void);
static void CameraBuff2ImageBuff_Copy(void);
static void MainMenu(void);
static void FrameCapture(void);
static void MemoryDump(void);
static void IntermediateDump(ImgPipeline_StageTypdef);
static void NNInference(void);

/* Private variables ---------------------------------------------------------*/
static volatile uint8_t new_frame_ready = 0;
static JOYState_TypeDef joyState = JOY_NONE;
static uint32_t camContrast = CAMERA_CONTRAST_LEVEL2;     /* Mid-level camera brightness. */
static uint32_t camBrightness = CAMERA_BRIGHTNESS_LEVEL2; /* Mid-level camera contrast. */
static uint32_t lcdBrightness = LCD_BRIGHTNESS_MID;       /* Mid-level LCD brightness. */
static uint32_t Tfps;

static uint32_t tpfc = 0;
static uint32_t tpfcstart = 0;
static uint32_t tpfcstop = 0;
static uint32_t tcapture = 0;
static uint32_t tcapturestart = 0;
static uint32_t tcapturestop = 0;
static uint32_t tairun = 0;
static uint32_t tairunstart = 0;
static uint32_t tairunstop = 0;

static uint32_t vsync_it = 0;

static uint32_t session_id = 0;
static uint32_t frame_count = 0;

static RNG_HandleTypeDef RngHandle; /* Random number generator */


/**
 * @brief  Application entry point
 * @param  None
 * @retval None
 */
int main(void)
{
  /* System Init, System clock, voltage scaling and L1-Cache configuration are done by CPU1 (Cortex-M7)
  in the meantime Domain D2 is put in STOP mode(Cortex-M4 in deep-sleep) */

  /* Configure the MPU attributes */
  MPU_Config();

  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  HAL_Init();

  /* Configure the system clock to 400 MHz */
  SystemClock_Config();

  /*Enable CRC HW IP block*/
  __HAL_RCC_CRC_CLK_ENABLE();

  /* Init Random Number Generator */
  RngHandle.Instance = RNG;
  HAL_RNG_DeInit(&RngHandle);
  HAL_RNG_Init(&RngHandle);

  /* Generate a session ID */
  session_id = 0;
  HAL_RNG_GenerateRandomNumber(&RngHandle, &session_id);

  /* LEDs initialization */
  LED_Init();

  /* Configure the Wakeup push-button in GPIO Mode */
  BSP_PB_Init(BUTTON_WAKEUP, BUTTON_MODE_GPIO);

  /* Activate joystick. */
  BSP_JOY_Init(JOY_MODE_GPIO);

  /* Initialize the LCD */
  LCD_Init();

  /* Display Welcome Screen */
  int is_menu = DisplayWelcomeScreen();
  if (is_menu != 0)
  {
    /* Run main menu (blocking call) */
    MainMenu();
  }

  /* If no menu asked, run main application (blocking call) */
  NNInference();

  /* End of program */
}

/* Private functions ---------------------------------------------------------*/

static void NNInference(void)
{
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  /* Initialize the CAMERA */
  CAMERA_Init();
  
  /* Initilaize the AI library */
  AI_Init();
  
  /* Wait until camera acquisition of first frame is completed */
  while (new_frame_ready == 0)
  {
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(100);
  };
  new_frame_ready = 0;
  
  /* Run main loop */
  while (1)
  {
    /* Check joystick input to adjust camera's brightness / contrast */
    Joystick_Run();
    
    CameraBuff2LcdBuff_Copy(); /* DMA transfer to LCD buffer */
    
    tpfcstart=HAL_GetTick();
    CameraBuff2ImageBuff_Copy(); /* DMA transfer with Pixel Format Conversion */
    tpfcstop=HAL_GetTick();
    
#if MEMORY_SCHEME != FULL_INTERNAL_MEM_OPT
    vsync_it=0;
    tcapturestart=HAL_GetTick();
    
    /*Start camera acquisition of subsequent frame*/
    BSP_CAMERA_Resume();
#endif 
    
    tairunstart=HAL_GetTick();
    /*Preprocess the Image buffer and run one inference of the Neural Network*/
    AI_Run();
    tairunstop=HAL_GetTick();   
    
#if MEMORY_SCHEME == FULL_INTERNAL_MEM_OPT
    vsync_it=0;
    tcapturestart=HAL_GetTick();
    
    /*Coherency purpose: clean the camera_frame_buff area in L1 D-Cache before DMA writing so to avoid to subsenquently pollute the camera buffer*/
    SCB_CleanDCache_by_Addr((void *)camera_frame_buff, CAM_FRAME_BUFFER_SIZE);
  
    /*Start camera acquisition of subsequent frame*/
    BSP_CAMERA_Resume();
#endif     
    
    /* Wait until camera acquisition of subsequent frame is completed in order to avoid bottleneck at FMC slave (btw
    * LTDC/DMA2D and DMA)*/
    while (new_frame_ready == 0)
      ;
    new_frame_ready = 0;
    
    tpfc = tpfcstop - tpfcstart;
    tairun = tairunstop - tairunstart;
    tcapture = tcapturestop - tcapturestart;
    
#if MEMORY_SCHEME == FULL_INTERNAL_MEM_OPT  
    Tfps = tpfc + tairun + tcapture;
#elif MEMORY_SCHEME != FULL_INTERNAL_MEM_OPT 
    if((tpfc + tairun) > tcapture)
      Tfps = tpfc + tairun;
    else
      Tfps = tcapture;
#endif
    
    /*Display Neural Network output classification results as well as other performances informations*/
    AI_Output_Display();
    
    BSP_LED_Toggle(LED_BLUE);
    
  } /* End while(1) */
}

static void MemoryDump(void)
{
  int dump_enabled = 0;
  
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  /* Initialize the CAMERA */
  CAMERA_Init();

  /* Initilaize the AI library */
  AI_Init();

  /* Run main loop */
  while (1)
  {
    while (!new_frame_ready);
    new_frame_ready = 0; /* Reset flag */

    /* Check joystick input to adjust camera's brightness / contrast */
    Joystick_Run();
    
    CameraBuff2LcdBuff_Copy(); /* DMA transfer to LCD buffer */
    
    CameraBuff2ImageBuff_Copy(); /* DMA transfer with Pixel Format Conversion */
    
    /* Check for user input */
    if (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
    {
      dump_enabled = 1;
      
      BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"MEMORY DUMP ON-GOING...", CENTER_MODE);
    }
    else
    {
      BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Press WAKEUP button to start memory dumping", CENTER_MODE);
    }
      
    LCD_Refresh();
    
    if(dump_enabled)
    {
      dump_enabled = 0;
      
      BSP_CAMERA_DeInit();
      
      IntermediateDump(INIT);
      
      frame_count++;
      
      IntermediateDump(CAMERA);
      IntermediateDump(RGB_24);
      
      /*Coherency purpose: invalidate the IMG_RGB24_BUFFER area in L1 D-Cache before CPU reading*/
      SCB_InvalidateDCache_by_Addr((void*)rgb24_image_buff, IMG_RGB24_BUFFER_SIZE);
      
      /*Resizing*/
      ImageResize((uint8_t*)(rgb24_image_buff), CAM_RES_WIDTH, CAM_RES_HEIGHT, RGB_888_BPP,
                  0, 0, 0, 0, (uint8_t*)(rgb24_scaled_image_buff),
                  ai_get_input_width(), ai_get_input_height());
      
      IntermediateDump(RGB_24_SCALED);
      
      /*Pixel value convertion and normalisation*/
      ai_pixel_conversion((void*)(rgb24_scaled_image_buff), (void*)(nn_input_buff), ai_get_input_width() * ai_get_input_height(), 1, pixel_conv_lut);
      
      IntermediateDump(NN_INPUT);
      
      /*Run neural network inference*/
      ai_run((void*)nn_input_buff, (void*)nn_output_buff);
      
      IntermediateDump(NN_OUTPUT);
      
      BSP_SD_DeInit();
      
      CAMERA_Init();
      
      BSP_LCD_Clear(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"MEMORY DUMP COMPLETED", CENTER_MODE);
      LCD_Refresh();
      HAL_Delay(1000);
      BSP_LCD_Clear(LCD_COLOR_BLACK);
    }
    else
    {
      BSP_CAMERA_Resume();
    }
    
    BSP_LED_Toggle(LED_BLUE);
    
  } /* End while(1) */
}

/**
 * @brief Intermediate dump function, dumping the memory content of a given buffer into a file in the SD Card
 *
*/
static void IntermediateDump(ImgPipeline_StageTypdef Stage)
{  
  char path[64];
  
  switch(Stage)
  {
  case INIT:
    if (BSP_SD_Init() != MSD_OK) /* Note: BSP_SD_Init() will shut down the camera */
    {
      BSP_LCD_Clear(LCD_COLOR_BLACK);
      BSP_LCD_DisplayStringAt(0, LINE(10), (uint8_t *)"Error. SDCard not detected", CENTER_MODE);
      LCD_Refresh();
      Error_Handler();
    }
    
    (void)STM32Fs_Init();
    
    /* Display session ID to LCD */
    char session_name[18];
    sprintf(session_name, "Session %X", (unsigned int)session_id);
    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)session_name, LEFT_MODE);
    break;
    
  case CAMERA:
    sprintf(path, "SESS_%X_img_%d_rgb565.dat", (unsigned int)session_id, (unsigned int)frame_count);
    STM32Fs_WriteRaw(path, camera_frame_buff, CAM_RES_WIDTH * CAM_RES_HEIGHT * RGB_565_BPP);
    break;
    
  case RGB_24:
    sprintf(path, "SESS_%X_img_%d_rgb888.bmp", (unsigned int)session_id, (unsigned int)frame_count);
    STM32Fs_WriteImageBMP(path, rgb24_image_buff, CAM_RES_WIDTH, CAM_RES_HEIGHT);
    break;
    
  case RGB_24_SCALED:
    /* Write the resized image */
    sprintf(path, "SESS_%X_img_%d_rgb888_resized.bmp", (unsigned int)session_id, (unsigned int)frame_count);
    STM32Fs_WriteImageBMP(path, rgb24_scaled_image_buff, ai_get_input_width(), ai_get_input_height());
    break;
    
  case NN_INPUT:
    /* Write the input tensor image */
    sprintf(path, "SESS_%X_input_tensor_%d.dat", (unsigned int)session_id, (unsigned int)frame_count);
    STM32Fs_WriteRaw(path, nn_input_buff, AI_NETWORK_IN_1_SIZE_BYTES);
    break;
    
  case NN_OUTPUT:
    /* Write the neural network output */
    sprintf(path, "SESS_%X_NN_output_%d.txt", (unsigned int)session_id, (unsigned int)frame_count);
    STM32Fs_WriteTextToFile(path, "          Neural Network Output\n\n", STM32FS_CREATE_NEW_FILE);
    for (int i = 0; i < NN_OUPUT_CLASS_NUMBER; i++)
    {
      char str[128];
      sprintf(str, "%20s:%8.3f\n", NN_OUTPUT_CLASS_LIST[i], NN_OUTPUT_BUFFER[i]);
      STM32Fs_WriteTextToFile(path, str, STM32FS_APPEND_TO_FILE);
    }
    break;
    
  default:
    break;
  } 
}

/**
 * @brief Run the Frame Capture application, where the user can capture frames from the camera to the microSD card test
 *
 */
static void FrameCapture(void)
{
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  /* Initialize the CAMERA */
  CAMERA_Init();

  char file_name[32];
  char session_name[18];

  int capture_enabled = 0;

  sprintf(session_name, "Session %X", (unsigned int)session_id);
  BSP_LED_On(LED_GREEN);
  BSP_LED_Off(LED_RED);

  while (1)
  {
    while (!new_frame_ready)
    {
      /* Check for user input */
      if (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
      {
        capture_enabled = 1;
      }
    }
    new_frame_ready = 0; /* Reset flag */

    /* Check for joystick input to update brightness/contrast */
    Joystick_Run();

    CameraBuff2LcdBuff_Copy();   /* DMA transfer form camera to LCD */
    CameraBuff2ImageBuff_Copy(); /* DMA transfer with Pixel Format Conversion from camera to image buffer */

    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)session_name, LEFT_MODE);
    BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"READY", RIGHT_MODE);

    LCD_Refresh();

    if (capture_enabled)
    {
      capture_enabled = 0;
      frame_count++;
      BSP_LED_Off(LED_GREEN);
      BSP_LED_On(LED_RED);
      BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"BUSY ", RIGHT_MODE);

      LCD_Refresh();

      uint8_t sd_error = BSP_SD_Init();
      if (sd_error != MSD_OK)
      {
        BSP_LCD_DisplayStringAt(0, LINE(12), (uint8_t *)"Error. SD Card not detected", CENTER_MODE);
        LCD_Refresh();
        Error_Handler();
      }

      /* Init STM32Fs library */
      STM32Fs_Init();
      stm32fs_err_t ret = STM32FS_ERROR_NONE;

      sprintf(file_name, "/SESS_%X_%d.bmp", (unsigned int)session_id, (unsigned int)frame_count);
      BSP_LED_On(LED_RED);

      ret = STM32Fs_WriteImageBMP(file_name, rgb24_image_buff, CAM_RES_WIDTH, CAM_RES_HEIGHT);

      BSP_LED_Off(LED_RED);

      if (ret != STM32FS_ERROR_NONE)
      {
        BSP_LCD_DisplayStringAt(0, LINE(12), (uint8_t *)"Error. Writting image failed", CENTER_MODE);
        LCD_Refresh();
        Error_Handler();
      }

      /* Re-Init camera (SDCard shuts down the camera) */
      CAMERA_Init();
      BSP_LED_On(LED_GREEN);
      BSP_LED_Off(LED_RED);
    }
    else
    {
      BSP_CAMERA_Resume();
    }

  } /* End while(1) */
}

/**
 * @brief Runs the main menu, providing a way to choose beetween 4 applications: NN Inference, NN Inference with
 * intermediate dumps, OnBoard validation, Camera Capture
 *
 */
static void MainMenu()
{
  BSP_LCD_Clear(LCD_COLOR_BLACK);

  BSP_LCD_DrawRect(200, 10, 400, 50);
  BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Main Menu", CENTER_MODE);

  BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"Press one of the key of the joystick", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, LINE(6), (uint8_t *)"to start an application", CENTER_MODE);

  /* Draw the "joystick arrows" */
  struct point_t
  {
    uint16_t x;
    uint16_t y;
  };
  const struct point_t pt_center = {.x = 400, .y = 350}; /* x,y coordinates */
  const uint16_t pt_offset = 50;                         /* pixels offset from the center (wideness of the square) */

  BSP_LCD_DrawLine(pt_center.x - pt_offset, pt_center.y, pt_center.x, pt_center.y - pt_offset);
  BSP_LCD_DrawLine(pt_center.x, pt_center.y - pt_offset, pt_center.x + pt_offset, pt_center.y);
  BSP_LCD_DrawLine(pt_center.x + pt_offset, pt_center.y, pt_center.x, pt_center.y + pt_offset);
  BSP_LCD_DrawLine(pt_center.x, pt_center.y + pt_offset, pt_center.x - pt_offset, pt_center.y);

  const char *appli_names[] = {"NN Inference From Camera", "Capture Frames", "OnBoard Validation",
                                   "Intermediate Dumps"};

  BSP_LCD_DisplayStringAt(0, pt_center.y - pt_offset - LINE(2), (uint8_t*)appli_names[0], CENTER_MODE);
  BSP_LCD_DisplayStringAt(pt_center.x + pt_offset + 10, pt_center.y - LINE(1) / 2, (uint8_t*)appli_names[1], LEFT_MODE);
  BSP_LCD_DisplayStringAt(0, pt_center.y + pt_offset + LINE(1), (uint8_t*)appli_names[2], CENTER_MODE);
  BSP_LCD_DisplayStringAt(20, pt_center.y - LINE(1) / 2, (uint8_t*)appli_names[3], LEFT_MODE);

  LCD_Refresh();

  JOYState_TypeDef joy_state = JOY_NONE;
  while (joy_state == JOY_NONE)
  {
    joy_state = BSP_JOY_GetState();
  }

  switch (joy_state)
  {
    case JOY_RIGHT:
      FrameCapture();
      break;
    case JOY_LEFT:
      MemoryDump();
      break;
    case JOY_UP:
      NNInference();
      break;
    case JOY_DOWN:
      OnBoardValid();
      break;
    default:
      NNInference();
      break;
  };

  while (1)
    ;
}

/**
 * @brief  Process the output from NN and display it on LCD and LED
 * @param  None
 * @retval None
 */
static void AI_Output_Display(void)
{
  static uint32_t occurrence_number = NN_OUTPUT_DISPLAY_REFRESH_RATE;
  static uint32_t display_mode=0;//by default, normal mode i.e. logo is displayed

  occurrence_number--;

  if (occurrence_number == 0)
  {
    char msg[70];

    int ranking[NN_OUPUT_CLASS_NUMBER];
    occurrence_number = NN_OUTPUT_DISPLAY_REFRESH_RATE;

    for (int i = 0; i < NN_OUPUT_CLASS_NUMBER; i++)
    {
      ranking[i] = i;
    }

    Bubblesort(NN_OUTPUT_BUFFER, ranking, NN_OUPUT_CLASS_NUMBER);
    
    /*Check if PB is pressed*/
    if (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
    {
      display_mode = !display_mode;
      
      BSP_LCD_Clear(LCD_COLOR_BLACK);
      
      if (display_mode == 1)
      {
        sprintf(msg, "Entering CAMERA PREVIEW mode");
      }
      else  if (display_mode == 0)
      {
        sprintf(msg, "Exiting CAMERA PREVIEW mode");
      }
      
      BSP_LCD_DisplayStringAt(0, LINE(9), (uint8_t*)msg, CENTER_MODE);
      
      sprintf(msg, "Please release button");
      BSP_LCD_DisplayStringAt(0, LINE(11), (uint8_t*)msg, CENTER_MODE);
      LCD_Refresh();
      
      /*Wait for PB release*/
      while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET);
      HAL_Delay(200);
      
      BSP_LCD_Clear(LCD_COLOR_BLACK);
    }
    
    if (display_mode == 0)
    {
      BSP_LCD_Clear(LCD_COLOR_BLACK);/*To clear the camera capture*/
      
      DisplayFoodLogo(LCD_RES_WIDTH / 2 - 64, LCD_RES_HEIGHT / 2 -100, ranking[0]);
    }
    else  if (display_mode == 1)
    {      
      sprintf(msg, "CAMERA PREVIEW MODE");
      
      BSP_LCD_DisplayStringAt(0, LINE(DISPLAY_ACQU_MODE_LINE), (uint8_t*)msg, CENTER_MODE);
    }
  
    for (int i = 0; i < NN_TOP_N_DISPLAY; i++)
    {
      sprintf(msg, "%s %.0f%%", NN_OUTPUT_CLASS_LIST[ranking[i]], NN_OUTPUT_BUFFER[i] * 100);
      BSP_LCD_DisplayStringAt(0, LINE(DISPLAY_TOP_N_LAST_LINE - NN_TOP_N_DISPLAY + i), (uint8_t *)msg, CENTER_MODE);
    }

    sprintf(msg, "Inference: %ldms", *(NN_INFERENCE_TIME));
    BSP_LCD_DisplayStringAt(0, LINE(DISPLAY_INFER_TIME_LINE), (uint8_t *)msg, CENTER_MODE);

    sprintf(msg, "Fps: %.1f", 1000.0F / (float)(Tfps));
    BSP_LCD_DisplayStringAt(0, LINE(DISPLAY_FPS_LINE), (uint8_t *)msg, CENTER_MODE);

    LCD_Refresh();

    /*Toggle LED based on result confidence*/
    BSP_LED_Off(LED_GREEN);
    BSP_LED_Off(LED_ORANGE);
    BSP_LED_Off(LED_RED);

    if ((NN_OUTPUT_BUFFER[0] * 100) < NN_BAD_RES)
    {
      BSP_LED_On(LED_RED);
    }
    else if (((NN_OUTPUT_BUFFER[0] * 100) >= NN_BAD_RES) && ((NN_OUTPUT_BUFFER[0] * 100) < NN_GOOD_RES))
    {
      BSP_LED_On(LED_ORANGE);
    }
    else
    {
      BSP_LED_On(LED_GREEN);
    }
  }
}

/**
 * @brief  LEDs Initialization
 * @param  None
 * @retval None
 */
static void LED_Init(void)
{
  BSP_LED_Init(LED_GREEN);
  BSP_LED_Init(LED_ORANGE);
  BSP_LED_Init(LED_RED);
  BSP_LED_Init(LED_BLUE);
}

/**
 * @brief  CAMERA Initialization
 * @param  None
 * @retval None
 */
static void CAMERA_Init(void)
{
  new_frame_ready = 0; /* clears flag */

  /* Reset and power down camera to be sure camera is Off prior start */
  BSP_CAMERA_PwrDown();

  /* Initialize the Camera */
  if (BSP_CAMERA_Init(CAMERA_RESOLUTION) != CAMERA_OK) Error_Handler();

  /* Start the camera capture */
  BSP_CAMERA_ContinuousStart((uint8_t *)camera_frame_buff);

  /* Wait for the camera initialization after HW reset */
  HAL_Delay(20);
}

/**
 * @brief  Transfer from camera frame buffer to LCD frame buffer for display
 * @param  None
 * @retval None
 */
static void CameraBuff2LcdBuff_Copy(void)
{
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  
  /*DMA2D transfer from Camera frame buffer to LCD write buffer*/
  LCD_DMA2D2LCDWriteBuffer((uint32_t *)(camera_frame_buff), (LCD_RES_WIDTH - CAM_RES_WIDTH) >> 1,
                           (LCD_RES_HEIGHT - CAM_RES_HEIGHT) >> 1, CAM_RES_WIDTH, CAM_RES_HEIGHT, DMA2D_INPUT_RGB565,
                           0);
}

/**
 * @brief  Transfer (with Pixel Format Conversion) from camera frame buffer to RGB24 Image buffer for subsequent
 * preprocessing
 * @param  None
 * @retval None
 */
static void CameraBuff2ImageBuff_Copy(void)
{
  /*DMA2D transfer from Camera frame buffer to RGB888 Image buffer*/
  DMA2D_MEMCOPY((uint32_t *)(camera_frame_buff), (uint32_t *)(rgb24_image_buff), 0, 0, CAM_RES_WIDTH, CAM_RES_HEIGHT,
                CAM_RES_WIDTH, DMA2D_INPUT_RGB565, DMA2D_OUTPUT_RGB888, 1, 0);
}

/**
 * @brief  Bubble sorting algorithm
 * @param  None
 * @retval None
 */
static void Bubblesort(float *prob, int *classes, int size)
{
  float p;
  int c;

  for (int i = 0; i < size; i++)
  {
    for (int ii = 0; ii < size - i - 1; ii++)
    {
      if (prob[ii] < prob[ii + 1])
      {
        p = prob[ii];
        prob[ii] = prob[ii + 1];
        prob[ii + 1] = p;
        c = classes[ii];
        classes[ii] = classes[ii + 1];
        classes[ii + 1] = c;
      }
    }
  }
}

/**
 * @brief  Get the joystick's status
 * @param  None
 * @retval None
 */
static void Joystick_Run(void)
{
  /* Get the joystick state. */
  joyState = BSP_JOY_GetState();

  switch (joyState)
  {
    case JOY_UP:
      if (lcdBrightness < LCD_BRIGHTNESS_MAX)
      {
        lcdBrightness += LCD_BRIGHTNESS_STEP;
      }
      else
      {
        joyState = JOY_NONE; /* Cancel LCD brightness change. */
      }
      break;

    case JOY_DOWN:
      if (lcdBrightness > LCD_BRIGHTNESS_MIN)
      {
        lcdBrightness -= LCD_BRIGHTNESS_STEP;
      }
      else
      {
        joyState = JOY_NONE; /* Cancel LCD brightness change. */
      }
      break;

    case JOY_LEFT:
      if (camContrast > CAMERA_CONTRAST_MIN)
      {
        camContrast--;
      }
      else
      {
        joyState = JOY_NONE; /* Cancel camera contrast change. */
      }
      break;

    case JOY_RIGHT:
      if (camContrast < CAMERA_CONTRAST_MAX)
      {
        camContrast++;
      }
      else
      {
        joyState = JOY_NONE; /* Cancel camera contrast change. */
      }
      break;

    default:
      break;
  }

  if ((joyState == JOY_UP) || (joyState == JOY_DOWN) || (joyState == JOY_LEFT) || (joyState == JOY_RIGHT))
  {
    /* Execute the change. */
    BSP_CAMERA_ContrastBrightnessConfig(camContrast, camBrightness);
    BSP_LCD_SetBrightness(lcdBrightness);
  }
  else if (joyState == JOY_SEL)
  {
    /* Restore the default values. */
    BSP_CAMERA_ContrastBrightnessConfig(CAMERA_CONTRAST_LEVEL2, CAMERA_BRIGHTNESS_LEVEL2);
    BSP_LCD_SetBrightness(LCD_BRIGHTNESS_MID);

    camContrast = CAMERA_CONTRAST_LEVEL2;     /* Mid-level camera brightness. */
    camBrightness = CAMERA_BRIGHTNESS_LEVEL2; /* Mid-level camera contrast. */
    lcdBrightness = LCD_BRIGHTNESS_MID;       /* Mid-level LCD brightness. */
  }
}

/**
 * @brief  Camera Frame Event callback
 * @param  None
 * @retval None
 */
void BSP_CAMERA_FrameEventCallback(void)
{
  /*Notifies the backgound task about new frame available for processing*/
  new_frame_ready = 1;

  /*Suspend acquisition of the data stream coming from camera*/
  BSP_CAMERA_Suspend();
}

/**
  * @brief  VSYNC Event callback.
  * @retval None
  */
void BSP_CAMERA_VsyncEventCallback(void)
{ 
  if(vsync_it==0)
  {
    vsync_it ++;
  }
  else if(vsync_it==1)
  {
    tcapturestart=tcapturestop;
    vsync_it ++;
  }
  
  tcapturestop = HAL_GetTick();
}


/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 400000000 (Cortex-M7 CPU Clock)
 *            HCLK(Hz)                       = 200000000 (Cortex-M4 CPU, Bus matrix Clocks)
 *            AHB Prescaler                  = 2
 *            D1 APB3 Prescaler              = 2 (APB3 Clock  100MHz)
 *            D2 APB1 Prescaler              = 2 (APB1 Clock  100MHz)
 *            D2 APB2 Prescaler              = 2 (APB2 Clock  100MHz)
 *            D3 APB4 Prescaler              = 2 (APB4 Clock  100MHz)
 *            HSE Frequency(Hz)              = 25000000
 *            PLL_M                          = 5
 *            PLL_N                          = 160
 *            PLL_P                          = 2
 *            PLL_Q                          = 4
 *            PLL_R                          = 2
 *            VDD(V)                         = 3.3
 *            Flash Latency(WS)              = 4
 * @param  None
 * @retval None
 */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /*!< Supply configuration update enable */
  HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /* The voltage scaling allows optimizing the power consumption when the device is
  clocked below the maximum system frequency, to update the voltage scaling value
  regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
  {
  }

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.CSIState = RCC_CSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 160;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if (ret != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure  bus clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_D1PCLK1 |
                                 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_D3PCLK1);

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
  if (ret != HAL_OK)
  {
    Error_Handler();
  }

  /*
  Note : The activation of the I/O Compensation Cell is recommended with communication  interfaces
  (GPIO, SPI, FMC, QSPI ...)  when  operating at  high frequencies(please refer to product datasheet)
  The I/O Compensation Cell activation  procedure requires :
  - The activation of the CSI clock
  - The activation of the SYSCFG clock
  - Enabling the I/O Compensation Cell : setting bit[0] of register SYSCFG_CCCSR
  */

  /*activate CSI clock mandatory for I/O Compensation Cell*/
  __HAL_RCC_CSI_ENABLE();

  /* Enable SYSCFG clock mandatory for I/O Compensation Cell */
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /* Enables the I/O Compensation Cell */
  HAL_EnableCompensationCell();
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @param  None
 * @retval None
 */
void Error_Handler(void)
{
  BSP_LED_Off(LED_GREEN);
  BSP_LED_Off(LED_ORANGE);
  BSP_LED_Off(LED_RED);
  BSP_LED_Off(LED_BLUE);

  /* Turn LED RED on */
  BSP_LED_On(LED_RED);
  while (1)
  {
  }
}

/**
 * @brief  CPU L1-Cache enable.
 * @param  None
 * @retval None
 */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
 * @brief  Configure the MPU attributes for the device's memories.
 * @param  None
 * @retval None
 */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

#if EXT_SDRAM_CACHE_ENABLED == 0
  /*
  To make the memory region non cacheable and avoid any cache coherency maintenance:
  - either: MPU_ACCESS_NOT_BUFFERABLE + MPU_ACCESS_NOT_CACHEABLE
  - or: MPU_ACCESS_SHAREABLE => the S field is equivalent to non-cacheable memory
  */
  /* External SDRAM memory: LCD Frame buffer => non-cacheable */
  /*TEX=001, C=0, B=0*/
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xD0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
#elif EXT_SDRAM_CACHE_ENABLED == 1
  /* External SDRAM memory: all as WBWA */
  /*TEX=001, C=1, B=1*/
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xD0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1; 
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE; 
#elif EXT_SDRAM_CACHE_ENABLED == 2
  /*External SDRAM memory: all as Write Thru:*/
  /*TEX=000, C=1, B=0*/
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0xD0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_8MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0; 
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
#else
#error Please check definition of EXT_SDRAM_CACHE_ENABLED define
#endif

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /*Internal SRAM memory: cache policies are WBWA (Write Back and Write Allocate) by default */

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
