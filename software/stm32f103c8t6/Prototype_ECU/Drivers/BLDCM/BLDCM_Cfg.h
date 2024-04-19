#ifndef _BLDCM_CFG_H_
#define _BLDCM_CFG_H_

#include "stm32f1xx_hal.h"

#define BLDCM_POLES							24

#define BLDCM_POLE_PAIRS					(BLDCM_POLES / 2)

#define BLDCM_PWM_SWITCHING_FREQUENCY		1500

#define BLDCM_SPWM_LUT_SIZE					125

#endif /* _BLDCM_CFG_H_ */
