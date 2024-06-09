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
	uint8_t Sector;
	struct
	{
		uint8_t HallW 		: 1;
		uint8_t HallV 		: 1;
		uint8_t HallU 		: 1;
		uint8_t Reserved 	: 5;
	} Instance;

} HallSensors = {0};

PWM_ConfigTypeDef xPWMConfigStruct = {
		.ppxPhaseTimerHandles[PWM_Phase_U] = &mPHASE_U_TIMER_HANDLE,
		.pulPhaseTimerChannels[PWM_Phase_U] = mPHASE_U_TIMER_CHANNEL,

		.ppxPhaseTimerHandles[PWM_Phase_V] = &mPHASE_V_TIMER_HANDLE,
		.pulPhaseTimerChannels[PWM_Phase_V] = mPHASE_V_TIMER_CHANNEL,

		.ppxPhaseTimerHandles[PWM_Phase_W] = &mPHASE_W_TIMER_HANDLE,
		.pulPhaseTimerChannels[PWM_Phase_W] = mPHASE_W_TIMER_CHANNEL,
};

<<<<<<< HEAD
HALL_ConfigTypeDef xHALLConfigStruct = {
		.ppxSensorPorts[HALL_Sensor_U] = GPIOB,
		.pulSensorPins[HALL_Sensor_U] = GPIO_PIN_7,

		.ppxSensorPorts[HALL_Sensor_V] = GPIOB,
		.pulSensorPins[HALL_Sensor_V] = GPIO_PIN_8,

		.ppxSensorPorts[HALL_Sensor_W] = GPIOB,
		.pulSensorPins[HALL_Sensor_W] = GPIO_PIN_9,
};

||||||| 61ab53e
=======
HALL_ConfigTypeDef xHALLConfigStruct = {
		.ppxSensorPorts[HALL_Sensor_U] = GPIOA,
		.pulSensorPins[HALL_Sensor_U] = GPIO_PIN_5,

		.ppxSensorPorts[HALL_Sensor_V] = GPIOA,
		.pulSensorPins[HALL_Sensor_V] = GPIO_PIN_6,

		.ppxSensorPorts[HALL_Sensor_W] = GPIOA,
		.pulSensorPins[HALL_Sensor_W] = GPIO_PIN_7,
};

>>>>>>> 8d117f3c019daa2017624ee00f258bfdf0c452c4
float fActualRPM;
float fSimRPM;

volatile uint32_t ADC_pulReadingBuffer;
volatile uint8_t ADC_pucReadingCounter = 0;

float fActualRPMPercent;
float fSimRPMPercent;
float fDesiredRPMPercent;

BLDCM_StateTypeDef xMotorCurrentState;
BLDCM_StateTypeDef xMotorTransitionState;

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
  MX_TIM1_Init();
  MX_TIM4_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /*
   * Start the ICU responsible for:
   * 	- Actual speed calculataion using a hall sensor input.
   */
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);


#if ( mBLDCM_OPERTATION == mBLDCM_SIMULATE )
  /*
   * Start the timer responsible for:
   * 	- Hall sensor sector simulation.
   * 	- Hall sensor physical signal simulation
   */
<<<<<<< HEAD
  HAL_TIM_Base_Start_IT(&htim4);
#endif
||||||| 61ab53e
  HAL_TIM_Base_Start_IT(&htim4);

=======
//  HAL_TIM_Base_Start_IT(&htim4);

>>>>>>> 8d117f3c019daa2017624ee00f258bfdf0c452c4

  /*
   * Start the adc conversion resposible for:
   * 	- Reading the motor throttle.
   */
//  HAL_ADC_Start_IT(&hadc1);


#if ( mBLDCM_OPERTATION == mBLDCM_SIMULATE )
  /*
   * Start the pwm responsible for:
   * 	- Indication of current speed % with it duty cycle.
   */
<<<<<<< HEAD
  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
#endif

  (void) HALL_vInit(&xHALLConfigStruct);
||||||| 61ab53e
  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);


=======
//  HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);

>>>>>>> 8d117f3c019daa2017624ee00f258bfdf0c452c4
  (void) BLDCM_vInit();
  (void) PWM_vInit(&xPWMConfigStruct);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  fActualRPM = BLDCM_fGetMotorActualRPM();
	  fActualRPMPercent = (fActualRPM / mBLDCM_MAX_SPEED_RPM) * 100;

	  xMotorCurrentState = BLDCM_xGetState();

	  uint8_t RPMPercent = (uint8_t)BLDCM_fGetMotorDesiredRPMPercent();

	  switch ( xMotorCurrentState )
	  {
		  case BLDCM_IDLE:

			  BLDCM_vCommutate();
//			  (void) PWM_vStop(PWM_Phase_U);
//			  (void) PWM_vStop(PWM_Phase_V);
//			  (void) PWM_vStop(PWM_Phase_W);

			  if ( mBLDCM_IDLE_STATE_THRESHOLD_PERCENTAGE < RPMPercent )
			  {
				  (void) BLDCM_vSetState(BLDCM_STARTING);
			  }

			  break;

		  case BLDCM_STARTING:

			  if ( BLDCM_xStartMotor() == BLDCM_STARTING_SUCCEEDED )
			  {
				  (void) BLDCM_vSetState(BLDCM_RUNNING);
			  }
			  else
			  {
				  (void) BLDCM_vSetState(BLDCM_IDLE);
			  }

			  break;

		  case BLDCM_RUNNING:

			  BLDCM_vCommutate();

			  if ( (uint8_t)BLDCM_fGetMotorActualRPMPercent() <  mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE )
			  {
				  BLDCM_vSetState(BLDCM_STOPPING);
			  }

			  break;

		  case BLDCM_STOPPING:

			  if ( BLDCM_xStopMotor() == BLDCM_STOPPING_SUCCEEDED )
			  {
				  (void) BLDCM_vSetState(BLDCM_IDLE);
			  }
			  else
			  {
				  (void) BLDCM_vSetState(BLDCM_RUNNING);
			  }

			  break;

		  default: break;
	  }
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

void SetDutyCycle(TIM_HandleTypeDef *htim, uint32_t pwmChannel, uint8_t DutyCyclePercentage)
{
	if ( (0 <= DutyCyclePercentage) && (DutyCyclePercentage <= 100) )
	{
		uint32_t pwmMaxCounterValue = __HAL_TIM_GET_AUTORELOAD(htim);
		uint32_t pwmNewCounterValue = (uint32_t)( ((float)DutyCyclePercentage / 100.0f) * (float)pwmMaxCounterValue );

		__HAL_TIM_SET_COMPARE(htim, pwmChannel, pwmNewCounterValue);
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	ADC_pulReadingBuffer += HAL_ADC_GetValue(hadc);
	ADC_pucReadingCounter++;

	if (ADC_pucReadingCounter >= 32)
	{
		ADC_pulReadingBuffer = ADC_pulReadingBuffer >> 5;
		ADC_pucReadingCounter = 0;

		ADC_pulReadingBuffer = 1.852 * ADC_pulReadingBuffer - 1897;

		fSimRPM = (float)((float)(mBLDCM_MAX_SPEED_RPM * ADC_pulReadingBuffer) / 4095);
		fSimRPMPercent = (fSimRPM / mBLDCM_MAX_SPEED_RPM) * 100;

<<<<<<< HEAD
#if ( mBLDCM_OPERTATION == mBLDCM_SIMULATE )
		SetDutyCycle(&htim4, TIM_CHANNEL_1, (uint8_t)BLDCM_fGetMotorDesiredRPMPercent());
||||||| 61ab53e
		SetDutyCycle(&htim4, TIM_CHANNEL_1, (uint8_t)fSimRPMPercent);
=======
//		SetDutyCycle(&htim4, TIM_CHANNEL_1, (uint8_t)BLDCM_fGetMotorActualRPMPercent());
>>>>>>> 8d117f3c019daa2017624ee00f258bfdf0c452c4

//		uint32_t ulTim4Prescaler = -13.89 * ADC_pulReadingBuffer + 60000;
//		__HAL_TIM_SET_PRESCALER(&htim4, ulTim4Prescaler);
#endif
		BLDCM_vUpdateMotorDesiredSpeedParameters(fSimRPM);

<<<<<<< HEAD
||||||| 61ab53e
		uint32_t ulTim4Prescaler = -14.58 * ADC_pulReadingBuffer + 60000;
		__HAL_TIM_SET_PRESCALER(&htim4, ulTim4Prescaler);
=======
//		uint32_t ulTim4Prescaler = -14.58 * ADC_pulReadingBuffer + 60000;
//		__HAL_TIM_SET_PRESCALER(&htim4, ulTim4Prescaler);
>>>>>>> 8d117f3c019daa2017624ee00f258bfdf0c452c4
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
