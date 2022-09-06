/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "statusRegisters.h"
#include "main.h"
#include "cell.h"
#include "chargeStatus.h"
#include "batteryStatus.h"
#include "safetyStatus.h"
#include "usart.h"

/* ###########################################################################*/
/* Local variables -----------------------------------------------------------*/
/** 
  * @brief Status Registers Structure definition
  */
volatile struct StatusRegisters
{
	uint16_t firmwareVersion;
	uint16_t batteryVoltage;
	uint16_t cellVoltageMax;
	uint16_t cellVoltageMin;
	int8_t cellTempMax;
	int8_t cellTempMin;
	int8_t radiatorTemperature;
	uint8_t stateOfCharge;
	uint16_t batteryStatus;
	uint16_t safetyAlert;
	uint16_t safetyStatus;
	uint16_t chargeAlert;
	uint16_t chargeStatus;
	uint16_t ADCVoltage1;
	uint16_t ADCVoltage2;
	uint16_t ADCVoltage3;
	uint16_t ADCVoltage4;
	uint16_t cellVoltage1;
	uint16_t cellVoltage2;
	uint16_t cellVoltage3;
	uint16_t cellVoltage4;
	int8_t cellTemp1;
	int8_t cellTemp2;
	int8_t cellTemp3;
	int8_t cellTemp4;
	uint16_t cellStatus1;
	uint16_t cellStatus2;
	uint16_t cellStatus3;
	uint16_t cellStatus4;
	uint8_t emptyByteToFullSize;    // RESERVED !!!
	uint8_t checksum;
	// Use the data placement rule to align!
} statusRegisters;

_Static_assert(sizeof(statusRegisters) == TX_BUFFER_SIZE, "Error: TX_BUFFER_SIZE invalid");

bool sendStatusRegisters = false;

/* ###########################################################################*/
/* Functions -----------------------------------------------------------------*/
void updateStatusRegisters()
{
	statusRegisters.batteryVoltage = getADCVoltage(Cell_4);
	statusRegisters.cellVoltageMax = getMVoltage(Cell_Status_CVMAX_Msk);
	statusRegisters.cellVoltageMin = getMVoltage(Cell_Status_CVMIN_Msk);
	statusRegisters.cellTempMax = getMTemperature(Cell_Status_CTMAX_Msk);
	statusRegisters.cellTempMin = getMTemperature(Cell_Status_CTMIN_Msk);
	statusRegisters.radiatorTemperature = getRadiatorTemperature();
	statusRegisters.stateOfCharge = getStateOfCharge();
	statusRegisters.batteryStatus = getBatteryStatus();
	statusRegisters.safetyAlert = getSafetyAlert();
	statusRegisters.safetyStatus = getSafetyStatus();
	statusRegisters.chargeAlert = getChargeAlert();
	statusRegisters.chargeStatus = getChargeStatus();
	statusRegisters.ADCVoltage1 = getADCVoltage(Cell_1);
	statusRegisters.ADCVoltage2 = getADCVoltage(Cell_2);
	statusRegisters.ADCVoltage3 = getADCVoltage(Cell_3);
	statusRegisters.ADCVoltage4 = getADCVoltage(Cell_4);
	statusRegisters.cellVoltage1 = getCellVoltage(Cell_1);
	statusRegisters.cellVoltage2 = getCellVoltage(Cell_2);
	statusRegisters.cellVoltage3 = getCellVoltage(Cell_3);
	statusRegisters.cellVoltage4 = getCellVoltage(Cell_4);
	statusRegisters.cellTemp1 = getCellTemperature(Cell_1);
	statusRegisters.cellTemp2 = getCellTemperature(Cell_2);
	statusRegisters.cellTemp3 = getCellTemperature(Cell_3);
	statusRegisters.cellTemp4 = getCellTemperature(Cell_4);
	statusRegisters.cellStatus1 = getCellStatus(Cell_1);
	statusRegisters.cellStatus2 = getCellStatus(Cell_2);
	statusRegisters.cellStatus3 = getCellStatus(Cell_3);
	statusRegisters.cellStatus4 = getCellStatus(Cell_4);
	statusRegisters.checksum = calculateChecksum((uint8_t*)&statusRegisters, sizeof(statusRegisters) - 1);
	if (sendStatusRegisters == true)
	{
		UART_TransmitData((uint8_t*)&statusRegisters, sizeof(statusRegisters));
		sendStatusRegisters = false;
	}
}

void canSendStatusRegistersUART()
{
	sendStatusRegisters = true;
}

void loadFirmwareVersion()
{
	statusRegisters.firmwareVersion = FIRMWARE_VERSION;
}