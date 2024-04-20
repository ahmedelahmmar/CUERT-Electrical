#ifndef _BLDCM_H_
#define _BLDCM_H_

#include "stm32f1xx_hal.h"
#include "Hall/Hall.h"
#include "PWM/PWM.h"

/* U,V, and W Phase Defines */
#define mPHASE_U_TIMER_HANDLE				htim1
#define mPHASE_V_TIMER_HANDLE				htim1
#define mPHASE_W_TIMER_HANDLE				htim1

#define mPHASE_U_TIMER_CHANNEL				TIM_CHANNEL_1
#define mPHASE_V_TIMER_CHANNEL				TIM_CHANNEL_2
#define mPHASE_W_TIMER_CHANNEL				TIM_CHANNEL_3

#define mPHASE_U_TIMER_ACTIVE_CHANNEL		HAL_TIM_ACTIVE_CHANNEL_1
#define mPHASE_V_TIMER_ACTIVE_CHANNEL		HAL_TIM_ACTIVE_CHANNEL_2
#define mPHASE_W_TIMER_ACTIVE_CHANNEL		HAL_TIM_ACTIVE_CHANNEL_3


/* Timer Defines */
#define mICU_TIMER_HANDLE					htim1
#define mICU_TIMER_RUNNING_FREQUENCY_HZ		72e6


/* External Handles */
extern TIM_HandleTypeDef 					mPHASE_U_TIMER_HANDLE;
extern TIM_HandleTypeDef 					mPHASE_V_TIMER_HANDLE;
extern TIM_HandleTypeDef 					mPHASE_U_TIMER_HANDLE;
extern TIM_HandleTypeDef 					mICU_TIMER_HANDLE;

typedef enum
{
	BLDCM_IDLE = 0,
	BLDCM_STARTING,
	BLDCM_STARTING_FAILED,
	BLDCM_STARTING_SUCCEEDED,
	BLDCM_RUNNING,
	BLDCM_STOPPING,
	BLDCM_STOPPING_FAILED,
	BLDCM_STOPPING_SUCCEEDED,
	BLDCM_STATE_LIMIT

} BLDCM_StateTypeDef;

typedef enum
{
	BLDCM_COMMUTATION_PWM_180 = 0,
	BLDCM_COMMUTATION_SPWM_180,
	BLDCM_COMMUTATION_LIMIT,

} BLDCM_CommutationTypeDef;

typedef enum
{
	BLDCM_SPEED_INCREASING = 0,
	BLDCM_SPEED_DECREASING,
	BLDCM_SPEED_CONSTANT

} BLDCM_SpeedChangeTypeDef;


void 						BLDCM_vInit(void);
void 						BLDCM_vSetState(BLDCM_StateTypeDef xState);
void 						BLDCM_vSetTransitionState(BLDCM_StateTypeDef xState);
void 						BLDCM_vUpdateMotorDesiredSpeedParameters(float fDesiredRPM);
void 						BLDCM_vUpdateMotorActualSpeedParameters(uint32_t ulTotalNumberOfTicks);
void 						BLDCM_vCommutate(BLDCM_CommutationTypeDef xCommutationLogic);

float 						BLDCM_fGetMotorActualRPM(void);
float						BLDCM_fGetMotorActualRPMPercent(void);
float	 					BLDCM_fGetDesiredRPMPercent(void);
float						BLDCM_fGetMotorDesiredRPMPercent(void);

BLDCM_StateTypeDef 			BLDCM_xGetState(void);
BLDCM_StateTypeDef 			BLDCM_xGetTransitionState(void);
BLDCM_StateTypeDef 			BLDCM_xStartMotor(void);
BLDCM_StateTypeDef 			BLDCM_xStopMotor(void);

BLDCM_SpeedChangeTypeDef 	BLDCM_xGetActualSpeedChange(void);
BLDCM_SpeedChangeTypeDef 	BLDCM_xGetDesiredSpeedChange(void);

#endif /* _BLDCM_H_ */
