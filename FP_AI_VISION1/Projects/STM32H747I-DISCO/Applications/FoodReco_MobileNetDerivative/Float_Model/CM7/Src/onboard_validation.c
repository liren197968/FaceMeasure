/**
 ******************************************************************************
 * @file    onboard_validation.c
 * @author  MCD Application Team
 * @brief   Application to perform on-target neural network evaluation 
 * and testing
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
#include "onboard_validation.h"

/** @addtogroup STM32H747I-DISCO_Applications
  * @{
  */

/** @addtogroup FoodReco_MobileNetDerivative
  * @{
  */
  
#if defined(__ICCARM__)
#pragma location = "Test_table"
#elif defined(__CC_ARM)
__attribute__((section(".Test_table"), zero_init))
#elif defined(__GNUC__)
__attribute__((section(".Test_table")))
#else
#error Unknown compiler
#endif
/*! Used to store image buffer read from microSD */
unsigned char image_buff[MAX_RES_WIDTH * MAX_RES_HEIGHT * RGB_888_BPP];

static char tmp_msg[512];

/* Private function prototypes -----------------------------------------------*/
static size_t ArgMax(ai_float *array, size_t nbr_elem);

/**
 * @brief Displays to LCD an explanation on how to perform on-board validation
 *
 */
void DisplayIntroMessage()
{
  static const uint16_t margin = 15; /* margin for text in pixels*/

  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_DrawRect(200, 10, 400, 50);
  BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)"Onboard Validation", CENTER_MODE);

  BSP_LCD_DisplayStringAt(margin, LINE(4), (uint8_t *)"Onboard validation will look for a directory", LEFT_MODE);
  BSP_LCD_DisplayStringAt(margin, LINE(5), (uint8_t *)"named 'dataset' at the root of the SDCard.", LEFT_MODE);
  BSP_LCD_DisplayStringAt(margin, LINE(6), (uint8_t *)"This directory should contain one directory", LEFT_MODE);
  BSP_LCD_DisplayStringAt(margin, LINE(7), (uint8_t *)"per class containing images in PPM (P6)", LEFT_MODE);
  BSP_LCD_DisplayStringAt(margin, LINE(8), (uint8_t *)"format.", LEFT_MODE);

  BSP_LCD_DisplayStringAt(margin, LINE(11), (uint8_t *)"Please insert the SDCard now,", LEFT_MODE);
  BSP_LCD_DisplayStringAt(margin, LINE(12), (uint8_t *)"then press the wake-up button to get started", LEFT_MODE);

  LCD_Refresh();

  /* Wait for button input */
  while (BSP_PB_GetState(BUTTON_WAKEUP) == RESET)
    ;
}

/**
 * @brief Performs validation of the model with images stored in the microSD Card
 * @warning Images must be in PPM (P6) format. More info here https://en.wikipedia.org/wiki/Netpbm_format
 *
 */
void OnBoardValid(void)
{
  DisplayIntroMessage();

  /* Init SDCard */
  uint8_t sd_error = BSP_SD_Init();
  if (sd_error != MSD_OK)
  {
    BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"Error. SD Card not detected", CENTER_MODE);
    LCD_Refresh();
    BSP_LED_On(LED_RED);
    while (1)
      ;
  }

  /* Init STM32Fs library */
  STM32Fs_Init();

  /* Init AI library */
  AI_Init();

  /* Open the 'dataset' directory */
  uint32_t nbr_dir = 0;
  stm32fs_err_t ret = STM32Fs_GetNumberFiles("/dataset", &nbr_dir, STM32FS_COUNT_DIRS);
  if (ret == STM32FS_ERROR_DIR_NOT_FOUND)
  {
    BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"Error. Directory 'dataset' doesn't exist", CENTER_MODE);
    LCD_Refresh();
    BSP_LED_On(LED_RED);
    while (1)
      ;
  }

  if (nbr_dir != AI_NETWORK_OUT_1_SIZE)
  {
    BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"Error. Number of found directories incorrect", CENTER_MODE);
    LCD_Refresh();
    BSP_LED_On(LED_RED);
    while (1)
      ;
  }

  /* Start validation */

  BSP_LED_On(LED_GREEN);

  /* Create the 'missclassified'  file */
  sprintf(tmp_msg, "List of missclassified files:\n");
  if (STM32Fs_WriteTextToFile("missclassified.txt", tmp_msg, STM32FS_CREATE_NEW_FILE) != STM32FS_ERROR_NONE)
  {
    BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"Error. Unable to create the missclassified file", CENTER_MODE);
    LCD_Refresh();
    BSP_LED_On(LED_RED);
    while (1)
      ;
  }

  BSP_LCD_Clear(LCD_COLOR_BLACK);

  BSP_LCD_SetFont(&Font20);
  sprintf(tmp_msg, "Found %d classes", (unsigned int)nbr_dir);
  BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)tmp_msg, CENTER_MODE);

  /* Initialize confusion matrix */
  uint32_t conf_matrix[AI_NET_OUTPUT_SIZE][AI_NET_OUTPUT_SIZE] = {0};

  /* Initialize loss */
  double overall_loss = 0.0;
  double avg_loss = 0.0;
  uint32_t nbr_tested = 1;

  DIR dataset_dir;
  FILINFO fno;

  STM32Fs_OpenDir("/dataset", &dataset_dir);

  /* Go through each directory in "/dataset" directory */
  while (STM32Fs_GetNextDir(&dataset_dir, &fno) == STM32FS_ERROR_NONE)
  {
    /* Find corresponding class index */
    int class_index = FindClassIndexFromString(fno.fname);

    if (class_index == -1)
    { /* Class index was not found */
      sprintf(tmp_msg, "Error, class %s doesn't exists", fno.fname);
      BSP_LCD_DisplayStringAt(0, LINE(3), (uint8_t *)tmp_msg, CENTER_MODE);
      LCD_Refresh();
      BSP_LED_On(LED_RED);
      while (1)
        ;
    }

    DIR class_dir;
    FILINFO img_fno;

    char class_path[64] = "/dataset/";
    strcat(class_path, fno.fname);
    STM32Fs_OpenDir(class_path, &class_dir);

    /* For each image in this directory (i.e class) */
    while (STM32Fs_GetNextFile(&class_dir, &img_fno) == STM32FS_ERROR_NONE)
    {
      BSP_LED_Toggle(LED_BLUE);
      BSP_LCD_Clear(LCD_COLOR_BLACK);

      sprintf(tmp_msg, "Class: %s, id %d", fno.fname, class_index);
      BSP_LCD_DisplayStringAt(0, LINE(1), (uint8_t *)tmp_msg, CENTER_MODE);

      strcpy(tmp_msg, class_path);
      strcat(tmp_msg, "/");
      strcat(tmp_msg, img_fno.fname);

      uint32_t width, height;

      stm32fs_err_t err = STM32Fs_GetImageInfoPPM(tmp_msg, &width, &height);

      if (err != STM32FS_ERROR_NONE)
      {
        continue; /* Skip this image */
      }

      if (width > MAX_RES_WIDTH || height > MAX_RES_HEIGHT || width <= 0 || height <= 0)
      {
        /* Image is to big to fit in image_buffer or is invalid, skip this image */
        continue;
      }

      /* AI Preprocessing ------------------------------ */

      if (width != ai_get_input_width() || height != ai_get_input_height())
      {
        /* Read to the image buffer */
        err = STM32Fs_ReadImagePPM(tmp_msg, image_buff, &width, &height);

        /* Resize the image  */
        ImageResize((uint8_t *)(image_buff), width, height, RGB_888_BPP, 0, 0, 0, 0,
                    (uint8_t *)(rgb24_scaled_image_buff), ai_get_input_width(), ai_get_input_height());
      }
      else
      {
        /* Skip resize and read directly to scaled buffer */
        err = STM32Fs_ReadImagePPM(tmp_msg, rgb24_scaled_image_buff, &width, &height);
      }

      if (err != STM32FS_ERROR_NONE)
      {
        continue; /* Skip this image */
      }
     
      /*Coherency purpose: clean the lcd_frame_write_buff area in L1 D-Cache before DMA2D reading*/
      SCB_CleanDCache_by_Addr((void *)rgb24_scaled_image_buff, 224*224*3);
      
      /* Display current image to LCD */
      LCD_DMA2D2LCDWriteBuffer((uint32_t *)rgb24_scaled_image_buff, 50, 130, 224, 224, DMA2D_INPUT_RGB888, 1);
      
      /*Pixel value normalisation*/
      RGB24_to_Float_Asym((void*)(rgb24_scaled_image_buff), (void*)(nn_input_buff), ai_get_input_width() * ai_get_input_height(), 0);
      
      /* AI Inference */
      ai_run((void *)nn_input_buff, (void *)nn_output_buff);

      /* AI Post Processing */
      size_t predicted_class = ArgMax(NN_OUTPUT_BUFFER, NN_OUPUT_CLASS_NUMBER);
      if (predicted_class != class_index)
      {
        stm32fs_err_t res = STM32Fs_WriteTextToFile("missclassified.txt", tmp_msg, STM32FS_APPEND_TO_FILE);
        sprintf(tmp_msg, " was missclassified as %s\n", NN_OUTPUT_CLASS_LIST[predicted_class]);
        res |= STM32Fs_WriteTextToFile("missclassified.txt", tmp_msg, STM32FS_APPEND_TO_FILE);
        if (res != STM32FS_ERROR_NONE)
        {
          BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"Error. Unable to write to missclassified file", CENTER_MODE);
          LCD_Refresh();
          while (1)
            ;
        }
      }

      float confidence = nn_output_buff[predicted_class];

      /* Compute categorical-crossentropy
       * As the output comes from the softmax layer, the loss is the negative log of confidence */

      nbr_tested++;
      double categorical_crossentropy = 10.0;
      if (confidence > 0)
      {
        categorical_crossentropy = -log((double)confidence);
      }
      overall_loss += categorical_crossentropy;

      if (nbr_tested > 0)
      {
        avg_loss = overall_loss / (double)nbr_tested;
      }

      /* Update confusion matrix */
      conf_matrix[class_index][predicted_class]++;

      /* Display confusion matrix */
      DisplayConfusionMatrix(conf_matrix);

      sprintf(tmp_msg, "%s %.0f%%", NN_OUTPUT_CLASS_LIST[predicted_class], confidence * 100);
      BSP_LCD_DisplayStringAt(40, LINE(21), (uint8_t *)tmp_msg, LEFT_MODE);
      sprintf(tmp_msg, "Average loss (categorical cross-entropy) %.4f ", avg_loss);
      BSP_LCD_DisplayStringAt(40, LINE(22), (uint8_t *)tmp_msg, LEFT_MODE);

      LCD_Refresh();

    } /* End for each file in class directory */

    f_closedir(&class_dir);

  } /* End loop through all class dirs */

  f_closedir(&dataset_dir);

  /* End of validation */
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_FillRect(50, 130, 224, 224);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayStringAt(40, LINE(10), (uint8_t *)"End of validation.", LEFT_MODE);
  BSP_LCD_DisplayStringAt(40, LINE(11), (uint8_t *)"Press wake-up", LEFT_MODE);
  BSP_LCD_DisplayStringAt(40, LINE(12), (uint8_t *)"button to see report", LEFT_MODE);
  LCD_Refresh();

  /* Wait for button input */
  while (BSP_PB_GetState(BUTTON_WAKEUP) == RESET)
    ;

  BSP_LCD_Clear(LCD_COLOR_BLACK);

  ClassificationReport_t report = classification_report(conf_matrix);

  DisplayClassificationReport(&report);

  WriteClassificationReport(&report, "classification_report.txt");

  WriteConfusionMatrix(conf_matrix, "confusion_matrix.csv");

  while (1)
  {
    HAL_Delay(100);
    BSP_LED_Toggle(LED_GREEN);
  };
}

/**
 * @brief Computes the index of the maximum value in the input array
 *
 * @param array pointer to first element of array of ai_float values
 * @param nbr_elem number of elements in the array
 * @return size_t index of the max element in the array
 */
static size_t ArgMax(ai_float *array, size_t nbr_elem)
{
  int argmax = 0;
  float valmax = -1.0;
  for (int i = 0; i < nbr_elem; i++)
  {
    if (array[i] > valmax)
    {
      argmax = i;
      valmax = array[i];
    }
  }
  return argmax;
}

/**
 * @brief Displays the confusion matrix to screen
 *
 * @param conf_matrix confusion matrix as a 2D array of uint32_t
 */
void DisplayConfusionMatrix(uint32_t conf_matrix[AI_NET_OUTPUT_SIZE][AI_NET_OUTPUT_SIZE])
{
  /* Heat-map LUTs */
  static const uint32_t heat_map[10] = {0xff07071d, 0xff461c48, 0xff6e1f57, 0xff951c5b, 0xffbf1654,
                                        0xffdf2f44, 0xfff16445, 0xfff58860, 0xfff6b995, 0xfff8dcc};

  /* find max value of confusion matrix */
  int conf_max = 0;
  for (int row = 0; row < NN_OUPUT_CLASS_NUMBER; row++)
  {
    for (int col = 0; col < NN_OUPUT_CLASS_NUMBER; col++)
    {
      if (conf_matrix[row][col] > conf_max)
      {
        conf_max = conf_matrix[row][col];
      }
    }
  }

  /* Set a smaller font to fit screen */
  BSP_LCD_SetFont(&Font12);

  /* Offset from top-left of LCD */
  const int x_off = 385;
  const int y_off = 60;
  char conf_value[8];
  for (int row = 0; row < NN_OUPUT_CLASS_NUMBER; row++)
  {
    for (int col = 0; col < NN_OUPUT_CLASS_NUMBER; col++)
    {
      uint32_t value = conf_matrix[row][col];
      if (value > 0)
      {
        float value_norm = value / (float)(conf_max + 1);
        int heat_map_idx = (int)floorf(value_norm * 9.0f);
        BSP_LCD_SetBackColor(heat_map[heat_map_idx]);
        uint32_t text_color = heat_map_idx > 5 ? LCD_COLOR_BLACK : LCD_COLOR_WHITE;
        BSP_LCD_SetTextColor(text_color);
        sprintf(conf_value, "%d", (unsigned int)conf_matrix[row][col]);
      }
      else
      {
        sprintf(conf_value, ".");
      }
      BSP_LCD_DisplayStringAt(x_off + col * 20, y_off + row * 20, (uint8_t *)conf_value, LEFT_MODE);
      BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    }
  }
  BSP_LCD_SetFont(&Font20); /* Reset font size */
}

/**
 * @brief Look for the index of a class name in the global NN_OUTPUT_CLASS_LIST array
 *
 * @param className string to look for in NN_OUTPUT_CLASS_LIST
 * @return int index of the class, -1 if not found
 */
int FindClassIndexFromString(char *className)
{
  for (size_t j = 0; j < AI_NET_OUTPUT_SIZE; j++)
  {
    if (strcmp(className, NN_OUTPUT_CLASS_LIST[j]) == 0)
    {
      return j;
    }
  }

  /* Class name was not found, return -1 */
  return -1;
}

/**
 * @brief Displays a classification report to screen
 * inspired from scikit-learn classification_report()
 *
 * @param report ClassificationReport_t structure to be dispayed
 */
void DisplayClassificationReport(const ClassificationReport_t *report)
{
  char line[64];

  BSP_LCD_DisplayStringAt(320, LINE(0), (uint8_t *)"precision recall f1-score support", LEFT_MODE);
  for (uint32_t target = 0; target < AI_NET_OUTPUT_SIZE; target++)
  {
    sprintf(line, "%20s %8.3f %8.3f %8.3f %4d", NN_OUTPUT_CLASS_LIST[target], report->precisions[target],
            report->recalls[target], report->f1_scores[target], (unsigned int)report->supports[target]);

    BSP_LCD_DisplayStringAt(40, LINE(2 + target), (uint8_t *)line, LEFT_MODE);
  }

  sprintf(line, "%20s %26.3f %4d", "accuracy", report->accuracy, (unsigned int)report->total_support);
  BSP_LCD_DisplayStringAt(40, LINE(3 + AI_NET_OUTPUT_SIZE), (uint8_t *)line, LEFT_MODE);
  sprintf(line, "%20s %8.3f %8.3f %8.3f %4d", "macro avg", report->macro_avg_precision, report->macro_avg_recall,
          report->macro_avg_f1_score, (unsigned int)report->total_support);
  BSP_LCD_DisplayStringAt(40, LINE(4 + AI_NET_OUTPUT_SIZE), (uint8_t *)line, LEFT_MODE);
  sprintf(line, "%20s %8.3f %8.3f %8.3f %4d", "weighted avg", report->weighted_avg_precision,
          report->weighted_avg_recall, report->weighted_avg_f1_score, (unsigned int)report->total_support);
  BSP_LCD_DisplayStringAt(40, LINE(5 + AI_NET_OUTPUT_SIZE), (uint8_t *)line, LEFT_MODE);

  LCD_Refresh();
}

/**
 * @brief Writes a confusion matrix to a csv file on filesystem
 *
 * @param conf_matrix Confusion matrix as 2D array of uint32_t to be written
 * @param path path in the filesystem
 */
void WriteConfusionMatrix(uint32_t conf_matrix[AI_NET_OUTPUT_SIZE][AI_NET_OUTPUT_SIZE], const char *path)
{
  FIL File;

  /* Deletes previous file */
  f_open(&File, path, FA_CREATE_ALWAYS | FA_WRITE);
  f_close(&File);

  f_open(&File, path, FA_OPEN_APPEND | FA_WRITE);
  f_printf(&File, ",Confusion Matrix\n\n");
  f_printf(&File, ",Predicted\n");

  for (int row = 0; row < NN_OUPUT_CLASS_NUMBER; row++)
  {
    f_printf(&File, "%s,", NN_OUTPUT_CLASS_LIST[row]);
    for (int col = 0; col < NN_OUPUT_CLASS_NUMBER; col++)
    {
      uint32_t value = conf_matrix[row][col];
      f_printf(&File, "%d,", value);
    }
    if (row == NN_OUPUT_CLASS_NUMBER / 2)
    {
      f_printf(&File, " Ground truth");
    }
    f_printf(&File, "\n");
  }

  f_close(&File);
}

/**
 * @brief Writes the classification report in a text file on filesystem
 *
 * @param report Classification Report structure
 * @param path path in the filesystem
 */
void WriteClassificationReport(const ClassificationReport_t *report, const char *path)
{
  FIL File;

  char line[64];
  int byteswritten;

  /* Deletes previous file */
  f_open(&File, path, FA_CREATE_ALWAYS | FA_WRITE);
  f_close(&File);

  f_open(&File, path, FA_OPEN_APPEND | FA_WRITE);

  f_printf(&File, "                       precision recall f1-score support\n");

  for (uint32_t target = 0; target < AI_NET_OUTPUT_SIZE; target++)
  {
    /* f_printf doesn't support floating point formating so f_write is used */
    sprintf(line, "%20s %8.3f %8.3f %8.3f %4d\n", NN_OUTPUT_CLASS_LIST[target], report->precisions[target],
            report->recalls[target], report->f1_scores[target], (unsigned int)report->supports[target]);
    f_write(&File, line, strlen(line), (void *)&byteswritten);
  }

  f_printf(&File, "\n");
  sprintf(line, "%20s %26.3f %4d\n", "accuracy", report->accuracy, (unsigned int)report->total_support);
  f_write(&File, line, strlen(line), (void *)&byteswritten);
  sprintf(line, "%20s %8.3f %8.3f %8.3f %4d\n", "macro avg", report->macro_avg_precision, report->macro_avg_recall,
          report->macro_avg_f1_score, (unsigned int)report->total_support);
  f_write(&File, line, strlen(line), (void *)&byteswritten);
  sprintf(line, "%20s %8.3f %8.3f %8.3f %4d\n", "weighted avg", report->weighted_avg_precision,
          report->weighted_avg_recall, report->weighted_avg_f1_score, (unsigned int)report->total_support);
  f_write(&File, line, strlen(line), (void *)&byteswritten);

  f_close(&File);
}

/**
 * @brief Creates a classification report from the confusion matrix.
 * The classification report is inspired from
 * https://scikit-learn.org/stable/modules/generated/sklearn.metrics.classification_report.html
 *
 * @param conf_matrix Confusion matrix from which the classification report should be created
 * @return ClassificationReport_t Classification report as a structure
 */
ClassificationReport_t classification_report(uint32_t conf_matrix[AI_NET_OUTPUT_SIZE][AI_NET_OUTPUT_SIZE])
{
  ClassificationReport_t report;

  uint32_t diagonal_sum = 0;
  uint32_t total_support = 0;

  /* For each class, compute precision, recall and f1-score */
  for (uint32_t target = 0; target < AI_NET_OUTPUT_SIZE; target++)
  {
    uint32_t true_positives = 0;
    uint32_t false_positives = 0;
    uint32_t false_negatives = 0;
    uint32_t support = 0;

    /* First pass columnwise to find false negatives */
    for (uint32_t pred = 0; pred < AI_NET_OUTPUT_SIZE; pred++)
    {
      support += conf_matrix[target][pred];

      if (target != pred)
      { /* False negatives */
        false_negatives += conf_matrix[target][pred];
      }
    } /* End pass columnwise */

    /* Second pass row-wise to compute false positives */
    for (uint32_t gtruth = 0; gtruth < AI_NET_OUTPUT_SIZE; gtruth++)
    {
      if (target != gtruth)
      { /* False positive */
        false_positives += conf_matrix[gtruth][target];
      }
    } /* End pass row-wise */

    /* True positives are in the diagonal */
    true_positives = conf_matrix[target][target];

    float precision = 0.0f;
    if ((true_positives + false_positives) != 0)
    {
      precision = true_positives / (float)(true_positives + false_positives);
    }
    float recall = 0.0f;
    if ((true_positives + false_negatives) != 0)
    {
      recall = true_positives / (float)(true_positives + false_negatives);
    }
    float f1_score = 0.0f;
    if ((recall + precision) != 0.0f)
    {
      f1_score = 2 * (recall * precision) / (recall + precision);
    }

    report.precisions[target] = precision;
    report.recalls[target] = recall;
    report.f1_scores[target] = f1_score;
    report.supports[target] = support;

    /* Increment total numbers to compute accuracy */
    diagonal_sum += true_positives;
    total_support += support;

  } /* End for target class */

  report.accuracy = 0.0f;
  if (total_support != 0)
  {
    report.accuracy = diagonal_sum / (float)total_support;
  }

  report.total_support = total_support;

  /* Compute macro average and weighted average */
  report.macro_avg_precision = 0.0f;
  report.macro_avg_recall = 0.0f;
  report.macro_avg_f1_score = 0.0f;

  report.weighted_avg_precision = 0.0f;
  report.weighted_avg_recall = 0.0f;
  report.weighted_avg_f1_score = 0.0f;

  for (uint32_t target = 0; target < AI_NET_OUTPUT_SIZE; target++)
  {
    report.macro_avg_precision += (float)report.precisions[target];
    report.macro_avg_recall += (float)report.recalls[target];
    report.macro_avg_f1_score += (float)report.f1_scores[target];

    report.weighted_avg_precision += (float)report.precisions[target] * report.supports[target];
    report.weighted_avg_recall += (float)report.recalls[target] * report.supports[target];
    report.weighted_avg_f1_score += (float)report.f1_scores[target] * report.supports[target];
  }
  report.macro_avg_precision /= (float)AI_NET_OUTPUT_SIZE;
  report.macro_avg_recall /= (float)AI_NET_OUTPUT_SIZE;
  report.macro_avg_f1_score /= (float)AI_NET_OUTPUT_SIZE;

  report.weighted_avg_precision /= (float)report.total_support;
  report.weighted_avg_recall /= (float)report.total_support;
  report.weighted_avg_f1_score /= (float)report.total_support;

  return report;
}

/**
  * @}
  */

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
