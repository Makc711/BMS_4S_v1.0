#ifndef __discreteDevices_H
#define __discreteDevices_H

/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cell.h"

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
typedef enum
{
	OFF = GPIO_PIN_RESET,
	ON  = GPIO_PIN_SET
} DevState;

/* ###########################################################################*/
/* Prototipes ----------------------------------------------------------------*/
void contactor(const DevState state);
void charger(const DevState state);
void motor(const DevState state);
void led20(const DevState state);
void led40(const DevState state);
void led60(const DevState state);
void led80(const DevState state);
void led100(const DevState state);
void led20Toggle();
void led40Toggle();
void led60Toggle();
void led80Toggle();
void led100Toggle();
void setBalance(const CellIndex index, const BalanceState state);

#endif /* __discreteDevices_H */