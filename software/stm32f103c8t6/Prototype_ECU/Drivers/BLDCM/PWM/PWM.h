#ifndef _SPWM_H_
#define _SPWM_H_

#include "../BLDCM.h"
#include "../BLDCM_Cfg.h"
#include "../Hall/Hall.h"

typedef enum
{
	PWM_Phase_U = 0,
	PWM_Phase_V,
	PWM_Phase_W,

} PWM_PhaseTypeDef;

typedef struct
{
	TIM_HandleTypeDef* 		ppxPhaseTimerHandles[3];
	uint32_t				pulPhaseTimerChannels[3];

} PWM_ConfigTypeDef;

void PWM_vInit(const PWM_ConfigTypeDef * const pxConfigStruct);
void PWM_vStart(const PWM_ConfigTypeDef * const pxConfigStruct, const PWM_PhaseTypeDef xHallPhase, const uint8_t ucDutyCycle);
void PWM_vStop(const PWM_ConfigTypeDef * const pxConfigStruct, const PWM_PhaseTypeDef xHallPhase);
void PWM_vDeInit(const PWM_ConfigTypeDef * const pxConfigStruct);

void PWM_vSetSPWMFrequency(const PWM_ConfigTypeDef * const pxConfigStruct, const uint32_t ulFrequency);
void PWM_vStartSPWM(const PWM_ConfigTypeDef * const pxConfigStruct, const PWM_PhaseTypeDef xHallPhase);


#endif /* _SPWM_H_ */
