#ifndef _BLDCM_H_
#define _BLDCM_H_

#include "stm32f1xx_hal.h"

typedef enum
{
	BLDCM_IDLE = 0,
	BLDCM_STARTING,
	BLDCM_RUNNING,
	BLDCM_STOPPING

} BLDCM_StatesTypeDef;

#endif /* _BLDCM_H_ */
