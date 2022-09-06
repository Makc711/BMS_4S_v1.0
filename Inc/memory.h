#ifndef __memory_H
#define __memory_H

/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cell.h"

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
#define DIGITS_AFTER_POIT_ADC_K 1000000

/* ###########################################################################*/
/* Prototipes ----------------------------------------------------------------*/
void loadSettings();
void saveSettingsInFlash();
void sendSettingsChargeUART();
void sendSettingsAlarmUART();
void copyDataToSettingsBMS(uint8_t *data, const uint8_t size);
void copyDataToSettingsAlarm(uint8_t *data, const uint8_t size);
uint8_t getSizeSettingsBMS();
uint8_t getSizeSettingsAlarm();
void setDefaultSettingsADC();
void setDefaultSettingsCharge();
void setDefaultSettingsAlarm();

/* Setters / Getters settingsADC ---------------------------------------------*/
void setADC_U_K(const CellIndex index, const uint32_t value);
uint32_t getADC_U_K(const CellIndex index);
void setADC_TEMP_K(const uint32_t value);
uint32_t getADC_TEMP_K();

/* Setters / Getters settingsBMS ---------------------------------------------*/
void setBalanceVoltageThreshold(const uint16_t value);
uint16_t getBalanceVoltageThreshold();
void setBalanceVoltageRecovery(const uint16_t value);
uint16_t getBalanceVoltageRecovery();
void setBalanceTime(const uint8_t value);
uint8_t getBalanceTime();
void setBalanceDeltaVoltage(const uint8_t value);
uint8_t getBalanceDeltaVoltage();
//----------
void setChargeVoltageThreshold(const uint16_t value);
uint16_t getChargeVoltageThreshold();
void setChargeVoltageRecovery(const uint16_t value);
uint16_t getChargeVoltageRecovery();
void setChargeCompletionTime(const uint8_t value);
uint8_t getChargeCompletionTime();
//----------
void setDischargeVoltageRecovery(const uint16_t value);
uint16_t getDischargeVoltageRecovery();
void setDischargeCompletionTime(const uint8_t value);
uint8_t getDischargeCompletionTime();
//----------
void setRemainingCapacityAlarmPercent(const uint8_t value);
uint8_t getRemainingCapacityAlarmPercent();
//----------
void setCV20Threshold(const uint16_t value);
uint16_t getCV20Threshold();
void setCV20Recovery(const uint16_t value);
uint16_t getCV20Recovery();
void setCV20Time(const uint8_t value);
uint8_t getCV20Time();
//----------
void setCV40Threshold(const uint16_t value);
uint16_t getCV40Threshold();
void setCV40Recovery(const uint16_t value);
uint16_t getCV40Recovery();
void setCV40Time(const uint8_t value);
uint8_t getCV40Time();
//----------
void setCV60Threshold(const uint16_t value);
uint16_t getCV60Threshold();
void setCV60Recovery(const uint16_t value);
uint16_t getCV60Recovery();
void setCV60Time(const uint8_t value);
uint8_t getCV60Time();
//----------
void setCV80Threshold(const uint16_t value);
uint16_t getCV80Threshold();
void setCV80Recovery(const uint16_t value);
uint16_t getCV80Recovery();
void setCV80Time(const uint8_t value);
uint8_t getCV80Time();
//----------
void setCV100Threshold(const uint16_t value);
uint16_t getCV100Threshold();
void setCV100Recovery(const uint16_t value);
uint16_t getCV100Recovery();
void setCV100Time(const uint8_t value);
uint8_t getCV100Time();

/* Setters / Getters settingsAlarm -------------------------------------------*/
void setCOVCThreshold(const uint16_t value);
uint16_t getCOVCThreshold();
void setCOVCRecovery(const uint16_t value);
uint16_t getCOVCRecovery();
void setCOVCTime(const uint8_t value);
uint8_t getCOVCTime();
//----------
void setCOVTThreshold(const uint16_t value);
uint16_t getCOVTThreshold();
void setCOVTRecovery(const uint16_t value);
uint16_t getCOVTRecovery();
void setCOVTTime(const uint8_t value);
uint8_t getCOVTTime();
//----------
void setCUVThreshold(const uint16_t value);
uint16_t getCUVThreshold();
void setCUVRecovery(const uint16_t value);
uint16_t getCUVRecovery();
void setCUVTime(const uint8_t value);
uint8_t getCUVTime();
//----------
void setCOTThreshold(const int8_t value);
int8_t getCOTThreshold();
void setCOTRecovery(const int8_t value);
int8_t getCOTRecovery();
void setCOTTime(const uint8_t value);
uint8_t getCOTTime();
//----------
void setCUTThreshold(const int8_t value);
int8_t getCUTThreshold();
void setCUTRecovery(const int8_t value);
int8_t getCUTRecovery();
void setCUTTime(const uint8_t value);
uint8_t getCUTTime();
//----------
void setROTThreshold(const int8_t value);
int8_t getROTThreshold();
void setROTRecovery(const int8_t value);
int8_t getROTRecovery();
void setROTTime(const uint8_t value);
uint8_t getROTTime();
//----------
void setRUTThreshold(const int8_t value);
int8_t getRUTThreshold();
void setRUTRecovery(const int8_t value);
int8_t getRUTRecovery();
void setRUTTime(const uint8_t value);
uint8_t getRUTTime();
//----------
void setUTCThreshold(const int8_t value);
int8_t getUTCThreshold();
void setUTCRecovery(const int8_t value);
int8_t getUTCRecovery();
void setUTCTime(const uint8_t value);
uint8_t getUTCTime();
//----------
void setUTDThreshold(const int8_t value);
int8_t getUTDThreshold();
void setUTDRecovery(const int8_t value);
int8_t getUTDRecovery();
void setUTDTime(const uint8_t value);
uint8_t getUTDTime();

//2222222222222222222222222222222222222222222222222222222222
//#define CHECKSUM_CONSTANT 44111
//
///******************  Bit definition for Event Register  *******************/
//#define Event_Safety_Status_C1OV_Pos             (0U)                               
//#define Event_Safety_Status_C1OV_Msk             (0x1U << Event_Safety_Status_C1OV_Pos)        /*!< 0x0000 0001 */
//#define Event_Safety_Status_C1OV                 Event_Safety_Status_C1OV_Msk                  /*!< Cell 1 Overvoltage */
//#define Event_Safety_Status_C1UV_Pos             (1U)                               
//#define Event_Safety_Status_C1UV_Msk             (0x1U << Event_Safety_Status_C1UV_Pos)        /*!< 0x0000 0002 */
//#define Event_Safety_Status_C1UV                 Event_Safety_Status_C1UV_Msk                  /*!< Cell 1 Undervoltage */
//#define Event_Safety_Status_C2OV_Pos             (2U)                               
//#define Event_Safety_Status_C2OV_Msk             (0x1U << Event_Safety_Status_C2OV_Pos)        /*!< 0x0000 0004 */
//#define Event_Safety_Status_C2OV                 Event_Safety_Status_C2OV_Msk                  /*!< Cell 2 Overvoltage */
//#define Event_Safety_Status_C2UV_Pos             (3U)                               
//#define Event_Safety_Status_C2UV_Msk             (0x1U << Event_Safety_Status_C2UV_Pos)        /*!< 0x0000 0008 */
//#define Event_Safety_Status_C2UV                 Event_Safety_Status_C2UV_Msk                  /*!< Cell 2 Undervoltage */
//#define Event_Safety_Status_C3OV_Pos             (4U)                               
//#define Event_Safety_Status_C3OV_Msk             (0x1U << Event_Safety_Status_C3OV_Pos)        /*!< 0x0000 0010 */
//#define Event_Safety_Status_C3OV                 Event_Safety_Status_C3OV_Msk                  /*!< Cell 3 Overvoltage */
//#define Event_Safety_Status_C3UV_Pos             (5U)                               
//#define Event_Safety_Status_C3UV_Msk             (0x1U << Event_Safety_Status_C3UV_Pos)        /*!< 0x0000 0020 */
//#define Event_Safety_Status_C3UV                 Event_Safety_Status_C3UV_Msk                  /*!< Cell 3 Undervoltage */
//#define Event_Safety_Status_C4OV_Pos             (6U)                               
//#define Event_Safety_Status_C4OV_Msk             (0x1U << Event_Safety_Status_C4OV_Pos)        /*!< 0x0000 0040 */
//#define Event_Safety_Status_C4OV                 Event_Safety_Status_C4OV_Msk                  /*!< Cell 4 Overvoltage */
//#define Event_Safety_Status_C4UV_Pos             (7U)                               
//#define Event_Safety_Status_C4UV_Msk             (0x1U << Event_Safety_Status_C4UV_Pos)        /*!< 0x0000 0080 */
//#define Event_Safety_Status_C4UV                 Event_Safety_Status_C4UV_Msk                  /*!< Cell 4 Undervoltage */
//#define Event_Safety_Status_C1OT_Pos             (8U)                                             // RESERVED !!!
//#define Event_Safety_Status_C1OT_Msk             (0x1U << Event_Safety_Status_C1OT_Pos)        /*!< 0x0000 0100 */
//#define Event_Safety_Status_C1OT                 Event_Safety_Status_C1OT_Msk                  /*!< Cell 1 Overtemperature */
//#define Event_Safety_Status_C1UT_Pos             (9U)                                             // RESERVED !!!
//#define Event_Safety_Status_C1UT_Msk             (0x1U << Event_Safety_Status_C1UT_Pos)        /*!< 0x0000 0200 */
//#define Event_Safety_Status_C1UT                 Event_Safety_Status_C1UT_Msk                  /*!< Cell 1 Undertemperature */
//#define Event_Safety_Status_C2OT_Pos             (10U)                                            // RESERVED !!!
//#define Event_Safety_Status_C2OT_Msk             (0x1U << Event_Safety_Status_C2OT_Pos)        /*!< 0x0000 0400 */
//#define Event_Safety_Status_C2OT                 Event_Safety_Status_C2OT_Msk                  /*!< Cell 2 Overtemperature */
//#define Event_Safety_Status_C2UT_Pos             (11U)                                            // RESERVED !!!
//#define Event_Safety_Status_C2UT_Msk             (0x1U << Event_Safety_Status_C2UT_Pos)        /*!< 0x0000 0800 */
//#define Event_Safety_Status_C2UT                 Event_Safety_Status_C2UT_Msk                  /*!< Cell 2 Undertemperature */
//#define Event_Safety_Status_C3OT_Pos             (12U)                                            // RESERVED !!!
//#define Event_Safety_Status_C3OT_Msk             (0x1U << Event_Safety_Status_C3OT_Pos)        /*!< 0x0000 1000 */
//#define Event_Safety_Status_C3OT                 Event_Safety_Status_C3OT_Msk                  /*!< Cell 3 Overtemperature */
//#define Event_Safety_Status_C3UT_Pos             (13U)                                            // RESERVED !!!
//#define Event_Safety_Status_C3UT_Msk             (0x1U << Event_Safety_Status_C3UT_Pos)        /*!< 0x0000 2000 */
//#define Event_Safety_Status_C3UT                 Event_Safety_Status_C3UT_Msk                  /*!< Cell 3 Undertemperature */
//#define Event_Safety_Status_C4OT_Pos             (14U)                                            // RESERVED !!!
//#define Event_Safety_Status_C4OT_Msk             (0x1U << Event_Safety_Status_C4OT_Pos)        /*!< 0x0000 4000 */
//#define Event_Safety_Status_C4OT                 Event_Safety_Status_C4OT_Msk                  /*!< Cell 4 Overtemperature */
//#define Event_Safety_Status_C4UT_Pos             (15U)                                            // RESERVED !!!
//#define Event_Safety_Status_C4UT_Msk             (0x1U << Event_Safety_Status_C4UT_Pos)        /*!< 0x0000 8000 */
//#define Event_Safety_Status_C4UT                 Event_Safety_Status_C4UT_Msk                  /*!< Cell 4 Undertemperature */
//#define Event_Balancing1_Pos                     (16U)                               
//#define Event_Balancing1_Msk                     (0x1U << Event_Balancing1_Pos)                /*!< 0x0001 0000 */
//#define Event_Balancing1                         Event_Balancing1_Msk                          /*!< Balancing 1 Enable */
//#define Event_Balancing2_Pos                     (17U)                               
//#define Event_Balancing2_Msk                     (0x1U << Event_Balancing2_Pos)                /*!< 0x0002 0000 */
//#define Event_Balancing2                         Event_Balancing2_Msk                          /*!< Balancing 2 Enable */
//#define Event_Balancing3_Pos                     (18U)                               
//#define Event_Balancing3_Msk                     (0x1U << Event_Balancing3_Pos)                /*!< 0x0004 0000 */
//#define Event_Balancing3                         Event_Balancing3_Msk                          /*!< Balancing 3 Enable */
//#define Event_Balancing4_Pos                     (19U)                               
//#define Event_Balancing4_Msk                     (0x1U << Event_Balancing4_Pos)                /*!< 0x0008 0000 */
//#define Event_Balancing4                         Event_Balancing4_Msk                          /*!< Balancing 4 Enable */
//#define Event_Safety_Status_ROT_Pos              (20U)                               
//#define Event_Safety_Status_ROT_Msk              (0x1U << Event_Safety_Status_ROT_Pos)         /*!< 0x0010 0000 */
//#define Event_Safety_Status_ROT                  Event_Safety_Status_ROT_Msk                   /*!< Radiator Overtemperature */
//#define Event_Safety_Status_RUT_Pos              (21U)                               
//#define Event_Safety_Status_RUT_Msk              (0x1U << Event_Safety_Status_RUT_Pos)         /*!< 0x0020 0000 */
//#define Event_Safety_Status_RUT                  Event_Safety_Status_RUT_Msk                   /*!< Radiator Undertemperature */
//
///** 
//  * @brief MA measurements Structure definition
//  */
//typedef volatile struct Measurements
//{
//	uint16_t U_cell_1; /*!< Cell 1 voltage, mV */
//	uint16_t U_cell_2; /*!< Cell 2 voltage, mV */
//	uint16_t U_cell_3; /*!< Cell 3 voltage, mV */
//	uint16_t U_cell_4; /*!< Cell 4 voltage, mV */
//	int8_t   Temperature_cell_1; /*!< Cell 1 temperature, 'C */ // RESERVED !!!
//	int8_t   Temperature_cell_2; /*!< Cell 2 temperature, 'C */ // RESERVED !!!
//	int8_t   Temperature_cell_3; /*!< Cell 3 temperature, 'C */ // RESERVED !!!
//	int8_t   Temperature_cell_4; /*!< Cell 4 temperature, 'C */ // RESERVED !!!
//	int8_t   TemperatureRadiator; /*!< Radiator temperature, 'C */
//	uint32_t Event_Register; /*!< Event Register */
//} MeasurementsTypeDef;
//2222222222222222222222222222222222222222222222222222222222

#endif /* __memory_H */