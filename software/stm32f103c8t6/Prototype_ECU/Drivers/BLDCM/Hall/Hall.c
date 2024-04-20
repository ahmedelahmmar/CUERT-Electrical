
#include "../../BLDCM/Hall/Hall.h"


static const HALL_ConfigTypeDef *HALL_pxConfigStruct;

static union
{
	uint8_t Sector;

	struct
	{
		uint8_t W 			: 1;
		uint8_t V 			: 1;
		uint8_t U 			: 1;
		uint8_t Reserved 	: 5;

	} Bits;

} HALL_xSectorState = {0};



void HALL_vInit(const HALL_ConfigTypeDef * const pxConfigStruct)
{
	if (pxConfigStruct != NULL)
	{
		HALL_pxConfigStruct = pxConfigStruct;
	}
}

extern __IO union
{
	uint8_t Sector;
	struct
	{
		uint8_t HallW 		: 1;
		uint8_t HallV 		: 1;
		uint8_t HallU 		: 1;
		uint8_t Reserved 	: 5;
	} Instance;

} HallSensors;

HALL_SectorTypeDef HALL_xGetSector(void)
{
//	return HALL_xSectorState.Sector;
	return HallSensors.Sector;
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HALL_xSectorState.Bits.U = ( (HALL_pxConfigStruct->ppxSensorPorts[HALL_Sensor_U]->IDR & HALL_pxConfigStruct->pulSensorPins[HALL_Sensor_U]) != 0 );
	HALL_xSectorState.Bits.V = ( (HALL_pxConfigStruct->ppxSensorPorts[HALL_Sensor_V]->IDR & HALL_pxConfigStruct->pulSensorPins[HALL_Sensor_V]) != 0 );
	HALL_xSectorState.Bits.W = ( (HALL_pxConfigStruct->ppxSensorPorts[HALL_Sensor_W]->IDR & HALL_pxConfigStruct->pulSensorPins[HALL_Sensor_W]) != 0 );
}

