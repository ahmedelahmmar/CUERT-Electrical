/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

__IO union
{
	uint8_t allBits;
	struct
	{
		uint8_t MotorDirection 	: 1;
		uint8_t MotorState 		: 1;
		uint8_t AdcConversionsCompleted : 1;
		uint8_t Flag3 : 1;
		uint8_t Flag4 : 1;
		uint8_t Flag5 : 1;
		uint8_t Flag6 : 1;
		uint8_t Flag7 : 1;
	} Bits;
} Flags;


__IO union
{
	uint8_t Sector;
	struct
	{
		uint8_t HallW 		: 1;
		uint8_t HallV 		: 1;
		uint8_t HallU 		: 1;
		uint8_t Reserved 	: 5;
	} Instance;

} HallSensors = {0};

__IO uint32_t AdcConversions[2];
__IO uint32_t newPrescaler = 0;
__IO uint32_t MotorRPS = 0;
__IO uint8_t CurrentDutyCycle = 50;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  HAL_ADC_Start_DMA(&hadc1, (uint32_t *)AdcConversions, 2);

  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_PWM_Start(mHU_TIMER_HANDLE, mHU_TIMER_CHANNEL);
  HAL_TIM_PWM_Start(mHV_TIMER_HANDLE, mHV_TIMER_CHANNEL);
  HAL_TIM_PWM_Start(mHW_TIMER_HANDLE, mHW_TIMER_CHANNEL);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


	  newPrescaler = (uint32_t)((float)((-0.5841f * AdcConversions[mSPEED_POT_INDEX] + 2400.0f ) - 1));
	  __HAL_TIM_SET_PRESCALER(&htim1, newPrescaler);

	  CurrentDutyCycle = (uint8_t)(0.02442 * AdcConversions[mDUTY_POT_INDEX]);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void S180_UpdateCommutation(void)
{
	switch (HallSensors.Sector)
	{
		case (SECTOR_1):

			SetDutyCycle(mHU_TIMER_HANDLE, mHU_TIMER_CHANNEL, CurrentDutyCycle);
			SetDutyCycle(mHV_TIMER_HANDLE, mHV_TIMER_CHANNEL, 0);
			SetDutyCycle(mHW_TIMER_HANDLE, mHW_TIMER_CHANNEL, CurrentDutyCycle);

			break;


		case (SECTOR_2):

			SetDutyCycle(mHU_TIMER_HANDLE, mHU_TIMER_CHANNEL, CurrentDutyCycle);
			SetDutyCycle(mHV_TIMER_HANDLE, mHV_TIMER_CHANNEL, 0);
			SetDutyCycle(mHW_TIMER_HANDLE, mHW_TIMER_CHANNEL, 0);

			break;


		case (SECTOR_3):

			SetDutyCycle(mHU_TIMER_HANDLE, mHU_TIMER_CHANNEL, CurrentDutyCycle);
			SetDutyCycle(mHV_TIMER_HANDLE, mHV_TIMER_CHANNEL, CurrentDutyCycle);
			SetDutyCycle(mHW_TIMER_HANDLE, mHW_TIMER_CHANNEL, 0);

			break;


		case (SECTOR_4):

			SetDutyCycle(mHU_TIMER_HANDLE, mHU_TIMER_CHANNEL, 0);
			SetDutyCycle(mHV_TIMER_HANDLE, mHV_TIMER_CHANNEL, CurrentDutyCycle);
			SetDutyCycle(mHW_TIMER_HANDLE, mHW_TIMER_CHANNEL, 0);

			break;


		case (SECTOR_5):

			SetDutyCycle(mHU_TIMER_HANDLE, mHU_TIMER_CHANNEL, 0);
			SetDutyCycle(mHV_TIMER_HANDLE, mHV_TIMER_CHANNEL, CurrentDutyCycle);
			SetDutyCycle(mHW_TIMER_HANDLE, mHW_TIMER_CHANNEL, CurrentDutyCycle);

			break;


		case (SECTOR_6):

			SetDutyCycle(mHU_TIMER_HANDLE, mHU_TIMER_CHANNEL, 0);
			SetDutyCycle(mHV_TIMER_HANDLE, mHV_TIMER_CHANNEL, 0);
			SetDutyCycle(mHW_TIMER_HANDLE, mHW_TIMER_CHANNEL, CurrentDutyCycle);

			break;


		default: break;
	}
}


void SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t pwmChannel, uint8_t DutyCyclePercentage)
{
	if ( (0 <= DutyCyclePercentage) && (DutyCyclePercentage <= 100) )
	{
		uint32_t pwmMaxCounterValue = __HAL_TIM_GET_AUTORELOAD(htim);
		uint32_t pwmNewCounterValue = (uint32_t)( ((float)DutyCyclePercentage / 100.0f) * (float)pwmMaxCounterValue );

		__HAL_TIM_SET_COMPARE(htim, pwmChannel, pwmNewCounterValue);
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
