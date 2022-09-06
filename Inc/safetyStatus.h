#ifndef __safetyStatus_H
#define __safetyStatus_H

/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
/******************  Bit definition for Safety Register  *******************/
#define Safety_Status_COVC_Pos            Cell_Status_COVC_Pos                              
#define Safety_Status_COVC_Msk            Cell_Status_COVC_Msk                   /*!< 0x0001 */
#define Safety_Status_COVC                Cell_Status_COVC                       /*!< Cell Overvoltage (Charger) */
#define Safety_Status_COVT_Pos            Cell_Status_COVT_Pos                               
#define Safety_Status_COVT_Msk            Cell_Status_COVT_Msk                   /*!< 0x0002 */
#define Safety_Status_COVT                Cell_Status_COVT                       /*!< Cell Overvoltage (Transistor) */
#define Safety_Status_CUV_Pos             Cell_Status_CUV_Pos                               
#define Safety_Status_CUV_Msk             Cell_Status_CUV_Msk                    /*!< 0x0004 */
#define Safety_Status_CUV                 Cell_Status_CUV                        /*!< Cell Undervoltage */
#define Safety_Status_COT_Pos             Cell_Status_COT_Pos                                         
#define Safety_Status_COT_Msk             Cell_Status_COT_Msk                    /*!< 0x0008 */
#define Safety_Status_COT                 Cell_Status_COT                        /*!< Cell Overtemperature */  // RESERVED !!!
#define Safety_Status_CUT_Pos             Cell_Status_CUT_Pos                                         
#define Safety_Status_CUT_Msk             Cell_Status_CUT_Msk                    /*!< 0x0010 */
#define Safety_Status_CUT                 Cell_Status_CUT                        /*!< Cell Undertemperature */  // RESERVED !!!
#define Safety_Status_UTC_Pos             Cell_Status_UTC_Pos                                        
#define Safety_Status_UTC_Msk             Cell_Status_UTC_Msk                    /*!< 0x0020 */
#define Safety_Status_UTC                 Cell_Status_UTC                        /*!< Undertemperature (Charge) */
#define Safety_Status_UTD_Pos             Cell_Status_UTD_Pos                                         
#define Safety_Status_UTD_Msk             Cell_Status_UTD_Msk                    /*!< 0x0040 */
#define Safety_Status_UTD                 Cell_Status_UTD                        /*!< Undertemperature (Discharge) */
#define Safety_Status_ROT_Pos             (7U)                                         
#define Safety_Status_ROT_Msk             (0x1U << Safety_Status_ROT_Pos)        /*!< 0x0080 */
#define Safety_Status_ROT                 Safety_Status_ROT_Msk                  /*!< Radiator Overtemperature */
#define Safety_Status_RUT_Pos             (8U)                                         
#define Safety_Status_RUT_Msk             (0x1U << Safety_Status_RUT_Pos)        /*!< 0x0100 */
#define Safety_Status_RUT                 Safety_Status_RUT_Msk                  /*!< Radiator Undertemperature */

/* ###########################################################################*/
/* Prototipes ----------------------------------------------------------------*/
void updateSafetyRegister();
void decrementTimerSafety();
uint16_t getSafetyAlert();
uint16_t getSafetyStatus();

#endif /* __safetyStatus_H */