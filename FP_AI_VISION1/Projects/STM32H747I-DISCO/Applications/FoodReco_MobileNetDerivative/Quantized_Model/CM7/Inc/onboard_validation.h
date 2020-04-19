/**
 ******************************************************************************
 * @file    onboard_validation.h
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
#ifndef ONBOARD_VALIDATION_H
#define ONBOARD_VALIDATION_H

#ifdef __cplusplus
extern "C" 
{
#endif

#include "display.h"
#include "fp_vision_app.h"
#include "stm32_fs.h"

#define MAX_RES_WIDTH (1080) /* Max width in pixels */
#define MAX_RES_HEIGHT (720) /* Max height in pixels */

/*! Structure holding classification report data */
typedef struct ClassificationReport
{
  float precisions[AI_NET_OUTPUT_SIZE];  /*! Precision per class */
  float recalls[AI_NET_OUTPUT_SIZE];     /*! Recall per class */
  float f1_scores[AI_NET_OUTPUT_SIZE];   /*! F1-score per class */
  uint32_t supports[AI_NET_OUTPUT_SIZE]; /*! Number of elements per class */

  float accuracy;         /*! Accuracy of the classification */
  uint32_t total_support; /*! Total number of elements classified */

  float macro_avg_precision; /*! Average precision */
  float macro_avg_recall;    /*! Average recall */
  float macro_avg_f1_score;  /*! Average F1-score */

  float weighted_avg_precision; /*! Support-weighted average precision */
  float weighted_avg_recall;    /*! Support-weighted average recall */
  float weighted_avg_f1_score;  /*! Support-weighted average F1-score */

} ClassificationReport_t;

void DisplayIntroMessage(void);
void OnBoardValid(void);
ClassificationReport_t classification_report(uint32_t conf_matrix[AI_NET_OUTPUT_SIZE][AI_NET_OUTPUT_SIZE]);
void DisplayClassificationReport(const ClassificationReport_t *report);
void WriteClassificationReport(const ClassificationReport_t *report, const char *path);
void WriteConfusionMatrix(uint32_t conf_matrix[AI_NET_OUTPUT_SIZE][AI_NET_OUTPUT_SIZE], const char *path);
void DisplayConfusionMatrix(uint32_t conf_matrix[AI_NET_OUTPUT_SIZE][AI_NET_OUTPUT_SIZE]);
int FindClassIndexFromString(char *);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* ONBOARD_VALIDATION_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
