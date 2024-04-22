
#include "PWM.h"
#include "../BLDCM_Cfg.h"

static const PWM_ConfigTypeDef *PWM_prvpxConfigStruct = NULL;

static uint8_t PWM_prvpucConstantDuty_LUT[mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE + 5][mBLDCM_LUT_SIZE] = {0};

static uint32_t PWM_prvpulPhaseTimerFrequencies[] = {mPHASE_U_TIMER_RUNNIING_FREQUENCY_HZ,
													mPHASE_V_TIMER_RUNNIING_FREQUENCY_HZ,
													mPHASE_W_TIMER_RUNNIING_FREQUENCY_HZ};

static uint8_t PWM_prvpucSPWM_LUT[mBLDCM_LUT_SIZE] = {0, 5, 10, 15, 20, 25, 29, 34,
													  39, 43, 48, 52, 56, 60, 64, 68,
													  72, 75, 78, 81, 84, 87, 89, 91, 93,
													  95, 96, 97, 98, 99, 100, 100, 100,
													  100, 99, 98, 97, 96, 95, 93, 91, 89,
													  87, 84, 81, 78, 75, 72, 68, 64, 60,
													  56, 52, 48, 43, 39, 34, 29, 25, 20,
													  15, 10, 5, 0,};

static void PWM_prvvAdjustSPWMFrequency(const float fMotorSpeedPercent);



void PWM_vInit(const PWM_ConfigTypeDef * const pxConfigStruct)
{
	if (pxConfigStruct != NULL)
	{
		PWM_prvpxConfigStruct = pxConfigStruct;

		for (uint8_t ucIndex = 0; ucIndex < mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE + 5; ++ucIndex)
		{

			for (uint8_t ucElementIndex = 0; ucElementIndex < mBLDCM_LUT_SIZE - 1; ++ucElementIndex)
			{
				PWM_prvpucConstantDuty_LUT[ucIndex][ucElementIndex] = ucIndex;
			}

			PWM_prvpucConstantDuty_LUT[ucIndex][mBLDCM_LUT_SIZE - 1] = 0;

			if (ucIndex < 3)
			{
				__HAL_TIM_SET_COMPARE(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
									  PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex],
									  0);

				HAL_TIM_PWM_Start_DMA(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex],
										 PWM_prvpxConfigStruct->pulPhaseTimerChannels[ucIndex],
										 (uint32_t *)PWM_prvpucConstantDuty_LUT[0],
										 mBLDCM_LUT_SIZE);
			}
		}
	}
}


void PWM_vStart(const PWM_ModesTypeDef xMode, const PWM_PhaseTypeDef xPhase)
{
	if (PWM_prvpxConfigStruct != NULL)
	{

		BLDCM_SpeedChangeTypeDef DesiredChange = BLDCM_xGetDesiredSpeedChange();

		switch ( xMode )
		{
			case PWM:

				uint8_t ucDutyCycle = BLDCM_fGetMotorActualRPMPercent();

				if ( PWM_Phase_U == xPhase )
				{
					(void) PWM_prvvAdjustSPWMFrequency( BLDCM_fGetMotorActualRPMPercent() );
				}

				if ( DesiredChange == BLDCM_SPEED_INCREASING )
				{
					ucDutyCycle += 2;
				}
				else if ( DesiredChange == BLDCM_SPEED_DECREASING )
				{
					if ( ucDutyCycle >= 2 )
					{
						ucDutyCycle -= 2;
					}
					else
					{
						ucDutyCycle = 0;
					}
				}


				if ( ucDutyCycle < mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE )
				{
					HAL_TIM_PWM_Start_DMA(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
										  PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase],
										  (uint32_t *)PWM_prvpucConstantDuty_LUT[ucDutyCycle],
										  mBLDCM_LUT_SIZE);

					HAL_TIMEx_PWMN_Start_DMA(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
							  	  	  	  	 PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase],
											 (uint32_t *)PWM_prvpucConstantDuty_LUT[ucDutyCycle],
											 mBLDCM_LUT_SIZE);
				}

				break;

			case SPWM:

				if ( PWM_Phase_U == xPhase )
				{
					if ( DesiredChange == BLDCM_SPEED_CONSTANT )
					{
						(void) PWM_prvvAdjustSPWMFrequency( BLDCM_fGetMotorActualRPMPercent() );
					}
					else
					{
						(void) PWM_prvvAdjustSPWMFrequency( BLDCM_fGetMotorDesiredRPMPercent() );
					}
				}

				HAL_TIM_PWM_Start_DMA(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
									  PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase],
									  (uint32_t *)PWM_prvpucSPWM_LUT,
									  mBLDCM_LUT_SIZE);

				HAL_TIMEx_PWMN_Start_DMA(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
						  	  	  	  	 PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase],
										 (uint32_t *)PWM_prvpucSPWM_LUT,
										 mBLDCM_LUT_SIZE);

				break;

			default: break;
		}
	}
}


void PWM_vStop(const PWM_PhaseTypeDef xPhase)
{
	if (PWM_prvpxConfigStruct != NULL)
	{
		__HAL_TIM_SET_COMPARE(PWM_prvpxConfigStruct->ppxPhaseTimerHandles[xPhase],
							  PWM_prvpxConfigStruct->pulPhaseTimerChannels[xPhase],
							  0);
	}
}




void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
	if (PWM_prvpxConfigStruct != NULL)
	{
		if ( (&mPHASE_U_TIMER_HANDLE == htim) && (mPHASE_U_TIMER_ACTIVE_CHANNEL == htim->Channel) )
		{
			if ( BLDCM_xGetState() == BLDCM_RUNNING )
			{
				PWM_vStart(SPWM, PWM_Phase_V);
			}
			else
			{
				PWM_vStart(PWM, PWM_Phase_V);
			}
		}
		else if ((&mPHASE_V_TIMER_HANDLE == htim) && (mPHASE_V_TIMER_ACTIVE_CHANNEL == htim->Channel) )
		{
			if ( BLDCM_xGetState() == BLDCM_RUNNING )
			{
				PWM_vStart(SPWM, PWM_Phase_W);
			}
			else
			{
				PWM_vStart(PWM, PWM_Phase_W);
			}
		}
	}
}


static void PWM_prvvAdjustSPWMFrequency(const float fMotorSpeedPercent)
{
	if (PWM_prvpxConfigStruct != NULL)
	{
		if ( (0 <= fMotorSpeedPercent) && (fMotorSpeedPercent <= 100) )
		{
			// SPWM frequency needs to be twice as fast the electrical cycle's frequency.
			float fElectricalCycleFrequencyInHz = ((BLDCM_fGetMotorActualRPM() * mBLDCM_POLE_PAIRS) / 60);

			uint32_t ulTimerAutoreload;
			uint32_t ulNewPrescaler;

			for (uint8_t ucIndex = 0; ucIndex < 3; ++ucIndex)
			{
				ulTimerAutoreload = __HAL_TIM_GET_AUTORELOAD( PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex] );
				ulNewPrescaler = (PWM_prvpulPhaseTimerFrequencies[ucIndex] / (mBLDCM_LUT_SIZE * ulTimerAutoreload * 2 * fElectricalCycleFrequencyInHz));

				__HAL_TIM_SET_PRESCALER( PWM_prvpxConfigStruct->ppxPhaseTimerHandles[ucIndex], ulNewPrescaler );
			}
		}
		else
		{
			/* Switch to minumum SPWM frequency */
		}
	}
}

//static void PWM_prvvSetDutyCycle(TIM_HandleTypeDef *htim, uint32_t pwmChannel, uint8_t DutyCyclePercentage)
//{
//	if ( (0 <= DutyCyclePercentage) && (DutyCyclePercentage <= 100) )
//	{
//		uint32_t pwmMaxCounterValue = __HAL_TIM_GET_AUTORELOAD(htim);
//		uint32_t pwmNewCounterValue = (uint32_t)( ((float)DutyCyclePercentage / 100.0f) * (float)pwmMaxCounterValue );
//
//		__HAL_TIM_SET_COMPARE(htim, pwmChannel, pwmNewCounterValue);
//	}
//}
