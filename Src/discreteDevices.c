/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "discreteDevices.h"

/* ###########################################################################*/
/* Functions -----------------------------------------------------------------*/
void contactor(const DevState state)
{
	HAL_GPIO_WritePin(Gate_GPIO_Port, Gate_Pin, (GPIO_PinState)state);
}

void charger(const DevState state)
{
	//HAL_GPIO_WritePin(ChargeOn_GPIO_Port, ChargeOn_Pin, (GPIO_PinState)state);
}

void motor(const DevState state)
{
	HAL_GPIO_WritePin(Motor_GPIO_Port, Motor_Pin, (GPIO_PinState)state);
}

void led20(const DevState state)
{
	HAL_GPIO_WritePin(Led20_GPIO_Port, Led20_Pin, (GPIO_PinState)state);
}

void led40(const DevState state)
{
	HAL_GPIO_WritePin(Led40_GPIO_Port, Led40_Pin, (GPIO_PinState)state);
}

void led60(const DevState state)
{
	HAL_GPIO_WritePin(Led60_GPIO_Port, Led60_Pin, (GPIO_PinState)state);
}

void led80(const DevState state)
{
	HAL_GPIO_WritePin(Led80_GPIO_Port, Led80_Pin, (GPIO_PinState)state);
}

void led100(const DevState state)
{
	HAL_GPIO_WritePin(Led100_GPIO_Port, Led100_Pin, (GPIO_PinState)state);
}

void led20Toggle()
{
	HAL_GPIO_TogglePin(Led20_GPIO_Port, Led20_Pin);
}

void led40Toggle()
{
	HAL_GPIO_TogglePin(Led40_GPIO_Port, Led40_Pin);
}

void led60Toggle()
{
	HAL_GPIO_TogglePin(Led60_GPIO_Port, Led60_Pin);
}

void led80Toggle()
{
	HAL_GPIO_TogglePin(Led80_GPIO_Port, Led80_Pin);
}

void led100Toggle()
{
	HAL_GPIO_TogglePin(Led100_GPIO_Port, Led100_Pin);
}

void setBalance(const CellIndex index, const BalanceState state)
{
	GPIO_TypeDef* Balance_GPIO_Port = Balance1_GPIO_Port;
	uint16_t Balance_Pin = Balance1_Pin;
	switch (index)
	{
	case Cell_1: 
		Balance_GPIO_Port = Balance1_GPIO_Port;
		Balance_Pin = Balance1_Pin;
		break;
	case Cell_2: 
		Balance_GPIO_Port = Balance2_GPIO_Port;
		Balance_Pin = Balance2_Pin; 
		break;
	case Cell_3: 			
		Balance_GPIO_Port = Balance3_GPIO_Port;
		Balance_Pin = Balance3_Pin; 
		break;
	case Cell_4: 
		Balance_GPIO_Port = Balance4_GPIO_Port;
		Balance_Pin = Balance4_Pin; 
		break;
	}
	HAL_GPIO_WritePin(Balance_GPIO_Port, Balance_Pin, (GPIO_PinState)state);
}