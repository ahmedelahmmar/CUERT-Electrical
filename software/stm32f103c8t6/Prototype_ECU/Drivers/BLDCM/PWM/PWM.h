#ifndef _SPWM_H_
#define _SPWM_H_

#include "../BLDCM.h"

typedef enum
{
	PWM_Phase_U = 0,
	PWM_Phase_V,
	PWM_Phase_W,

} PWM_PhaseTypeDef;

typedef enum
{
	PWM = 0,
	SPWM,

} PWM_ModesTypeDef;

typedef struct
{
	TIM_HandleTypeDef* 		ppxPhaseTimerHandles[3];
	uint32_t				pulPhaseTimerChannels[3];

} PWM_ConfigTypeDef;

void PWM_vInit(const PWM_ConfigTypeDef * const pxConfigStruct);
void PWM_vStart(const PWM_ModesTypeDef xMode, const PWM_PhaseTypeDef xPhase);
void PWM_vStop(const PWM_PhaseTypeDef xHallPhase);

#endif /* _SPWM_H_ */
