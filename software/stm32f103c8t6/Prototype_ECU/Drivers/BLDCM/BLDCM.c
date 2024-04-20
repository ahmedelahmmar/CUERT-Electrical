
#include "BLDCM.h"
#include "BLDCM_Cfg.h"


static BLDCM_StateTypeDef 			BLDCM_prvxMotorActualState;
static BLDCM_StateTypeDef  			BLDCM_prvxMotorTransitionState;
static BLDCM_CommutationTypeDef 	BLDCM_prvfSPWMActualFrequency;

static float						BLDCM_prvfPWMActualDutyCycle;
static float						BLDCM_prvfPWMDesiredDutyCycle;

static float						BLDCM_prvfMotorActualDesiredRPM;
static float						BLDCM_prvfMotorPreviousDesiredRPM;
static float						BLDCM_prvfMotorDesiredRPMPercent;

static float						BLDCM_prvfMotorActualCurrentRPM;
static float						BLDCM_prvfMotorPreviousActualRPM;
static float						BLDCM_prvfMotorActualRPMPercent;


static void BLDCM_prvvPWM180(void);
static void BLDCM_prvvSPWM180(void);



void BLDCM_vInit(void)
{
	(void) BLDCM_vSetState(BLDCM_IDLE);
	(void) BLDCM_vSetTransitionState(BLDCM_IDLE);

	BLDCM_prvfPWMActualDutyCycle = 0;
}



void BLDCM_vSetState(BLDCM_StateTypeDef xState)
{
	if (xState < BLDCM_STATE_LIMIT)
	{
		BLDCM_prvxMotorActualState = xState;
	}
}



void BLDCM_vSetTransitionState(BLDCM_StateTypeDef xState)
{
	if (xState < BLDCM_STATE_LIMIT)
	{
		BLDCM_prvxMotorTransitionState = xState;
	}
}



void BLDCM_vUpdateMotorDesiredSpeedParameters(float fDesiredRPM)
{
	if (fDesiredRPM >= 0)
	{
		BLDCM_prvfMotorPreviousDesiredRPM = BLDCM_prvfMotorActualDesiredRPM;
		BLDCM_prvfMotorActualDesiredRPM = fDesiredRPM;
		BLDCM_prvfMotorDesiredRPMPercent = (BLDCM_prvfMotorActualDesiredRPM / BLDCM_MAX_SPEED_RPM) * 100.0f;
	}
}



void BLDCM_vUpdateMotorActualSpeedParameters(uint32_t ulTotalNumberOfTicks)
{
	float fElectricalCyclePeriodInSeconds = ( (float)ulTotalNumberOfTicks / (float)mICU_TIMER_RUNNING_FREQUENCY_HZ );
	float fMechanicalCyclePeriodInSeconds = fElectricalCyclePeriodInSeconds * BLDCM_POLE_PAIRS;

	BLDCM_prvfMotorPreviousActualRPM = BLDCM_prvfMotorActualCurrentRPM;
	BLDCM_prvfMotorActualCurrentRPM = fMechanicalCyclePeriodInSeconds * 60;
	BLDCM_prvfMotorActualRPMPercent = (BLDCM_prvfMotorActualCurrentRPM / BLDCM_MAX_SPEED_RPM) * 100.0f;
}



void BLDCM_vCommutate(BLDCM_CommutationTypeDef xCommutationLogic)
{
	if ( (xCommutationLogic < BLDCM_COMMUTATION_LIMIT) && (BLDCM_IDLE != BLDCM_prvxMotorActualState) )
	{
		switch (xCommutationLogic)
		{
			case BLDCM_COMMUTATION_PWM_180:

				(void) BLDCM_prvvPWM180();

				break;

			case BLDCM_COMMUTATION_SPWM_180:

				(void) BLDCM_prvvSPWM180();

				break;

			default: break;
		}

	}
}



float BLDCM_fGetDesiredRPM(void)
{
	return BLDCM_prvfMotorActualDesiredRPM;
}



float BLDCM_fGetDesiredRPMPercent(void)
{
	return BLDCM_prvfMotorDesiredRPMPercent;
}



float BLDCM_fGetMotorActualRPM(void)
{
	return BLDCM_prvfMotorActualCurrentRPM;

}



float BLDCM_fGetMotorActualRPMPercent(void)
{
	return BLDCM_prvfMotorActualRPMPercent;
}



BLDCM_StateTypeDef BLDCM_xGetState(void)
{
	return BLDCM_prvxMotorActualState;
}



BLDCM_StateTypeDef BLDCM_xGetTransitionState(void)
{
	return BLDCM_prvxMotorTransitionState;
}



BLDCM_StateTypeDef BLDCM_xStartMotor()
{
	BLDCM_StateTypeDef xReturn;

	while ( (BLDCM_IDLE_STATE_THRESHOLD_PERCENTAGE < BLDCM_prvfMotorDesiredRPMPercent)
			&&
			(BLDCM_prvfMotorDesiredRPMPercent < BLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE) )
	{
		BLDCM_prvfPWMActualDutyCycle = BLDCM_prvfMotorDesiredRPMPercent; /* Should be increased gradually */

		BLDCM_vCommutate(BLDCM_COMMUTATION_PWM_180);
	}

	if ( BLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE < BLDCM_prvfMotorDesiredRPMPercent )
	{
		xReturn = BLDCM_STARTING_SUCCEEDED;

		(void) PWM_vSetModeTo(SPWM);
	}
	else
	{
		xReturn = BLDCM_STARTING_FAILED;
	}

	return xReturn;
}



BLDCM_StateTypeDef BLDCM_xStopMotor()
{
	BLDCM_StateTypeDef xReturn;

	while ( (BLDCM_IDLE_STATE_THRESHOLD_PERCENTAGE < BLDCM_prvfMotorDesiredRPMPercent)
			&&
			(BLDCM_prvfMotorDesiredRPMPercent < BLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE) )
	{
		/* if the acutal speed is still greater than 20% then decrease SPWM frequency gradually while using SPWM
		 * else switch to PWM and decrease PWM duty cycle gradually starting from 20% duty cycle */

		BLDCM_prvfPWMActualDutyCycle = BLDCM_prvfMotorDesiredRPMPercent; /* Should be decreased gradually */

		/* Should check our speed and according to it decide whether we will use pwm or spwm */
		BLDCM_vCommutate(BLDCM_COMMUTATION_PWM_180);
	}

	if (BLDCM_prvfMotorDesiredRPMPercent < BLDCM_IDLE_STATE_THRESHOLD_PERCENTAGE)
	{
		xReturn = BLDCM_STOPPING_SUCCEEDED;
	}
	else
	{
		xReturn = BLDCM_STOPPING_FAILED;

		(void) PWM_vSetModeTo(SPWM);
	}

	return xReturn;
}



static void BLDCM_prvvPWM180(void)
{
	switch ( HALL_xGetSector() )
	{
		case (HALL_SECTOR_1):


			PWM_vStart(PWM_Phase_U, BLDCM_prvfPWMActualDutyCycle);
			PWM_vStop(PWM_Phase_V);
			PWM_vStart(PWM_Phase_W, BLDCM_prvfPWMActualDutyCycle);

			break;


		case (HALL_SECTOR_2):

//			PWM_vStart(PWM_Phase_U, BLDCM_prvfPWMActualDutyCycle);
//			PWM_vStop(PWM_Phase_V);
			PWM_vStop(PWM_Phase_W);

			break;


		case (HALL_SECTOR_3):

//			PWM_vStart(PWM_Phase_U, BLDCM_prvfPWMActualDutyCycle);
			PWM_vStart(PWM_Phase_V, BLDCM_prvfPWMActualDutyCycle);
//			PWM_vStop(PWM_Phase_W);

			break;

		case (HALL_SECTOR_4):

			PWM_vStop(PWM_Phase_U);
//			PWM_vStart(PWM_Phase_V, BLDCM_prvfPWMActualDutyCycle);
//			PWM_vStop(PWM_Phase_W);

			break;


		case (HALL_SECTOR_5):

//			PWM_vStop(PWM_Phase_U);
//			PWM_vStart(PWM_Phase_V, BLDCM_prvfPWMActualDutyCycle);
			PWM_vStart(PWM_Phase_W, BLDCM_prvfPWMActualDutyCycle);

			break;


		case (HALL_SECTOR_6):

//			PWM_vStop(PWM_Phase_U);
			PWM_vStop(PWM_Phase_V);
//			PWM_vStart(PWM_Phase_W, BLDCM_prvfPWMActualDutyCycle);

			break;


		default: break;
	}
}



BLDCM_SpeedChangeTypeDef BLDCM_xGetActualSpeedChange(void)
{
	BLDCM_SpeedChangeTypeDef xReturn;

	if ( BLDCM_prvfMotorPreviousActualRPM < BLDCM_prvfMotorActualCurrentRPM )
	{
		xReturn = BLDCM_SPEED_INCREASING;
	}
	else if ( BLDCM_prvfMotorPreviousActualRPM == BLDCM_prvfMotorActualCurrentRPM )
	{
		xReturn = BLDCM_SPEED_CONSTANT;
	}
	else
	{
		xReturn = BLDCM_SPEED_DECREASING;
	}

	return xReturn;
}



BLDCM_SpeedChangeTypeDef BLDCM_xGetDesiredSpeedChange(void)
{
	BLDCM_SpeedChangeTypeDef xReturn;

	if ( BLDCM_prvfMotorPreviousDesiredRPM < BLDCM_prvfMotorActualDesiredRPM )
	{
		xReturn = BLDCM_SPEED_INCREASING;
	}
	else if ( BLDCM_prvfMotorPreviousDesiredRPM == BLDCM_prvfMotorActualDesiredRPM )
	{
		xReturn = BLDCM_SPEED_CONSTANT;
	}
	else
	{
		xReturn = BLDCM_SPEED_DECREASING;
	}

	return xReturn;
}



static void BLDCM_prvvSPWM180(void)
{
	if ( HALL_SECTOR_1 == HALL_xGetSector() )
	{
		PWM_vStartSPWM(PWM_Phase_U);
	}
}


