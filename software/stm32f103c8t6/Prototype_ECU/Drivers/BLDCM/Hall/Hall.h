#ifndef _HALL_H_
#define _HALL_H_

#include "stm32f1xx_hal.h"

typedef enum
{
	HALL_Sensor_U = 0,
	HALL_Sensor_V,
	HALL_Sensor_W,

} HALL_SensorTypeDef;


typedef enum
{
	HALL_SECTOR_1 = 0b110,
	HALL_SECTOR_2 = 0b100,
	HALL_SECTOR_3 = 0b101,
	HALL_SECTOR_4 = 0b001,
	HALL_SECTOR_5 = 0b011,
	HALL_SECTOR_6 = 0b010,

} HALL_SectorTypeDef;


typedef struct
{
	GPIO_TypeDef*	ppxSensorPorts[3];
	uint32_t		pulSensorPins[3];

} HALL_ConfigTypeDef;


void HALL_vInit(const HALL_ConfigTypeDef * const pxConfigStruct);
HALL_SectorTypeDef HALL_xGetSector(void);

#endif
