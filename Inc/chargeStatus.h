#ifndef __chargeStatus_H
#define __chargeStatus_H

/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* ###########################################################################*/
/* Defines -------------------------------------------------------------------*/
/******************  Bit definition for Charge Register  *******************/
#define Charge_Status_CC_Pos            (0U)                               
#define Charge_Status_CC_Msk            (0x1U << Charge_Status_CC_Pos)       /*!< 0x0001 */
#define Charge_Status_CC                Charge_Status_CC_Msk                 /*!< Charge Complete */
#define Charge_Status_DC_Pos            (1U)                               
#define Charge_Status_DC_Msk            (0x1U << Charge_Status_DC_Pos)       /*!< 0x0002 */
#define Charge_Status_DC                Charge_Status_DC_Msk                 /*!< Discharge Complete */
#define Charge_Status_RCA_Pos           (2U)                               
#define Charge_Status_RCA_Msk           (0x1U << Charge_Status_RCA_Pos)      /*!< 0x0004 */
#define Charge_Status_RCA               Charge_Status_RCA_Msk                /*!< Remaining Capacity Alarm */
#define Charge_Status_BAL_Pos           (3U)                                         
#define Charge_Status_BAL_Msk           (0x1U << Charge_Status_BAL_Pos)      /*!< 0x0008 */
#define Charge_Status_BAL               Charge_Status_BAL_Msk                /*!< Balance */
#define Charge_Status_SOC20_Pos         (4U)                                         
#define Charge_Status_SOC20_Msk         (0x1U << Charge_Status_SOC20_Pos)    /*!< 0x0010 */
#define Charge_Status_SOC20             Charge_Status_SOC20_Msk              /*!< State of Charge 20% */
#define Charge_Status_SOC40_Pos         (5U)                                         
#define Charge_Status_SOC40_Msk         (0x1U << Charge_Status_SOC40_Pos)    /*!< 0x0020 */
#define Charge_Status_SOC40             Charge_Status_SOC40_Msk              /*!< State of Charge 40% */
#define Charge_Status_SOC60_Pos         (6U)                                         
#define Charge_Status_SOC60_Msk         (0x1U << Charge_Status_SOC60_Pos)    /*!< 0x0040 */
#define Charge_Status_SOC60             Charge_Status_SOC60_Msk              /*!< State of Charge 60% */
#define Charge_Status_SOC80_Pos         (7U)                                         
#define Charge_Status_SOC80_Msk         (0x1U << Charge_Status_SOC80_Pos)    /*!< 0x0080 */
#define Charge_Status_SOC80             Charge_Status_SOC80_Msk              /*!< State of Charge 80% */
#define Charge_Status_SOC100_Pos        (8U)                                         
#define Charge_Status_SOC100_Msk        (0x1U << Charge_Status_SOC100_Pos)   /*!< 0x0100 */
#define Charge_Status_SOC100            Charge_Status_SOC100_Msk             /*!< State of Charge 100% */

#define SOC0   0
#define SOC20  20
#define SOC40  40
#define SOC60  60
#define SOC80  80
#define SOC100 100

#define LED_BLINK_TIME 1

/* ###########################################################################*/
/* Prototipes ----------------------------------------------------------------*/
void updateChargeRegister();
void decrementTimerCharge();
uint16_t getChargeAlert();
uint16_t getChargeStatus();
uint8_t getStateOfCharge();

#endif /* __chargeStatus_H */