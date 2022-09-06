/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "safetyStatus.h"
#include "cell.h"
#include "memory.h"

/* ###########################################################################*/
/* Local variables -----------------------------------------------------------*/
uint16_t safetyAlert;
uint16_t safetyStatus;
uint8_t ROTTime;
uint8_t RUTTime;

/* ###########################################################################*/
/* Local Prototipes ----------------------------------------------------------*/
void updateSafetyAlert();
void updateSafetyStatus();
void calculateSafetyRegister(uint16_t *safetyRegister, uint16_t(*func)(const CellIndex));

/* ###########################################################################*/
/* Functions -----------------------------------------------------------------*/
void updateSafetyRegister()
{
	updateSafetyAlert();
	updateSafetyStatus();
}
void updateSafetyAlert()
{
	// SAFETY_STATUS_ROT
	if ((getRadiatorTemperature() > getROTThreshold()) && ((safetyAlert & Safety_Status_ROT_Msk) != Safety_Status_ROT))
	{
		safetyAlert |= Safety_Status_ROT_Msk;
		ROTTime = getROTTime();
	}
	if ((getRadiatorTemperature() < getROTRecovery()) && ((safetyAlert & Safety_Status_ROT_Msk) == Safety_Status_ROT))
	{
		safetyAlert &= ~Safety_Status_ROT_Msk;
		ROTTime = getROTTime();
	}
	// SAFETY_STATUS_RUT
	if ((getRadiatorTemperature() < getRUTThreshold()) && ((safetyAlert & Safety_Status_RUT_Msk) != Safety_Status_RUT))
	{
		safetyAlert |= Safety_Status_RUT_Msk;
		RUTTime = getRUTTime();
	}
	if ((getRadiatorTemperature() > getRUTRecovery()) && ((safetyAlert & Safety_Status_RUT_Msk) == Safety_Status_RUT))
	{
		safetyAlert &= ~Safety_Status_RUT_Msk;
		RUTTime = getRUTTime();
	}
	calculateSafetyRegister(&safetyAlert, &getCellAlert);
}

void updateSafetyStatus()
{
	updateRegisterStatus(&safetyStatus, safetyAlert, Safety_Status_ROT_Msk, ROTTime);
	updateRegisterStatus(&safetyStatus, safetyAlert, Safety_Status_RUT_Msk, RUTTime);
	calculateSafetyRegister(&safetyStatus, &getCellStatus);
}

/**
  * @brief  Update SafetyRegister
  *
  * @param  safetyRegister  Pointer to SafetyRegister.
  * @param  func			Pointer to function getCellAlert/getCellStatus.
  */
void calculateSafetyRegister(uint16_t *safetyRegister, uint16_t(*func)(const CellIndex))
{
	bool isCOVCset = false;
	bool isCOVTset = false;
	bool isCUVset = false;
	bool isCOTset = false;
	bool isCUTset = false;
	bool isUTCset = false;
	bool isUTDset = false;
	for(CellIndex index = Cell_1 ; index <= Cell_4 ; index++)
	{
		if ((func(index) & Cell_Status_COVC_Msk) == Cell_Status_COVC)
		{
			isCOVCset = true;
		}
		if ((func(index) & Cell_Status_COVT_Msk) == Cell_Status_COVT)
		{
			isCOVTset = true;
		}
		if ((func(index) & Cell_Status_CUV_Msk) == Cell_Status_CUV)
		{
			isCUVset = true;
		}
		if ((func(index) & Cell_Status_COT_Msk) == Cell_Status_COT)
		{
			isCOTset = true;
		}
		if ((func(index) & Cell_Status_CUT_Msk) == Cell_Status_CUT)
		{
			isCUTset = true;
		}
		if (((*safetyRegister & Safety_Status_RUT_Msk) == Safety_Status_RUT) || ((func(index) & Cell_Status_UTC_Msk) == Cell_Status_UTC))
		{
			isUTCset = true;
		}
		if (((*safetyRegister & Safety_Status_RUT_Msk) == Safety_Status_RUT) || ((func(index) & Cell_Status_UTD_Msk) == Cell_Status_UTD))
		{
			isUTDset = true;
		}
	}
	setStatus(safetyRegister, Safety_Status_COVC_Msk, isCOVCset);
	setStatus(safetyRegister, Safety_Status_COVT_Msk, isCOVTset);
	setStatus(safetyRegister, Safety_Status_CUV_Msk, isCUVset);
	setStatus(safetyRegister, Safety_Status_COT_Msk, isCOTset);
	setStatus(safetyRegister, Safety_Status_CUT_Msk, isCUTset);
	setStatus(safetyRegister, Safety_Status_UTC_Msk, isUTCset);
	setStatus(safetyRegister, Safety_Status_UTD_Msk, isUTDset);
}

void decrementTimerSafety()
{
	ROTTime = decrementTime(ROTTime);
	RUTTime = decrementTime(RUTTime);
}

uint16_t getSafetyAlert()
{
	return safetyAlert;
}

uint16_t getSafetyStatus()
{
	return safetyStatus;
}