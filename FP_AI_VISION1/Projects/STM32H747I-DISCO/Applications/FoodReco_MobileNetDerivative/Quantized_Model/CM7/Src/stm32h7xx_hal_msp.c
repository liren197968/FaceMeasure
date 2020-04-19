/**
 ******************************************************************************
 * @file    stm32h7xx_hal_msp.c
 * @author  MCD Application Team
 * @brief   HAL MSP module for Cortex-M7.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32H747I-DISCO_Applications
 * @{
 */

/** @addtogroup FoodReco_MobileNetDerivative
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
 * @{
 */

/**
 * @brief  Initializes the Global MSP.
 * @param  None
 * @retval None
 */
void HAL_MspInit(void) {}

/**
 * @brief  DeInitializes the Global MSP.
 * @param  None
 * @retval None
 */
void HAL_MspDeInit(void) {}

/**
 * @brief RNG MSP Initialization
 *        This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 * @param hrng: RNG handle pointer
 * @retval None
 */
void HAL_RNG_MspInit(RNG_HandleTypeDef *hrng)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

  /*Select PLL output as RNG clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RNG;
  PeriphClkInitStruct.RngClockSelection = RCC_RNGCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

  /* RNG Peripheral clock enable */
  __HAL_RCC_RNG_CLK_ENABLE();
}

/**
 * @brief RNG MSP De-Initialization
 *        This function freeze the hardware resources used in this example:
 *          - Disable the Peripheral's clock
 * @param hrng: RNG handle pointer
 * @retval None
 */
void HAL_RNG_MspDeInit(RNG_HandleTypeDef *hrng)
{
  /* Enable RNG reset state */
  __HAL_RCC_RNG_FORCE_RESET();

  /* Release RNG from reset state */
  __HAL_RCC_RNG_RELEASE_RESET();
}

/**
 * @brief  Initializes the PPP MSP.
 * @param  None
 * @retval None
 */
/*void HAL_PPP_MspInit(void)
{*/
/*}*/

/**
 * @brief  DeInitializes the PPP MSP.
 * @param  None
 * @retval None
 */
/*void HAL_PPP_MspDeInit(void)
{*/
/*}*/

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
