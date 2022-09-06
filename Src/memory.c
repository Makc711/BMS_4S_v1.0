/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "memory.h"
#include "usart.h"
#include <string.h>

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
#define FLASH_PAGE    31 // Last page
#define FLASH_ADDRESS FLASH_BASE + FLASH_PAGE * FLASH_PAGE_SIZE // Start addresse of eeprom (flash)

// Default ADC_Coefficients
#define ADC_U1_K_DEF	1047683 // * DIGITS_AFTER_POIT_ADC_K (Umax=4300)
#define ADC_U2_K_DEF	2051282 // * DIGITS_AFTER_POIT_ADC_K (Umax=8400)
#define ADC_U3_K_DEF	3024000 // * DIGITS_AFTER_POIT_ADC_K (Umax=12400)
#define ADC_U4_K_DEF	3968503 // * DIGITS_AFTER_POIT_ADC_K (Umax=16300)
#define ADC_TEMP_K_DEF	18662   // * DIGITS_AFTER_POIT_ADC_K

// Default Settings_BMS
#define BALANCE_VOLTAGE_THRESHOLD_DEF			3520 // mV
#define BALANCE_VOLTAGE_RECOVERY_DEF			3365 // mV
#define BALANCE_TIME_DEF						120  // s
#define CHARGE_VOLTAGE_THRESHOLD_DEF			3600 // mV
#define CHARGE_VOLTAGE_RECOVERY_DEF				3350 // mV
#define CHARGE_COMPLETION_TIME_DEF				5    // s
#define DISCHARGE_VOLTAGE_RECOVERY_DEF			2850 // mV
#define DISCHARGE_COMPLETION_TIME_DEF			5    // s
#define REMAINING_CAPACITY_ALARM_PERCENT_DEF	25   // %
#define CV20_THRESHOLD_DEF						3100 // mV
#define CV20_RECOVERY_DEF						3000 // mV
#define CV20_TIME_DEF							10   // s
#define CV40_THRESHOLD_DEF						3150 // mV
#define CV40_RECOVERY_DEF						3100 // mV
#define CV40_TIME_DEF							10   // s
#define CV60_THRESHOLD_DEF						3250 // mV
#define CV60_RECOVERY_DEF						3150 // mV
#define CV60_TIME_DEF							10   // s
#define CV80_THRESHOLD_DEF						3400 // mV
#define CV80_RECOVERY_DEF						3200 // mV
#define CV80_TIME_DEF							10   // s
#define CV100_THRESHOLD_DEF						3550 // mV
#define CV100_RECOVERY_DEF						3250 // mV
#define CV100_TIME_DEF							10   // s
#define BALANCE_DELTA_VOLTAGE_DEF				50   // mV

// Default Settings_Alarm
#define COVC_THRESHOLD_DEF	3650 // mV
#define COVC_RECOVERY_DEF	3300 // mV
#define COVC_TIME_DEF		3    // s
#define COVT_THRESHOLD_DEF	3700 // mV
#define COVT_RECOVERY_DEF	3375 // mV
#define COVT_TIME_DEF		5    // s
#define CUV_THRESHOLD_DEF	2800 // mV
#define CUV_RECOVERY_DEF	2850 // mV
#define CUV_TIME_DEF		5    // s
#define COT_THRESHOLD_DEF	60   // 'C
#define COT_RECOVERY_DEF	50   // 'C
#define COT_TIME_DEF		0    // s
#define CUT_THRESHOLD_DEF	-40  // 'C
#define CUT_RECOVERY_DEF	-30  // 'C
#define CUT_TIME_DEF		0    // s
#define ROT_THRESHOLD_DEF	70   // 'C
#define ROT_RECOVERY_DEF	50   // 'C
#define ROT_TIME_DEF		5    // s
#define RUT_THRESHOLD_DEF	-40  // 'C
#define RUT_RECOVERY_DEF	-30  // 'C
#define RUT_TIME_DEF		5    // s
#define UTC_THRESHOLD_DEF	-10  // 'C
#define UTC_RECOVERY_DEF	-5   // 'C
#define UTC_TIME_DEF		0    // s
#define UTD_THRESHOLD_DEF	-40  // 'C
#define UTD_RECOVERY_DEF	-35  // 'C
#define UTD_TIME_DEF		0    // s

/* ###########################################################################*/
/* Variables -----------------------------------------------------------------*/
/** 
  * @brief Settings ADC Coefficients Structure definition
  */
volatile struct ADC_Coefficients
{
	uint32_t ADC_U_K[NUMBER_OF_CELLS];	// divide by DIGITS_AFTER_POIT_ADC_K
	uint32_t ADC_TEMP_K;	// divide by DIGITS_AFTER_POIT_ADC_K
	// Use the data placement rule to align!
	// 20 bytes = 5  32-bits words.  It's - OK
	// !!! Full size (bytes) must be a multiple of 4 !!!
} settingsADC;
#define SETTINGS_ADC_ADDRESS   FLASH_ADDRESS
const uint8_t settingsADCWords = sizeof(settingsADC) / sizeof(uint32_t);

/** 
  * @brief Settings BMS Structure definition
  */
volatile struct Settings_Charge
{
	uint16_t Balance_Voltage_Threshold; /*!< Voltage above which balancing resistors turn on, mV */
	uint16_t Balance_Voltage_Recovery; /*!< Voltage below which balancing resistors turn off, mV */
	uint8_t Balance_Time; /*!< Maximum runtime balancing resistors without control, s */
	uint8_t Charge_Completion_Time; /*!< Storage time for setting the CHARGE_STATUS_CC bit (battery charge completed), s */
	uint16_t Charge_Voltage_Threshold; /*!< The threshold of the event of reaching the minimum voltage of the element AB level of the full charge, mV */
	uint16_t Charge_Voltage_Recovery; /*!< Threshold for resetting the event of reaching the minimum voltage of the element AB of the level of full charge, mV */
	uint16_t Discharge_Voltage_Recovery; /*!< Threshold for resetting the event of reaching the minimum voltage of the element AB level of full discharge, mV */
	uint8_t Discharge_Completion_Time; /*!< Storage time for setting the bit CHARGE_STATUS_DC (battery fully discharged), s */
	uint8_t Remaining_Capacity_Alarm_Percent; /*!< The percentage of charge for the low charge signal CHARGE_STATUS_RCA, if zero, the function is disabled, % */
	uint16_t CV20_Threshold; /*!< Cell Voltage 20% Threshold, mV */
	uint16_t CV20_Recovery; /*!< Cell Voltage 20% Recovery, mV */
	uint8_t CV20_Time; /*!< Cell Voltage 20% Time, s */
	uint8_t CV40_Time; /*!< Cell Voltage 40% Time, s */
	uint16_t CV40_Threshold; /*!< Cell Voltage 40% Threshold, mV */
	uint16_t CV40_Recovery; /*!< Cell Voltage 40% Recovery, mV */
	uint16_t CV60_Threshold; /*!< Cell Voltage 60% Threshold, mV */
	uint16_t CV60_Recovery; /*!< Cell Voltage 60% Recovery, mV */
	uint8_t CV60_Time; /*!< Cell Voltage 60% Time, s */
	uint8_t CV80_Time; /*!< Cell Voltage 80% Time, s */
	uint16_t CV80_Threshold; /*!< Cell Voltage 80% Threshold, mV */
	uint16_t CV80_Recovery; /*!< Cell Voltage 80% Recovery, mV */
	uint16_t CV100_Threshold; /*!< Cell Voltage 100% Threshold, mV */
	uint16_t CV100_Recovery; /*!< Cell Voltage 100% Recovery,mV */
	uint8_t CV100_Time; /*!< Cell Voltage 100% Time, s */
	uint8_t Balance_Delta_Voltage; /* Voltage difference above which balancing resistors turn on, mV */
	// Use the data placement rule to align!
	// 40 bytes = 10  32-bits words.  It's - OK
	// !!! Full size (bytes) must be a multiple of 4 !!!
} settingsCharge;
#define SETTINGS_BMS_ADDRESS   SETTINGS_ADC_ADDRESS + sizeof(settingsADC)
const uint8_t settingsBMSWords = sizeof(settingsCharge) / sizeof(uint32_t);

/** 
  * @brief Settings Alarm Structure definition
  */
volatile struct Settings_Alarm
{
	uint16_t COVC_Threshold; /*!< Cell Over Voltage Charger Threshold, mV */
	uint16_t COVC_Recovery; /*!< Cell Over Voltage Charger Recovery, mV */
	uint8_t COVC_Time; /*!< Cell Over Voltage Charger Time, s */
	uint8_t COVT_Time; /*!< Cell Over Voltage Transistor Time, s */
	uint16_t COVT_Threshold; /*!< Cell Over Voltage Transistor Threshold, mV */
	uint16_t COVT_Recovery; /*!< Cell Over Voltage Transistor Recovery, mV */
	uint16_t CUV_Threshold; /*!< Cell Under Voltage Threshold, mV */
	uint16_t CUV_Recovery; /*!< Cell Under Voltage Recovery, mV */
	uint8_t CUV_Time; /*!< Cell Under Voltage Time, s */
	int8_t COT_Threshold; /*!< Cell Over Temperature Threshold, 'C */  // RESERVED !!!
	int8_t COT_Recovery; /*!< Cell Over Temperature Recovery, 'C */  // RESERVED !!!
	uint8_t COT_Time; /*!< Cell Over Temperature Time, s */  // RESERVED !!!
	int8_t CUT_Threshold; /*!< Cell Under Temperature Threshold, 'C */  // RESERVED !!!
	int8_t CUT_Recovery; /*!< Cell Under Temperature Recovery, 'C */  // RESERVED !!!
	uint8_t CUT_Time; /*!< Cell Under Temperature Time, s */  // RESERVED !!!
	int8_t ROT_Threshold; /*!< Radiator Over Temperature Threshold, 'C */
	int8_t ROT_Recovery; /*!< Radiator Over Temperature Recovery, 'C */
	uint8_t ROT_Time; /*!< Radiator Over Temperature Time, s */
	int8_t RUT_Threshold; /*!< Radiator Under Temperature Threshold, 'C */
	int8_t RUT_Recovery; /*!< Radiator Under Temperature Recovery, 'C */
	uint8_t RUT_Time; /*!< Radiator Under Temperature Time, s */
	int8_t UTC_Threshold; /*!< Under Temperature Charge Threshold, 'C */  // RESERVED !!!
	int8_t UTC_Recovery; /*!< Under Temperature Charge Recovery, 'C */  // RESERVED !!!
	uint8_t UTC_Time; /*!< Under Temperature Charge Time, s */  // RESERVED !!!
	int8_t UTD_Threshold; /*!< Under Temperature Discharge Threshold, 'C */  // RESERVED !!!
	int8_t UTD_Recovery; /*!< Under Temperature Discharge Recovery, 'C */  // RESERVED !!!
	uint8_t UTD_Time; /*!< Under Temperature Discharge Time, s */  // RESERVED !!!
	uint8_t emptyByteToFullSize;   // RESERVED !!!
	uint16_t emptyHalfWordToFullSize;  // RESERVED !!!
	// Use the data placement rule to align!
	// 36 byte = 9  32-bits words.  It's - OK
	// !!! Full size (bytes) must be a multiple of 4 !!!
} settingsAlarm;
#define SETTINGS_ALARM_ADDRESS   SETTINGS_BMS_ADDRESS + sizeof(settingsCharge)
const uint8_t settingsAlarmWords = sizeof(settingsAlarm) / sizeof(uint32_t);

_Static_assert(max(sizeof(settingsCharge), sizeof(settingsAlarm)) == RX_BUFFER_SIZE, "Error: RX_BUFFER_SIZE invalid");

/* ###########################################################################*/
/* Local prototypes ----------------------------------------------------------*/
void flashUnlock();
void flashLock();
void flashErasePage(const uint32_t address);
void flashWrite(const uint32_t TypeProgram, const uint32_t Address, const uint64_t Data);
uint32_t flashReadWord(const uint32_t address);
void readSettingsFromFlash(const uint32_t settingsAddress, void *settings, const uint8_t size);
void writeSettingsToFlash(const uint32_t settingsAddress, void *settings, const uint8_t size);
bool isMemoryErased(const uint32_t address);

/* ###########################################################################*/
/* Functions -----------------------------------------------------------------*/
void flashUnlock()
{
	while (HAL_FLASH_Unlock() != HAL_OK) {}
}

void flashLock()
{
	while (HAL_FLASH_Lock() != HAL_OK) {}
}

void flashErasePage(const uint32_t address)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = address;
	EraseInitStruct.NbPages = 1;
	uint32_t PAGEError;
	while (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK) {}
}

/**
  * @brief  Program halfword, word or double word at a specified address
  *
  * @param  typeProgram   Indicate the way to program at a specified address.
  *                       This parameter can be:
  *                       - FLASH_TYPEPROGRAM_HALFWORD   <- Program a half-word (16-bit)
  *                       - FLASH_TYPEPROGRAM_WORD       <- Program a word (32-bit)
  *                       - FLASH_TYPEPROGRAM_DOUBLEWORD <- Program a double word (64-bit)
  * @param  address       Specifie the address to be programmed.
  * @param  data          Specifie the data to be programmed
  */
void flashWrite(const uint32_t typeProgram, const uint32_t address, const uint64_t data)
{
	while (HAL_FLASH_Program(typeProgram, address, data) != HAL_OK) {}
}

uint32_t flashReadWord(const uint32_t address)
{
	return (*(__IO uint32_t*) address);
}

void readSettingsFromFlash(const uint32_t settingsAddress, void *settings, const uint8_t size)
{
	uint32_t sourceAddr = settingsAddress;
	uint32_t *destAddr = settings;
	for (uint8_t i = 0; i < size; i++)
	{
		*destAddr = flashReadWord(sourceAddr);
		sourceAddr += sizeof(sourceAddr);
		destAddr++;
	}
}

void writeSettingsToFlash(const uint32_t settingsAddress, void *settings, const uint8_t size)
{
	uint32_t *sourceAddr = settings;
	uint32_t destAddr = settingsAddress;
	for (uint8_t i = 0; i < size; i++)
	{
		flashWrite(FLASH_TYPEPROGRAM_WORD, destAddr, *sourceAddr);
		sourceAddr++;
		destAddr += sizeof(destAddr);
	}
}

void saveSettingsInFlash()
{
	flashUnlock();
	flashErasePage(FLASH_ADDRESS);
	writeSettingsToFlash(SETTINGS_ADC_ADDRESS, (void *)&settingsADC, settingsADCWords);
	writeSettingsToFlash(SETTINGS_BMS_ADDRESS, (void *)&settingsCharge, settingsBMSWords);
	writeSettingsToFlash(SETTINGS_ALARM_ADDRESS, (void *)&settingsAlarm, settingsAlarmWords);
	flashLock();
}

bool isMemoryErased(const uint32_t address)
{
	return flashReadWord(address) == 0xFFFFFFFF;
}

void loadSettings()
{
	if (isMemoryErased(FLASH_ADDRESS))
	{
		setDefaultSettingsADC();
		setDefaultSettingsCharge();
		setDefaultSettingsAlarm();
		saveSettingsInFlash();
	} else
	{
		readSettingsFromFlash(SETTINGS_ADC_ADDRESS, (void *)&settingsADC, settingsADCWords);
		readSettingsFromFlash(SETTINGS_BMS_ADDRESS, (void *)&settingsCharge, settingsBMSWords);
		readSettingsFromFlash(SETTINGS_ALARM_ADDRESS, (void *)&settingsAlarm, settingsAlarmWords);
	}
}

void setDefaultSettingsADC()
{
	settingsADC.ADC_U_K[Cell_1] = ADC_U1_K_DEF;
	settingsADC.ADC_U_K[Cell_2] = ADC_U2_K_DEF;
	settingsADC.ADC_U_K[Cell_3] = ADC_U3_K_DEF;
	settingsADC.ADC_U_K[Cell_4] = ADC_U4_K_DEF;
	settingsADC.ADC_TEMP_K = ADC_TEMP_K_DEF;
}

void setDefaultSettingsCharge()
{
	settingsCharge.Balance_Voltage_Threshold = BALANCE_VOLTAGE_THRESHOLD_DEF;
	settingsCharge.Balance_Voltage_Recovery = BALANCE_VOLTAGE_RECOVERY_DEF;
	settingsCharge.Balance_Time = BALANCE_TIME_DEF;
	settingsCharge.Charge_Voltage_Threshold = CHARGE_VOLTAGE_THRESHOLD_DEF;
	settingsCharge.Charge_Voltage_Recovery = CHARGE_VOLTAGE_RECOVERY_DEF;
	settingsCharge.Charge_Completion_Time = CHARGE_COMPLETION_TIME_DEF;
	settingsCharge.Discharge_Voltage_Recovery = DISCHARGE_VOLTAGE_RECOVERY_DEF;
	settingsCharge.Discharge_Completion_Time = DISCHARGE_COMPLETION_TIME_DEF;
	settingsCharge.Remaining_Capacity_Alarm_Percent = REMAINING_CAPACITY_ALARM_PERCENT_DEF;
	settingsCharge.CV20_Threshold = CV20_THRESHOLD_DEF;
	settingsCharge.CV20_Recovery = CV20_RECOVERY_DEF;
	settingsCharge.CV20_Time = CV20_TIME_DEF;
	settingsCharge.CV40_Threshold = CV40_THRESHOLD_DEF;
	settingsCharge.CV40_Recovery = CV40_RECOVERY_DEF;
	settingsCharge.CV40_Time = CV40_TIME_DEF;
	settingsCharge.CV60_Threshold = CV60_THRESHOLD_DEF;
	settingsCharge.CV60_Recovery = CV60_RECOVERY_DEF;
	settingsCharge.CV60_Time = CV60_TIME_DEF;
	settingsCharge.CV80_Threshold = CV80_THRESHOLD_DEF;
	settingsCharge.CV80_Recovery = CV80_RECOVERY_DEF;
	settingsCharge.CV80_Time = CV80_TIME_DEF;
	settingsCharge.CV100_Threshold = CV100_THRESHOLD_DEF;
	settingsCharge.CV100_Recovery = CV100_RECOVERY_DEF;
	settingsCharge.CV100_Time = CV100_TIME_DEF;
	settingsCharge.Balance_Delta_Voltage = BALANCE_DELTA_VOLTAGE_DEF;
}

void setDefaultSettingsAlarm()
{
	settingsAlarm.COVC_Threshold = COVC_THRESHOLD_DEF;
	settingsAlarm.COVC_Recovery = COVC_RECOVERY_DEF;
	settingsAlarm.COVC_Time = COVC_TIME_DEF;
	settingsAlarm.COVT_Threshold = COVT_THRESHOLD_DEF;
	settingsAlarm.COVT_Recovery = COVT_RECOVERY_DEF;
	settingsAlarm.COVT_Time = COVT_TIME_DEF;
	settingsAlarm.CUV_Threshold = CUV_THRESHOLD_DEF;
	settingsAlarm.CUV_Recovery = CUV_RECOVERY_DEF;
	settingsAlarm.CUV_Time = CUV_TIME_DEF;
	settingsAlarm.COT_Threshold = COT_THRESHOLD_DEF;
	settingsAlarm.COT_Recovery = COT_RECOVERY_DEF;
	settingsAlarm.COT_Time = COT_TIME_DEF;
	settingsAlarm.CUT_Threshold = CUT_THRESHOLD_DEF;
	settingsAlarm.CUT_Recovery = CUT_RECOVERY_DEF;
	settingsAlarm.CUT_Time = CUT_TIME_DEF;
	settingsAlarm.ROT_Threshold = ROT_THRESHOLD_DEF;
	settingsAlarm.ROT_Recovery = ROT_RECOVERY_DEF;
	settingsAlarm.ROT_Time = ROT_TIME_DEF;
	settingsAlarm.RUT_Threshold = RUT_THRESHOLD_DEF;
	settingsAlarm.RUT_Recovery = RUT_RECOVERY_DEF;
	settingsAlarm.RUT_Time = RUT_TIME_DEF;
	settingsAlarm.UTC_Threshold = UTC_THRESHOLD_DEF;
	settingsAlarm.UTC_Recovery = UTC_RECOVERY_DEF;
	settingsAlarm.UTC_Time = UTC_TIME_DEF;
	settingsAlarm.UTD_Threshold = UTD_THRESHOLD_DEF;
	settingsAlarm.UTD_Recovery = UTD_RECOVERY_DEF;
	settingsAlarm.UTD_Time = UTD_TIME_DEF;
}

void sendSettingsChargeUART()
{
	UART_TransmitData((uint8_t*)&settingsCharge, sizeof(settingsCharge));
}

void sendSettingsAlarmUART()
{
	UART_TransmitData((uint8_t*)&settingsAlarm, sizeof(settingsAlarm));
}

void copyDataToSettingsBMS(uint8_t *data, const uint8_t size)
{
	memcpy((uint8_t*)&settingsCharge, data, size);
}

void copyDataToSettingsAlarm(uint8_t *data, const uint8_t size)
{
	memcpy((uint8_t*)&settingsAlarm, data, size);
}

uint8_t getSizeSettingsBMS()
{
	return sizeof(settingsCharge);
}
uint8_t getSizeSettingsAlarm()
{
	return sizeof(settingsAlarm);
}

/* Setters / Getters settingsADC ---------------------------------------------*/
void setADC_U_K(const CellIndex index, const uint32_t value)
{
	settingsADC.ADC_U_K[index] = value;
}
uint32_t getADC_U_K(const CellIndex index)
{
	return settingsADC.ADC_U_K[index];
}
void setADC_TEMP_K(const uint32_t value)
{
	settingsADC.ADC_TEMP_K = value;
}
uint32_t getADC_TEMP_K()
{
	return settingsADC.ADC_TEMP_K;
}

/* Setters / Getters settingsBMS ---------------------------------------------*/
void setBalanceVoltageThreshold(const uint16_t value)
{
	settingsCharge.Balance_Voltage_Threshold = value;
}
uint16_t getBalanceVoltageThreshold()
{
	return settingsCharge.Balance_Voltage_Threshold;
}
void setBalanceVoltageRecovery(const uint16_t value)
{
	settingsCharge.Balance_Voltage_Recovery = value;
}
uint16_t getBalanceVoltageRecovery()
{
	return settingsCharge.Balance_Voltage_Recovery;
}
void setBalanceTime(const uint8_t value)
{
	settingsCharge.Balance_Time = value;
}
uint8_t getBalanceTime()
{
	return settingsCharge.Balance_Time;
}
void setBalanceDeltaVoltage(const uint8_t value)
{
	settingsCharge.Balance_Delta_Voltage = value;
}
uint8_t getBalanceDeltaVoltage()
{
	return settingsCharge.Balance_Delta_Voltage;
}
//----------
void setChargeVoltageThreshold(const uint16_t value)
{
	settingsCharge.Charge_Voltage_Threshold = value;
}
uint16_t getChargeVoltageThreshold()
{
	return settingsCharge.Charge_Voltage_Threshold;
}
void setChargeVoltageRecovery(const uint16_t value)
{
	settingsCharge.Charge_Voltage_Recovery = value;
}
uint16_t getChargeVoltageRecovery()
{
	return settingsCharge.Charge_Voltage_Recovery;
}
void setChargeCompletionTime(const uint8_t value)
{
	settingsCharge.Charge_Completion_Time = value;
}
uint8_t getChargeCompletionTime()
{
	return settingsCharge.Charge_Completion_Time;
}
//----------
void setDischargeVoltageRecovery(const uint16_t value)
{
	settingsCharge.Discharge_Voltage_Recovery = value;
}
uint16_t getDischargeVoltageRecovery()
{
	return settingsCharge.Discharge_Voltage_Recovery;
}
void setDischargeCompletionTime(const uint8_t value)
{
	settingsCharge.Discharge_Completion_Time = value;
}
uint8_t getDischargeCompletionTime()
{
	return settingsCharge.Discharge_Completion_Time;
}
//----------
void setRemainingCapacityAlarmPercent(const uint8_t value)
{
	settingsCharge.Remaining_Capacity_Alarm_Percent = value;
}
uint8_t getRemainingCapacityAlarmPercent()
{
	return settingsCharge.Remaining_Capacity_Alarm_Percent;
}
//----------
void setCV20Threshold(const uint16_t value)
{
	settingsCharge.CV20_Threshold = value;
}
uint16_t getCV20Threshold()
{
	return settingsCharge.CV20_Threshold;
}
void setCV20Recovery(const uint16_t value)
{
	settingsCharge.CV20_Recovery = value;
}
uint16_t getCV20Recovery()
{
	return settingsCharge.CV20_Recovery;
}
void setCV20Time(const uint8_t value)
{
	settingsCharge.CV20_Time = value;
}
uint8_t getCV20Time()
{
	return settingsCharge.CV20_Time;
}
//----------
void setCV40Threshold(const uint16_t value)
{
	settingsCharge.CV40_Threshold = value;
}
uint16_t getCV40Threshold()
{
	return settingsCharge.CV40_Threshold;
}
void setCV40Recovery(const uint16_t value)
{
	settingsCharge.CV40_Recovery = value;
}
uint16_t getCV40Recovery()
{
	return settingsCharge.CV40_Recovery;
}
void setCV40Time(const uint8_t value)
{
	settingsCharge.CV40_Time = value;
}
uint8_t getCV40Time()
{
	return settingsCharge.CV40_Time;
}
//----------
void setCV60Threshold(const uint16_t value)
{
	settingsCharge.CV60_Threshold = value;
}
uint16_t getCV60Threshold()
{
	return settingsCharge.CV60_Threshold;
}
void setCV60Recovery(const uint16_t value)
{
	settingsCharge.CV60_Recovery = value;
}
uint16_t getCV60Recovery()
{
	return settingsCharge.CV60_Recovery;
}
void setCV60Time(const uint8_t value)
{
	settingsCharge.CV60_Time = value;
}
uint8_t getCV60Time()
{
	return settingsCharge.CV60_Time;
}
//----------
void setCV80Threshold(const uint16_t value)
{
	settingsCharge.CV80_Threshold = value;
}
uint16_t getCV80Threshold()
{
	return settingsCharge.CV80_Threshold;
}
void setCV80Recovery(const uint16_t value)
{
	settingsCharge.CV80_Recovery = value;
}
uint16_t getCV80Recovery()
{
	return settingsCharge.CV80_Recovery;
}
void setCV80Time(const uint8_t value)
{
	settingsCharge.CV80_Time = value;
}
uint8_t getCV80Time()
{
	return settingsCharge.CV80_Time;
}
//----------
void setCV100Threshold(const uint16_t value)
{
	settingsCharge.CV100_Threshold = value;
}
uint16_t getCV100Threshold()
{
	return settingsCharge.CV100_Threshold;
}
void setCV100Recovery(const uint16_t value)
{
	settingsCharge.CV100_Recovery = value;
}
uint16_t getCV100Recovery()
{
	return settingsCharge.CV100_Recovery;
}
void setCV100Time(const uint8_t value)
{
	settingsCharge.CV100_Time = value;
}
uint8_t getCV100Time()
{
	return settingsCharge.CV100_Time;
}

/* Setters / Getters settingsAlarm -------------------------------------------*/
void setCOVCThreshold(const uint16_t value)
{
	settingsAlarm.COVC_Threshold = value;
}
uint16_t getCOVCThreshold()
{
	return settingsAlarm.COVC_Threshold;
}
void setCOVCRecovery(const uint16_t value)
{
	settingsAlarm.COVC_Recovery = value;
}
uint16_t getCOVCRecovery()
{
	return settingsAlarm.COVC_Recovery;
}
void setCOVCTime(const uint8_t value)
{
	settingsAlarm.COVC_Time = value;
}
uint8_t getCOVCTime()
{
	return settingsAlarm.COVC_Time;
}
//----------
void setCOVTThreshold(const uint16_t value)
{
	settingsAlarm.COVT_Threshold = value;
}
uint16_t getCOVTThreshold()
{
	return settingsAlarm.COVT_Threshold;
}
void setCOVTRecovery(const uint16_t value)
{
	settingsAlarm.COVT_Recovery = value;
}
uint16_t getCOVTRecovery()
{
	return settingsAlarm.COVT_Recovery;
}
void setCOVTTime(const uint8_t value)
{
	settingsAlarm.COVT_Time = value;
}
uint8_t getCOVTTime()
{
	return settingsAlarm.COVT_Time;
}
//----------
void setCUVThreshold(const uint16_t value)
{
	settingsAlarm.CUV_Threshold = value;
}
uint16_t getCUVThreshold()
{
	return settingsAlarm.CUV_Threshold;
}
void setCUVRecovery(const uint16_t value)
{
	settingsAlarm.CUV_Recovery = value;
}
uint16_t getCUVRecovery()
{
	return settingsAlarm.CUV_Recovery;
}
void setCUVTime(const uint8_t value)
{
	settingsAlarm.CUV_Time = value;
}
uint8_t getCUVTime()
{
	return settingsAlarm.CUV_Time;
}
//----------
void setCOTThreshold(const int8_t value)
{
	settingsAlarm.COT_Threshold = value;
}
int8_t getCOTThreshold()
{
	return settingsAlarm.COT_Threshold;
}
void setCOTRecovery(const int8_t value)
{
	settingsAlarm.COT_Recovery = value;
}
int8_t getCOTRecovery()
{
	return settingsAlarm.COT_Recovery;
}
void setCOTTime(const uint8_t value)
{
	settingsAlarm.COT_Time = value;
}
uint8_t getCOTTime()
{
	return settingsAlarm.COT_Time;
}
//----------
void setCUTThreshold(const int8_t value)
{
	settingsAlarm.CUT_Threshold = value;
}
int8_t getCUTThreshold()
{
	return settingsAlarm.CUT_Threshold;
}
void setCUTRecovery(const int8_t value)
{
	settingsAlarm.CUT_Recovery = value;
}
int8_t getCUTRecovery()
{
	return settingsAlarm.CUT_Recovery;
}
void setCUTTime(const uint8_t value)
{
	settingsAlarm.CUT_Time = value;
}
uint8_t getCUTTime()
{
	return settingsAlarm.CUT_Time;
}
//----------
void setROTThreshold(const int8_t value)
{
	settingsAlarm.ROT_Threshold = value;
}
int8_t getROTThreshold()
{
	return settingsAlarm.ROT_Threshold;
}
void setROTRecovery(const int8_t value)
{
	settingsAlarm.ROT_Recovery = value;
}
int8_t getROTRecovery()
{
	return settingsAlarm.ROT_Recovery;
}
void setROTTime(const uint8_t value)
{
	settingsAlarm.ROT_Time = value;
}
uint8_t getROTTime()
{
	return settingsAlarm.ROT_Time;
}
//----------
void setRUTThreshold(const int8_t value)
{
	settingsAlarm.RUT_Threshold = value;
}
int8_t getRUTThreshold()
{
	return settingsAlarm.RUT_Threshold;
}
void setRUTRecovery(const int8_t value)
{
	settingsAlarm.RUT_Recovery = value;
}
int8_t getRUTRecovery()
{
	return settingsAlarm.RUT_Recovery;
}
void setRUTTime(const uint8_t value)
{
	settingsAlarm.RUT_Time = value;
}
uint8_t getRUTTime()
{
	return settingsAlarm.RUT_Time;
}
//----------
void setUTCThreshold(const int8_t value)
{
	settingsAlarm.UTC_Threshold = value;
}
int8_t getUTCThreshold()
{
	return settingsAlarm.UTC_Threshold;
}
void setUTCRecovery(const int8_t value)
{
	settingsAlarm.UTC_Recovery = value;
}
int8_t getUTCRecovery()
{
	return settingsAlarm.UTC_Recovery;
}
void setUTCTime(const uint8_t value)
{
	settingsAlarm.UTC_Time = value;
}
uint8_t getUTCTime()
{
	return settingsAlarm.UTC_Time;
}
//----------
void setUTDThreshold(const int8_t value)
{
	settingsAlarm.UTD_Threshold = value;
}
int8_t getUTDThreshold()
{
	return settingsAlarm.UTD_Threshold;
}
void setUTDRecovery(const int8_t value)
{
	settingsAlarm.UTD_Recovery = value;
}
int8_t getUTDRecovery()
{
	return settingsAlarm.UTD_Recovery;
}
void setUTDTime(const uint8_t value)
{
	settingsAlarm.UTD_Time = value;
}
uint8_t getUTDTime()
{
	return settingsAlarm.UTD_Time;
}