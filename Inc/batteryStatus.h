#ifndef __batteryStatus_H
#define __batteryStatus_H

/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
/******************  Bit definition for Battery Register  *******************/
#define Battery_Status_FC_Pos			(0U)                               
#define Battery_Status_FC_Msk           (0x1U << Battery_Status_FC_Pos)       /*!< 0x0001 */
#define Battery_Status_FC               Battery_Status_FC_Msk                 /*!< Fully Charged */
#define Battery_Status_FD_Pos           (1U)                               
#define Battery_Status_FD_Msk           (0x1U << Battery_Status_FD_Pos)       /*!< 0x0002 */
#define Battery_Status_FD               Battery_Status_FD_Msk                 /*!< Fully Discharged */
#define Battery_Status_RCA_Pos          (2U)                               
#define Battery_Status_RCA_Msk          (0x1U << Battery_Status_RCA_Pos)      /*!< 0x0004 */
#define Battery_Status_RCA              Battery_Status_RCA_Msk                /*!< Remaining Capacity Alarm */
#define Battery_Status_TDA_Pos          (3U)                                         
#define Battery_Status_TDA_Msk          (0x1U << Battery_Status_TDA_Pos)      /*!< 0x0008 */
#define Battery_Status_TDA              Battery_Status_TDA_Msk                /*!< Terminate Discharge Alarm */
#define Battery_Status_OTA_Pos          (4U)                                         
#define Battery_Status_OTA_Msk          (0x1U << Battery_Status_OTA_Pos)      /*!< 0x0010 */
#define Battery_Status_OTA              Battery_Status_OTA_Msk                /*!< Over Temperature Alarm */
#define Battery_Status_TCA_Pos          (5U)                                         
#define Battery_Status_TCA_Msk          (0x1U << Battery_Status_TCA_Pos)      /*!< 0x0020 */
#define Battery_Status_TCA              Battery_Status_TCA_Msk                /*!< Terminate Charge Alarm */
#define Battery_Status_OCA_Pos          (6U)                                         
#define Battery_Status_OCA_Msk          (0x1U << Battery_Status_OCA_Pos)      /*!< 0x0040 */
#define Battery_Status_OCA              Battery_Status_OCA_Msk                /*!< Over Charged Alarm */
#define Battery_Status_PS_Pos           (7U)                                         
#define Battery_Status_PS_Msk           (0x1U << Battery_Status_PS_Pos)       /*!< 0x0080 */
#define Battery_Status_PS               Battery_Status_PS_Msk                 /*!< Password Set */

/* ###########################################################################*/
/* Prototipes ----------------------------------------------------------------*/
void updateBatteryRegister();
uint16_t getBatteryStatus();

#endif /* __batteryStatus_H */
