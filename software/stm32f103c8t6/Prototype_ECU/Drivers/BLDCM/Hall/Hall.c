
#include "../../BLDCM/Hall/Hall.h"

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

} xHall = {0};


void HALL_vInit(const HALL_ConfigTypeDef * const pxHallStruct)
{
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/*Configure GPIO pin of Hall U */
	GPIO_InitStruct.Pin = pxHallStruct->pulHallSensorGpioPins[HALL_Sensor_U];
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(pxHallStruct->ppxHallSensorGpioPorts[HALL_Sensor_U], &GPIO_InitStruct);

	/*Configure GPIO pin of Hall V */
	GPIO_InitStruct.Pin = pxHallStruct->pulHallSensorGpioPins[HALL_Sensor_V];
	HAL_GPIO_Init(pxHallStruct->ppxHallSensorGpioPorts[HALL_Sensor_V], &GPIO_InitStruct);

	/*Configure GPIO pin of Hall W */
	GPIO_InitStruct.Pin = pxHallStruct->pulHallSensorGpioPins[HALL_Sensor_W];
	HAL_GPIO_Init(pxHallStruct->ppxHallSensorGpioPorts[HALL_Sensor_W], &GPIO_InitStruct);
}

void HALL_vUpdateSector(const HALL_ConfigTypeDef * const pxHallStruct);
void HALL_vGetSector(const HALL_ConfigTypeDef * const pxHallStruct, uint8_t * const pucBuffer);
