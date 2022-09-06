#ifndef __cell_H
#define __cell_H

/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
#define NUMBER_OF_CELLS 4

/******************  Bit definition for Event Register  *******************/
#define Cell_Status_COVC_Pos            (0U)                               
#define Cell_Status_COVC_Msk            (0x1U << Cell_Status_COVC_Pos)       /*!< 0x0001 */
#define Cell_Status_COVC                Cell_Status_COVC_Msk                 /*!< Cell Overvoltage (Charger) */
#define Cell_Status_COVT_Pos            (1U)                               
#define Cell_Status_COVT_Msk            (0x1U << Cell_Status_COVT_Pos)       /*!< 0x0002 */
#define Cell_Status_COVT                Cell_Status_COVT_Msk                 /*!< Cell Overvoltage (Transistor) */
#define Cell_Status_CUV_Pos             (2U)                               
#define Cell_Status_CUV_Msk             (0x1U << Cell_Status_CUV_Pos)        /*!< 0x0004 */
#define Cell_Status_CUV                 Cell_Status_CUV_Msk                  /*!< Cell Undervoltage */
#define Cell_Status_COT_Pos             (3U)                                         
#define Cell_Status_COT_Msk             (0x1U << Cell_Status_COT_Pos)        /*!< 0x0008 */
#define Cell_Status_COT                 Cell_Status_COT_Msk                  /*!< Cell Overtemperature */  // RESERVED !!!
#define Cell_Status_CUT_Pos             (4U)                                         
#define Cell_Status_CUT_Msk             (0x1U << Cell_Status_CUT_Pos)        /*!< 0x0010 */
#define Cell_Status_CUT                 Cell_Status_CUT_Msk                  /*!< Cell Undertemperature */  // RESERVED !!!
#define Cell_Status_UTC_Pos             (5U)                                         
#define Cell_Status_UTC_Msk             (0x1U << Cell_Status_UTC_Pos)        /*!< 0x0020 */
#define Cell_Status_UTC                 Cell_Status_UTC_Msk                  /*!< Cell Undertemperature (Charge) */  // RESERVED !!!
#define Cell_Status_UTD_Pos             (6U)                                         
#define Cell_Status_UTD_Msk             (0x1U << Cell_Status_UTD_Pos)        /*!< 0x0040 */
#define Cell_Status_UTD                 Cell_Status_UTD_Msk                  /*!< Cell Undertemperature (Discharge) */  // RESERVED !!!
#define Cell_Status_CTMAX_Pos           (7U)                               
#define Cell_Status_CTMAX_Msk           (0x1U << Cell_Status_CTMAX_Pos)      /*!< 0x0080 */
#define Cell_Status_CTMAX               Cell_Status_CTMAX_Msk                /*!< Maximum battery temperature */  // RESERVED !!!
#define Cell_Status_CTMIN_Pos           (8U)                               
#define Cell_Status_CTMIN_Msk           (0x1U << Cell_Status_CTMIN_Pos)      /*!< 0x0100 */
#define Cell_Status_CTMIN               Cell_Status_CTMIN_Msk                /*!< Minimum battery temperature */  // RESERVED !!!
#define Cell_Status_CVMAX_Pos           (9U)                               
#define Cell_Status_CVMAX_Msk           (0x1U << Cell_Status_CVMAX_Pos)      /*!< 0x0200 */
#define Cell_Status_CVMAX               Cell_Status_CVMAX_Msk                /*!< Maximum battery voltage */
#define Cell_Status_CVMIN_Pos           (10U)                               
#define Cell_Status_CVMIN_Msk           (0x1U << Cell_Status_CVMIN_Pos)      /*!< 0x0400 */
#define Cell_Status_CVMIN               Cell_Status_CVMIN_Msk                /*!< Minimum battery voltage */
#define Cell_Status_Balancing_Pos       (11U)                               
#define Cell_Status_Balancing_Msk       (0x1U << Cell_Status_Balancing_Pos)  /*!< 0x0800 */
#define Cell_Status_Balancing           Cell_Status_Balancing_Msk            /*!< Balancing Enable */

typedef enum CellIndex
{
	Cell_1 = 0,
	Cell_2 = 1,
	Cell_3 = 2,
	Cell_4 = 3
} CellIndex;

typedef enum
{
	BalanceOFF = GPIO_PIN_RESET,
	BalanceON  = GPIO_PIN_SET
} BalanceState;

/* ###########################################################################*/
/* Prototipes ----------------------------------------------------------------*/
void updateADCMeasurements();
uint16_t getADCVoltage(const CellIndex index);
uint16_t getCellVoltage(const CellIndex index);
int8_t getCellTemperature(const CellIndex index);
uint16_t getCellStatus(const CellIndex index);
uint16_t getCellAlert(const CellIndex index);
int8_t getRadiatorTemperature();
uint16_t getMVoltage(const uint16_t mask);
int8_t getMTemperature(const uint16_t mask);
void updateBalanceStatus(const CellIndex index, const BalanceState state);
void decrementTimers();
uint8_t decrementTime(const uint8_t time);
void balance(const CellIndex index, const BalanceState state);
bool isAllCellsBalanced();
bool isCellBalanced();
void updateRegisterStatus(uint16_t *registerStatus, const uint16_t registerAlert, const uint16_t mask, const uint8_t timer);
void setStatus(uint16_t *registerStatus, const uint16_t mask, const bool state);

#endif /* __cell_H */