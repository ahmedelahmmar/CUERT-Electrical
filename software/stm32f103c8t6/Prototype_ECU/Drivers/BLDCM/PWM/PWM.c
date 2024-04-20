
#include "PWM.h"

#include "../BLDCM_Cfg.h"


static uint32_t PWM_prvpulSPWM_LUT[BLDCM_SPWM_LUT_SIZE] = {0, 3, 5, 8, 10, 13, 15, 18, 20, 23,
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


static const PWM_ConfigTypeDef *PWM_prvpxConfigStruct = NULL;


static void PWM_prvvSetDutyCycle(TIM_HandleTypeDef *htim, uint32_t pwmChannel, uint8_t DutyCyclePercentage);


void PWM_vInit(const PWM_ConfigTypeDef * const pxConfigStruct)
{
	if (pxConfigStruct != NULL)
	{
		PWM_prvpxConfigStruct = pxConfigStruct;

		for (uint8_t ucIndex = 0; ucIndex < 3; ++ucIndex)
		{
			__HAL_TIM_SET_COMPARE(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
								  PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex],
								  0);

			HAL_TIM_PWM_Start(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
						      PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex]);
		}
	}
}


void PWM_vStart(const PWM_PhaseTypeDef xPhase, const uint8_t ucDutyCycle)
{
	if (PWM_prvpxConfigStruct != NULL)
	{
		PWM_prvvSetDutyCycle(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
							 PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase],
							 ucDutyCycle);

		HAL_TIM_PWM_Start_IT(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
							 PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase]);
	}
}


void PWM_vStop(const PWM_PhaseTypeDef xPhase)
{
	if (PWM_prvpxConfigStruct != NULL)
	{
		PWM_prvvSetDutyCycle(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
							 PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase],
							 0);
	}
}


//void PWM_vDeInit(const PWM_ConfigTypeDef * const pxConfigStruct)
//{
//	if (pxConfigStruct != NULL)
//	{
//		for (uint8_t ucIndex = 0; ucIndex < 3; ++ucIndex)
//		{
//			HAL_TIM_PWM_DeInit(pxConfigStruct->ppxPhaseTimerHandles[ucIndex]);
//		}
//	}
//}


void PWM_vSetModeTo(const PWM_ModesTypeDef xMode)
{
	if (PWM_prvpxConfigStruct != NULL)
	{
		switch (xMode)
		{
			case PWM:

				for (uint8_t ucIndex = 0; ucIndex < 3; ++ucIndex)
				{
					if ( BLDCM_STOPPING == BLDCM_xGetTransitionState() )
					{
						HAL_TIM_PWM_Stop_DMA(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
											 PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex]);
					}

					HAL_TIM_PWM_Start_IT(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
										PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex]);

					__HAL_TIM_SET_COMPARE(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
										  PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex],
										  0);
				}

				break;

			case SPWM:

				for (uint8_t ucIndex = 0; ucIndex < 3; ++ucIndex)
				{
					__HAL_TIM_SET_COMPARE(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
										  PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex],
										  0);

					HAL_TIM_PWM_Stop_IT(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
										PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex]);
				}

				break;
		}
	}
}

void PWM_vSetSPWMFrequency(const uint32_t ulFrequency);

void PWM_vStartSPWM(const PWM_PhaseTypeDef xPhase)
{
	if (PWM_prvpxConfigStruct != NULL)
	{
		HAL_TIM_PWM_Start_DMA(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
						      PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase],
							  PWM_prvpulSPWM_LUT,
							  BLDCM_SPWM_LUT_SIZE);
	}
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	if (PWM_prvpxConfigStruct != NULL)
	{
		if ( (&mPHASE_U_TIMER_HANDLE == htim) && (mPHASE_U_TIMER_ACTIVE_CHANNEL == htim->Channel) )
		{
			PWM_vStartSPWM(PWM_Phase_V);
		}
		else if ((&mPHASE_V_TIMER_HANDLE == htim) && (mPHASE_V_TIMER_ACTIVE_CHANNEL == htim->Channel) )
		{
			PWM_vStartSPWM(PWM_Phase_W);
		}
	}
}


static void PWM_prvvSetDutyCycle(TIM_HandleTypeDef *htim, uint32_t pwmChannel, uint8_t DutyCyclePercentage)
{
	if ( (0 <= DutyCyclePercentage) && (DutyCyclePercentage <= 100) )
	{
		uint32_t pwmMaxCounterValue = __HAL_TIM_GET_AUTORELOAD(htim);
		uint32_t pwmNewCounterValue = (uint32_t)( ((float)DutyCyclePercentage / 100.0f) * (float)pwmMaxCounterValue );

		__HAL_TIM_SET_COMPARE(htim, pwmChannel, pwmNewCounterValue);
	}
}
