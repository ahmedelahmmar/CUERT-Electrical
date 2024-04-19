#ifndef _HALL_H_
#define _HALL_H_

#include "stm32f1xx_hal.h"


typedef enum
{
	HALL_Sensor_U = 0,
	HALL_Sensor_V,
	HALL_Sensor_W,

} HALL_SensorTypeDef;

typedef struct
{
	GPIO_TypeDef*	ppxHallSensorGpioPorts[3];
	uint32_t		pulHallSensorGpioPins[3];

} HALL_ConfigTypeDef;


void HALL_vInit(const HALL_ConfigTypeDef * const pxHallStruct);
void HALL_vUpdateSector(const HALL_ConfigTypeDef * const pxHallStruct);
void HALL_vGetSector(const HALL_ConfigTypeDef * const pxHallStruct, uint8_t * const pucBuffer);

#endif
