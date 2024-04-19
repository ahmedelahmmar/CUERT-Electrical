
#include "PWM.h"

static uint32_t SPWM_LUT[BLDCM_SPWM_LUT_SIZE] = {0, 3, 5, 8, 10, 13, 15, 18, 20, 23,
													   25, 28, 30, 32, 35, 37, 39, 42, 44,
													   46, 49, 51, 53, 55, 57, 59, 61, 63,
													   65, 67, 69, 71, 72, 74, 76, 78, 79,
													   81, 82, 83, 85, 86, 87, 89, 90, 91,
													   92, 93, 94, 95, 95, 96, 97, 97, 98,
													   98, 99, 99, 99, 100, 100, 100, 100,
													   100, 100, 100, 99, 99, 99, 98, 98,
													   97, 97, 96, 95, 95, 94, 93, 92, 91,
													   90, 89, 87, 86, 85, 83, 82, 81, 79,
													   78, 76, 74, 72, 71, 69, 67, 65, 63,
													   61, 59, 57, 55, 53, 51, 49, 46, 44,
													   42, 39, 37, 35, 32, 30, 28, 25, 23,
													   20, 18, 15, 13, 10, 8, 5, 3, 0};



static void PWM_vSetDutyCycle(TIM_HandleTypeDef *htim, uint32_t pwmChannel, uint8_t DutyCyclePercentage);



void PWM_vInit(const PWM_ConfigTypeDef * const pxConfigStruct)
{
	if (pxConfigStruct != NULL)
	{
		for (uint8_t ucIndex = 0; ucIndex < 3; ++ucIndex)
		{
			HAL_TIM_PWM_Start(pxConfigStruct->ppxPhaseTimerHandles[ucIndex], pxConfigStruct->pulPhaseTimerChannels[ucIndex]);
		}
	}
}


void PWM_vStart(const PWM_ConfigTypeDef * const pxConfigStruct, const PWM_PhaseTypeDef xPhase, const uint8_t ucDutyCycle)
{
	if (pxConfigStruct != NULL)
	{
		PWM_vSetDutyCycle(pxConfigStruct->ppxPhaseTimerHandles[xPhase], pxConfigStruct->pulPhaseTimerChannels[xPhase], ucDutyCycle);
	}
}


void PWM_vStop(const PWM_ConfigTypeDef * const pxConfigStruct, const PWM_PhaseTypeDef xPhase)
{
	if (pxConfigStruct != NULL)
	{
		PWM_vSetDutyCycle(pxConfigStruct->ppxPhaseTimerHandles[xPhase], pxConfigStruct->pulPhaseTimerChannels[xPhase], 0);
	}
}


void PWM_vDeInit(const PWM_ConfigTypeDef * const pxConfigStruct)
{
	if (pxConfigStruct != NULL)
	{
		for (uint8_t ucIndex = 0; ucIndex < 3; ++ucIndex)
		{
			HAL_TIM_PWM_DeInit(pxConfigStruct->ppxPhaseTimerHandles[ucIndex]);
		}
	}
}


void PWM_vSetSPWMFrequency(const PWM_ConfigTypeDef * const pxConfigStruct, const uint32_t ulFrequency);

void PWM_vStartSPWM(const PWM_ConfigTypeDef * const pxConfigStruct, const PWM_PhaseTypeDef xPhase)
{
	if (pxConfigStruct != NULL)
	{
		HAL_TIM_PWM_Start_DMA(pxConfigStruct->ppxPhaseTimerHandles[xPhase],
							  pxConfigStruct->pulPhaseTimerChannels[xPhase],
							  SPWM_LUT,
							  BLDCM_SPWM_LUT_SIZE);
	}
}


static void PWM_vSetDutyCycle(TIM_HandleTypeDef *htim, uint32_t pwmChannel, uint8_t DutyCyclePercentage)
{
	if ( (0 <= DutyCyclePercentage) && (DutyCyclePercentage <= 100) )
	{
		uint32_t pwmMaxCounterValue = __HAL_TIM_GET_AUTORELOAD(htim);
		uint32_t pwmNewCounterValue = (uint32_t)( ((float)DutyCyclePercentage / 100.0f) * (float)pwmMaxCounterValue );

		__HAL_TIM_SET_COMPARE(htim, pwmChannel, pwmNewCounterValue);
	}
}
