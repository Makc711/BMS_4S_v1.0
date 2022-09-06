/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "batteryStatus.h"
#include "cell.h"
#include "chargeStatus.h"
#include "safetyStatus.h"
#include "discreteDevices.h"
#include "usart.h"

/* ###########################################################################*/
/* Local variables -----------------------------------------------------------*/
uint16_t batteryStatus;

/* ###########################################################################*/
/* Local Prototipes ----------------------------------------------------------*/
void updateDiscreteContCharg();

/* ###########################################################################*/
/* Functions -----------------------------------------------------------------*/
void updateBatteryRegister()
{
	setStatus(&batteryStatus, Battery_Status_FC_Msk, (getChargeStatus() & Charge_Status_CC_Msk) == Charge_Status_CC);
	setStatus(&batteryStatus, Battery_Status_FD_Msk, (getChargeStatus() & Charge_Status_DC_Msk) == Charge_Status_DC);
	setStatus(&batteryStatus, Battery_Status_RCA_Msk, (getChargeStatus() & Charge_Status_RCA_Msk) == Charge_Status_RCA);
	setStatus(&batteryStatus, Battery_Status_TDA_Msk, ((getSafetyStatus() & Safety_Status_CUV_Msk) == Safety_Status_CUV) 
													|| ((getSafetyStatus() & Safety_Status_COVT_Msk) == Safety_Status_COVT)
													|| ((getSafetyStatus() & Safety_Status_COT_Msk) == Safety_Status_COT)
													|| ((getSafetyStatus() & Safety_Status_ROT_Msk) == Safety_Status_ROT)
													|| ((getSafetyStatus() & Safety_Status_UTD_Msk) == Safety_Status_UTD));
	setStatus(&batteryStatus, Battery_Status_OTA_Msk, ((getSafetyStatus() & Safety_Status_COT_Msk) == Safety_Status_COT) 
													|| ((getSafetyStatus() & Safety_Status_ROT_Msk) == Safety_Status_ROT));
	setStatus(&batteryStatus, Battery_Status_TCA_Msk, ((getSafetyStatus() & Safety_Status_COVC_Msk) == Safety_Status_COVC) 
													|| ((getSafetyStatus() & Safety_Status_COT_Msk) == Safety_Status_COT)
													|| ((getSafetyStatus() & Safety_Status_ROT_Msk) == Safety_Status_ROT)
													|| ((getSafetyStatus() & Safety_Status_UTC_Msk) == Safety_Status_UTC));
	setStatus(&batteryStatus, Battery_Status_OCA_Msk, (getSafetyStatus() & Safety_Status_COVT_Msk) == Safety_Status_COVT);
	if (getDebuggingStatus() == false)
	{
		updateDiscreteContCharg();
	}
}

void updateDiscreteContCharg()
{
	contactor(((batteryStatus & Battery_Status_TDA_Msk) == Battery_Status_TDA) ? OFF : ON);
	charger(((batteryStatus & Battery_Status_TCA_Msk) == Battery_Status_TCA) ? OFF : ON);
}

uint16_t getBatteryStatus()
{
	return batteryStatus;
}