
#include "BLDCM.h"
#include "BLDCM_Cfg.h"


static BLDCM_StateTypeDef 			BLDCM_prvxMotorActualState;
//static BLDCM_StateTypeDef  			BLDCM_prvxMotorTransitionState;
static BLDCM_CommutationTypeDef 	BLDCM_prvfSPWMActualFrequency;

static float						BLDCM_prvfPWMActualDutyCycle;
static float						BLDCM_prvfPWMDesiredDutyCycle;

static float						BLDCM_prvfMotorActualDesiredRPM;
static float						BLDCM_prvfMotorPreviousDesiredRPM;
static float						BLDCM_prvfMotorDesiredRPMPercent;

static float						BLDCM_prvfMotorActualCurrentRPM;
static float						BLDCM_prvfMotorPreviousActualRPM;
static float						BLDCM_prvfMotorActualRPMPercent;


void BLDCM_vInit(void)
{
	(void) BLDCM_vSetState(BLDCM_IDLE);

	BLDCM_prvfPWMActualDutyCycle = 0;
}



void BLDCM_vSetState(BLDCM_StateTypeDef xState)
{
	if (xState < BLDCM_STATE_LIMIT)
	{
		BLDCM_prvxMotorActualState = xState;
	}
}



//void BLDCM_vSetTransitionState(BLDCM_StateTypeDef xState)
//{
//	if (xState < BLDCM_STATE_LIMIT)
//	{
//		BLDCM_prvxMotorTransitionState = xState;
//	}
//}



void BLDCM_vUpdateMotorDesiredSpeedParameters(float fDesiredRPM)
{
	if (fDesiredRPM >= 0)
	{
		BLDCM_prvfMotorPreviousDesiredRPM = BLDCM_prvfMotorActualDesiredRPM;
		BLDCM_prvfMotorActualDesiredRPM = fDesiredRPM;
		BLDCM_prvfMotorDesiredRPMPercent = (BLDCM_prvfMotorActualDesiredRPM / mBLDCM_MAX_SPEED_RPM) * 100.0f;
	}
}



void BLDCM_vUpdateMotorActualSpeedParameters(uint32_t ulTotalNumberOfTicks)
{
	float fElectricalCyclePeriodInSeconds = ( (float)ulTotalNumberOfTicks / (float)mICU_TIMER_RUNNING_FREQUENCY_HZ );
	float fMechanicalCyclePeriodInSeconds = fElectricalCyclePeriodInSeconds * mBLDCM_POLE_PAIRS;

	BLDCM_prvfMotorPreviousActualRPM = BLDCM_prvfMotorActualCurrentRPM;

	BLDCM_prvfMotorActualCurrentRPM = 60 / fMechanicalCyclePeriodInSeconds;
	BLDCM_prvfMotorActualRPMPercent = (BLDCM_prvfMotorActualCurrentRPM / mBLDCM_MAX_SPEED_RPM) * 100.0f;
}



void BLDCM_vCommutate(void)
{
	switch ( HALL_xGetSector() )
	{
		case HALL_SECTOR_1:

			PWM_vStart(PWM_Phase_U);
			PWM_vStop(PWM_Phase_V);
			PWM_vStart(PWM_Phase_W);
			break;

		case HALL_SECTOR_2:

			PWM_vStart(PWM_Phase_U);
			PWM_vStop(PWM_Phase_V);
			PWM_vStop(PWM_Phase_W);
			break;

		case HALL_SECTOR_3:

			PWM_vStart(PWM_Phase_U);
			PWM_vStart(PWM_Phase_V);
			PWM_vStop(PWM_Phase_W);
			break;

		case HALL_SECTOR_4:

			PWM_vStop(PWM_Phase_U);
			PWM_vStart(PWM_Phase_V);
			PWM_vStop(PWM_Phase_W);
			break;

		case HALL_SECTOR_5:

			PWM_vStop(PWM_Phase_U);
			PWM_vStart(PWM_Phase_V);
			PWM_vStart(PWM_Phase_W);
			break;

		case HALL_SECTOR_6:

			PWM_vStop(PWM_Phase_U);
			PWM_vStop(PWM_Phase_V);
			PWM_vStart(PWM_Phase_W);
			break;

//	case HALL_SECTOR_1:
//
//		PWM_vStart(PWM_Phase_U);
//		PWM_vStop(PWM_Phase_V);
//		PWM_vStop(PWM_Phase_W);
//		break;
//
//	case HALL_SECTOR_2:
//
//		PWM_vStart(PWM_Phase_U);
//		PWM_vStart(PWM_Phase_V);
//		PWM_vStop(PWM_Phase_W);
//		break;
//
//	case HALL_SECTOR_3:
//
//		PWM_vStop(PWM_Phase_U);
//		PWM_vStart(PWM_Phase_V);
//		PWM_vStart(PWM_Phase_W);
//		break;
//
//	case HALL_SECTOR_4:
//
//		PWM_vStop(PWM_Phase_U);
//		PWM_vStop(PWM_Phase_V);
//		PWM_vStart(PWM_Phase_W);
//		break;
//
//		default: break;
	}
}



float BLDCM_fGetDesiredRPM(void)
{
	return BLDCM_prvfMotorActualDesiredRPM;
}



float BLDCM_fGetMotorActualRPM(void)
{
	return BLDCM_prvfMotorActualCurrentRPM;

}



float BLDCM_fGetMotorActualRPMPercent(void)
{
	return BLDCM_prvfMotorActualRPMPercent;
}



float BLDCM_fGetMotorDesiredRPMPercent(void)
{
	return BLDCM_prvfMotorDesiredRPMPercent;
}



BLDCM_StateTypeDef BLDCM_xGetState(void)
{
	return BLDCM_prvxMotorActualState;
}



//BLDCM_StateTypeDef BLDCM_xGetTransitionState(void)
//{
//	return BLDCM_prvxMotorTransitionState;
//}



BLDCM_StateTypeDef BLDCM_xStartMotor()
{
	BLDCM_StateTypeDef xReturn;

//#if (mBLDCM_OPERTATION == mBLDCM_SIMULATE )
	uint8_t ucRPMPercent = BLDCM_fGetMotorDesiredRPMPercent();
//#else
//	uint8_t ucRPMPercent = BLDCM_fGetMotorActualRPMPercent();
//#endif

	while ( (mBLDCM_IDLE_STATE_THRESHOLD_PERCENTAGE < ucRPMPercent)
			&&
<<<<<<< HEAD
			(ucRPMPercent < mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE) )
		{
||||||| 61ab53e
			(BLDCM_prvfMotorActualRPMPercent < mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE) )
	{
		BLDCM_prvfPWMActualDutyCycle = BLDCM_prvfMotorActualRPMPercent; /* Should be increased gradually */
=======
			(BLDCM_prvfMotorActualRPMPercent < mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE) )
	{
		BLDCM_prvfPWMActualDutyCycle = BLDCM_prvfMotorActualRPMPercent + 2; /* Should be more than  gradually */
>>>>>>> 8d117f3c019daa2017624ee00f258bfdf0c452c4

			BLDCM_vCommutate();

//#if (mBLDCM_OPERTATION == mBLDCM_SIMULATE )
	ucRPMPercent = BLDCM_fGetMotorDesiredRPMPercent();
//#else
//	ucRPMPercent = BLDCM_fGetMotorActualRPMPercent();
//#endif
		}

	if ( mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE < ucRPMPercent)
	{
		xReturn = BLDCM_STARTING_SUCCEEDED;
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

#if (mBLDCM_OPERTATION == mBLDCM_SIMULATE )
	uint8_t ucRPMPercent = BLDCM_fGetMotorDesiredRPMPercent();
#else
	uint8_t ucRPMPercent = BLDCM_fGetMotorActualRPMPercent();
#endif

	while ( (mBLDCM_IDLE_STATE_THRESHOLD_PERCENTAGE < ucRPMPercent)
			&&
			(ucRPMPercent < mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE) )
	{

		BLDCM_vCommutate();

//		/* if the acutal speed is still greater than 20% then decrease SPWM frequency gradually while using SPWM
//		 * else switch to PWM and decrease PWM duty cycle gradually starting from 20% duty cycle */
//
//		BLDCM_prvfPWMActualDutyCycle = BLDCM_prvfMotorActualRPMPercent; /* Should be decreased gradually */
//
//		/* Should check our speed and according to it decide whether we will use pwm or spwm */
//		BLDCM_vCommutate();
#if (mBLDCM_OPERTATION == mBLDCM_SIMULATE )
	ucRPMPercent = BLDCM_fGetMotorDesiredRPMPercent();
#else
	ucRPMPercent = BLDCM_fGetMotorActualRPMPercent();
#endif
	}

	if (ucRPMPercent < mBLDCM_TRANSITION_STATE_THRESHOLD_PERCENTAGE)
	{
		xReturn = BLDCM_STOPPING_SUCCEEDED;
	}
	else
	{
		xReturn = BLDCM_STOPPING_FAILED;

	}

	return xReturn;
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

	// Todo: add tolerance

	if ( BLDCM_prvfMotorActualCurrentRPM < BLDCM_prvfMotorActualDesiredRPM )
	{
		xReturn = BLDCM_SPEED_INCREASING;
	}
	else if ( BLDCM_prvfMotorActualCurrentRPM == BLDCM_prvfMotorActualDesiredRPM )
	{
		xReturn = BLDCM_SPEED_CONSTANT;
	}
	else
	{
		xReturn = BLDCM_SPEED_DECREASING;
	}

	return xReturn;
}
