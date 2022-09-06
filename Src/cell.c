/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "cell.h"
#include "adc.h"
#include "memory.h"
#include "safetyStatus.h"
#include "chargeStatus.h"
#include "discreteDevices.h"

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
#define MEASUREMENTS_BALANCE_PERIOD_DEF		10  // s
#define MEASUREMENTS_DELAY_DEF				5   // s
#define BALANCE_ENABLE_DELAY				2	// s

//#define ADC_U_REF 3300.0 // mV
//#define ADC_MAX_BIT 4095 // bit
//#define ADC_U1_R25 10    // kOhm  <--[_R25_]--u--[_R35_]--|
//#define ADC_U1_R35 33    // kOhm
//#define ADC_U1_K ((ADC_U_REF * (ADC_U1_R35 + ADC_U1_R25) * DIGITS_AFTER_POIT) / (ADC_MAX_BIT * ADC_U1_R35)) // 1,050061 0500610500610500610500611       Umax=4300
//#define ADC_U2_R26 51    // kOhm  <--[_R26_]--u--[_R36_]--|
//#define ADC_U2_R36 33    // kOhm
//#define ADC_U2_K ((ADC_U_REF * (ADC_U2_R36 + ADC_U2_R26) * DIGITS_AFTER_POIT) / (ADC_MAX_BIT * ADC_U2_R36)) // 2,051282 0512820512820512820512821       Umax=8400
//#define ADC_U3_R27 91    // kOhm  <--[_R27_]--u--[_R37_]--|
//#define ADC_U3_R37 33    // kOhm
//#define ADC_U3_K ((ADC_U_REF * (ADC_U3_R37 + ADC_U3_R27) * DIGITS_AFTER_POIT) / (ADC_MAX_BIT * ADC_U3_R37)) // 3,028083 028083028083028083028083        Umax=12400
//#define ADC_U4_R28 130   // kOhm  <--[_R28_]--u--[_R38_]--|
//#define ADC_U4_R38 33    // kOhm
//#define ADC_U4_K ((ADC_U_REF * (ADC_U4_R38 + ADC_U4_R28) * DIGITS_AFTER_POIT) / (ADC_MAX_BIT * ADC_U4_R38)) // 3,980463 980463980463980463980464        Umax=16300

/* ###########################################################################*/
/* Local variables -----------------------------------------------------------*/
typedef volatile struct Cell
{
	uint16_t adcU; /*!< ADC voltage, mV */
	uint16_t voltage; /*!< Cell voltage, mV */
	int16_t  temperature; /*!< Cell temperature, 'C */  // RESERVED !!!
	uint16_t cellStatus; /*!< Event Register (Cell_Status) */
} Cell;
Cell cells[NUMBER_OF_CELLS];
const uint8_t sizeOfMeasurements = sizeof(cells);

typedef volatile struct Timer
{
	uint8_t OVCTime;
	uint8_t OVTTime;
	uint8_t UVTime;
	uint8_t OTTime; // RESERVED !!!
	uint8_t UTTime; // RESERVED !!!
	uint8_t UTCTime; // RESERVED !!!
	uint8_t UTDTime; // RESERVED !!!
	uint8_t BalanceTime;
} Timer;
Timer timer[NUMBER_OF_CELLS]; // переместить в структуру cells, сруктуру по uart отправлять не будем
uint16_t cellAlert[NUMBER_OF_CELLS]; // переместить в структуру cells, сруктуру по uart отправлять не будем
int8_t radiatorTemperature;

/* ###########################################################################*/
/* Local Prototipes ----------------------------------------------------------*/
void updateMeasurementsCellVoltage(const CellIndex cellIndex);
uint16_t calculateVoltage(const uint16_t v1, const uint16_t v2);
void updateCellStatus();
void updateCellAlert(const CellIndex index);
void updateBalance(const CellIndex index);
void updateStatusMeasurementsEnabled();

/* ###########################################################################*/
/* Functions -----------------------------------------------------------------*/
void updateADCMeasurements()
{
	for (CellIndex index = Cell_1; index <= Cell_4; index++)
	{
		updateMeasurementsCellVoltage(index);
	}
	updateCellStatus();
	radiatorTemperature = getADCData(ADC_Temper) * (uint64_t)getADC_TEMP_K() / DIGITS_AFTER_POIT_ADC_K; // Проверить, возможно вычисление неправильно при минусе
}

void updateMeasurementsCellVoltage(const CellIndex index)
{
	cells[index].adcU = getADCData(ADC_U1 + index) * (uint64_t)getADC_U_K(index) / DIGITS_AFTER_POIT_ADC_K;
	cells[index].voltage = (index == Cell_1) 
		? cells[Cell_1].adcU 
		: calculateVoltage(cells[index].adcU, cells[index - 1].adcU);
}

uint16_t calculateVoltage(const uint16_t v1, const uint16_t v2)
{
	return (v1 > v2) ? (v1 - v2) : 0;
}

void updateCellStatus()
{
	CellIndex indexMaxVoltage = Cell_1;
	CellIndex indexMinVoltage = Cell_1;
	for (CellIndex index = Cell_1; index <= Cell_4; index++)
	{
		updateCellAlert(index);
		uint16_t registerStatus = cells[index].cellStatus;
		updateRegisterStatus(&registerStatus, cellAlert[index], Cell_Status_COVC_Msk, timer[index].OVCTime);
		updateRegisterStatus(&registerStatus, cellAlert[index], Cell_Status_COVT_Msk, timer[index].OVTTime);
		updateRegisterStatus(&registerStatus, cellAlert[index], Cell_Status_CUV_Msk, timer[index].UVTime);
		cells[index].cellStatus = registerStatus;
		if (cells[index].voltage > cells[indexMaxVoltage].voltage)
		{
			indexMaxVoltage = index;
		}
		cells[index].cellStatus &= ~Cell_Status_CVMAX_Msk;
		if (cells[index].voltage < cells[indexMinVoltage].voltage)
		{
			indexMinVoltage = index;
		}
		cells[index].cellStatus &= ~Cell_Status_CVMIN_Msk;
	}
	cells[indexMaxVoltage].cellStatus |= Cell_Status_CVMAX_Msk;
	cells[indexMinVoltage].cellStatus |= Cell_Status_CVMIN_Msk;
	for (CellIndex index = Cell_1; index <= Cell_4; index++)
	{
		updateBalance(index);
	}
}

void updateCellAlert(const CellIndex index)
{
	// CELL_STATUS_COVС
	if((cells[index].voltage > getCOVCThreshold()) && ((cellAlert[index] & Cell_Status_COVC_Msk) != Cell_Status_COVC))
	{
		cellAlert[index] |= Cell_Status_COVC_Msk;
		timer[index].OVCTime = getCOVCTime();
	}
	if ((cells[index].voltage < getCOVCRecovery()) && ((cellAlert[index] & Cell_Status_COVC_Msk) == Cell_Status_COVC))
	{
		cellAlert[index] &= ~Cell_Status_COVC_Msk;
		timer[index].OVCTime = getCOVCTime();
	}
	// CELL_STATUS_COVT
	if((cells[index].voltage > getCOVTThreshold()) && ((cellAlert[index] & Cell_Status_COVT_Msk) != Cell_Status_COVT))
	{
		cellAlert[index] |= Cell_Status_COVT_Msk;
		timer[index].OVTTime = getCOVTTime();
	}
	if ((cells[index].voltage < getCOVTRecovery()) && ((cellAlert[index] & Cell_Status_COVT_Msk) == Cell_Status_COVT))
	{
		cellAlert[index] &= ~Cell_Status_COVT_Msk;
		timer[index].OVTTime = getCOVTTime();
	}
	// CELL_STATUS_CUV
	if((cells[index].voltage < getCUVThreshold()) && ((cellAlert[index] & Cell_Status_CUV_Msk) != Cell_Status_CUV))
	{
		cellAlert[index] |= Cell_Status_CUV_Msk;
		timer[index].UVTime = getCUVTime();
	}
	if ((cells[index].voltage > getCUVRecovery()) && ((cellAlert[index] & Cell_Status_CUV_Msk) == Cell_Status_CUV))
	{
		cellAlert[index] &= ~Cell_Status_CUV_Msk;
		timer[index].UVTime = getCUVTime();
	}
}

void updateRegisterStatus(uint16_t *registerStatus, const uint16_t registerAlert, const uint16_t mask, const uint8_t timer)
{
	if(((registerAlert & mask) != (*registerStatus & mask)) && (timer == 0))
	{
		setStatus(registerStatus, mask, (registerAlert & mask) == mask);
	}
}

void updateBalance(const CellIndex index)
{
	const uint16_t deltaVoltage = cells[index].voltage - getMVoltage(Cell_Status_CVMIN_Msk);
	if ((cells[index].voltage > getBalanceVoltageThreshold()) && (deltaVoltage >= getBalanceDeltaVoltage()))
	{
		balance(index, BalanceON);
	}
	if (((cells[index].voltage < getBalanceVoltageRecovery()) || (deltaVoltage < getBalanceDeltaVoltage()) || (timer[index].BalanceTime == 0))  && ((cells[index].cellStatus & Cell_Status_Balancing_Msk) == Cell_Status_Balancing))
	{
		balance(index, BalanceOFF);
	}
}

uint16_t getADCVoltage(const CellIndex index)
{
	return cells[index].adcU;
}

uint16_t getCellVoltage(const CellIndex index)
{
	return cells[index].voltage;
}

int8_t getCellTemperature(const CellIndex index)
{
	return cells[index].temperature;
}

uint16_t getCellStatus(const CellIndex index)
{
	return cells[index].cellStatus;
}

uint16_t getCellAlert(const CellIndex index)
{
	return cellAlert[index];
}

int8_t getRadiatorTemperature()
{
	return radiatorTemperature;
}

/**
  * @brief  Get MIN or MAX battery voltage
  *
  * @param  mask  Mask of maximum or minimum battery voltage.
  *               This parameter can be:
  *               - Cell_Status_CVMAX_Msk   <- MAX battery voltage
  *               - Cell_Status_CVMIN_Msk   <- MIN battery voltage
  */
uint16_t getMVoltage(const uint16_t mask)
{
	uint16_t value = 0;
	for (CellIndex index = Cell_1; index <= Cell_4; index++)
	{
		if ((cells[index].cellStatus & mask) == mask)
		{
			value = cells[index].voltage;
			break;
		}
	}
	return value;
}

/**
  * @brief  Get MIN or MAX battery temperature
  *
  * @param  mask  Mask of maximum or minimum battery temperature.
  *               This parameter can be:
  *               - Cell_Status_CTMAX_Msk   <- MAX battery temperature
  *               - Cell_Status_CTMIN_Msk   <- MIN battery temperature
  */
int8_t getMTemperature(const uint16_t mask)
{
	int8_t value = 0;
	for (CellIndex index = Cell_1; index <= Cell_4; index++)
	{
		if ((cells[index].cellStatus & mask) == mask)
		{
			value = cells[index].temperature;
			break;
		}
	}
	return value;
}

void updateBalanceStatus(const CellIndex index, const BalanceState state)
{
	uint16_t registerStatus = cells[index].cellStatus;
	setStatus(&registerStatus, Cell_Status_Balancing_Msk, state == BalanceON);
	cells[index].cellStatus = registerStatus;
}

void decrementTimers()
{
	for (CellIndex index = Cell_1; index <= Cell_4; index++)
	{
		timer[index].OVCTime = decrementTime(timer[index].OVCTime);
		timer[index].OVTTime = decrementTime(timer[index].OVTTime);
		timer[index].UVTime = decrementTime(timer[index].UVTime);
		timer[index].OTTime = decrementTime(timer[index].OTTime);
		timer[index].UTTime = decrementTime(timer[index].UTTime);
		timer[index].UTCTime = decrementTime(timer[index].UTCTime);
		timer[index].UTDTime = decrementTime(timer[index].UTDTime);
		timer[index].BalanceTime = decrementTime(timer[index].BalanceTime);
	}
	decrementTimerSafety();
	decrementTimerCharge();
}

uint8_t decrementTime(const uint8_t time)
{
	return (time > 0) ? (time - 1) : 0;
}

void balance(const CellIndex index, const BalanceState state)
{
	if (state == BalanceON)
	{
		timer[index].BalanceTime = getBalanceTime();
	}
	setBalance(index, state);
	updateBalanceStatus(index, state);
}

bool isAllCellsBalanced()
{
	bool value = true;
	for (CellIndex index = Cell_1; index <= Cell_4; index++)
	{
		if ((cells[index].cellStatus & Cell_Status_Balancing_Msk) != Cell_Status_Balancing)
		{
			value = false;
			break;
		}
	}
	return value;
}

bool isCellBalanced()
{
	bool value = false;
	for (CellIndex index = Cell_1; index <= Cell_4; index++)
	{
		if ((cells[index].cellStatus & Cell_Status_Balancing_Msk) == Cell_Status_Balancing)
		{
			value = true;
			break;
		}
	}
	return value;
}

void setStatus(uint16_t *registerStatus, const uint16_t mask, const bool state)
{
	*registerStatus = (state == true) ? (*registerStatus | mask) : (*registerStatus & ~mask);
}