/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void UpdateCommutationLogic(void);
void SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t pwmChannel, uint8_t DutyCyclePercentage);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUILTIN_LED_Pin GPIO_PIN_13
#define BUILTIN_LED_GPIO_Port GPIOC
#define mDUTY_POT_Pin GPIO_PIN_0
#define mDUTY_POT_GPIO_Port GPIOA
#define mHU_Pin GPIO_PIN_6
#define mHU_GPIO_Port GPIOA
#define mHV_Pin GPIO_PIN_7
#define mHV_GPIO_Port GPIOA
#define mHW_Pin GPIO_PIN_0
#define mHW_GPIO_Port GPIOB
#define mSPEED_POT_Pin GPIO_PIN_1
#define mSPEED_POT_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define mSPEED_POT_INDEX	0
#define mDUTY_POT_INDEX		1

#define SECTOR_1				0b110
#define SECTOR_2				0b100
#define SECTOR_3				0b101
#define SECTOR_4				0b001
#define SECTOR_5				0b011
#define SECTOR_6				0b010

#define mHU_TIMER_HANDLE	&htim3
#define mHV_TIMER_HANDLE	&htim3
#define mHW_TIMER_HANDLE	&htim3

#define mHU_TIMER_CHANNEL	TIM_CHANNEL_1
#define mHV_TIMER_CHANNEL	TIM_CHANNEL_2
#define mHW_TIMER_CHANNEL	TIM_CHANNEL_3

#define MOTOR_POLE_PAIRS	5
#define MOTOR_STOPPED		0
#define MOTOR_RUNNING		1
#define DIRECTION_CCW		0
#define DIRECTION_CW		1
#define TIMER2_CLOCK_FREQ_HZ		1500
#define LUT_SIZE			120

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
