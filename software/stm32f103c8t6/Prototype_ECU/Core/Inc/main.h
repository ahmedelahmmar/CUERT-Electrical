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
#include "../../Drivers/BLDCM/BLDCM.h"
#include "../../Drivers/BLDCM/BLDCM_Cfg.h"
#include "../../Drivers/BLDCM/PWM/PWM.h"
#include "../../Drivers/BLDCM/Hall/Hall.h"
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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BUILTIN_LED_Pin GPIO_PIN_13
#define BUILTIN_LED_GPIO_Port GPIOC
#define ADC_CURRENT_SPEED_POT_Pin GPIO_PIN_0
#define ADC_CURRENT_SPEED_POT_GPIO_Port GPIOB
#define PWM_PHASE_U_Pin GPIO_PIN_8
#define PWM_PHASE_U_GPIO_Port GPIOA
#define PWM_PHASE_V_Pin GPIO_PIN_9
#define PWM_PHASE_V_GPIO_Port GPIOA
#define PWM_PHASE_W_Pin GPIO_PIN_10
#define PWM_PHASE_W_GPIO_Port GPIOA
#define ICU_HALL_SENSOR_X_Pin GPIO_PIN_15
#define ICU_HALL_SENSOR_X_GPIO_Port GPIOA
#define PWM_ACTUAL_CURRENT_SPEED_Pin GPIO_PIN_6
#define PWM_ACTUAL_CURRENT_SPEED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

#define mSPEED_POT_INDEX	0
#define mDUTY_POT_INDEX		1

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
