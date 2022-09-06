/* ###########################################################################*/
/* Includes ------------------------------------------------------------------*/
#include "chargeStatus.h"
#include "cell.h"
#include "memory.h"
#include "safetyStatus.h"
#include "discreteDevices.h"

/* ###########################################################################*/
/* Local variables -----------------------------------------------------------*/
uint16_t chargeAlert;
uint16_t chargeStatus;
uint8_t stateOfCharge = 0;
uint8_t ledBlinkTime = 0;
uint8_t CCTime;
uint8_t DCTime;
uint8_t DOC20Time;
uint8_t DOC40Time;
uint8_t DOC60Time;
uint8_t DOC80Time;
uint8_t DOC100Time;

/* ###########################################################################*/
/* Local Prototipes ----------------------------------------------------------*/
void updateStateOfCharge();
void updateSOCLedBlink();
void updateSOCLed();
void updateChargeAlert();
void updateChargeStatus();

/* ###########################################################################*/
/* Functions -----------------------------------------------------------------*/
void updateChargeRegister()
{
	updateStateOfCharge();
	updateChargeAlert();
	updateChargeStatus();
}

void updateStateOfCharge()
{
	stateOfCharge = SOC0;
	if ((chargeStatus & Charge_Status_SOC20_Msk) == Charge_Status_SOC20)
	{
		stateOfCharge = SOC20;
		if ((chargeStatus & Charge_Status_SOC40_Msk) == Charge_Status_SOC40)
		{
			stateOfCharge = SOC40;
			if ((chargeStatus & Charge_Status_SOC60_Msk) == Charge_Status_SOC60)
			{
				stateOfCharge = SOC60;
				if ((chargeStatus & Charge_Status_SOC80_Msk) == Charge_Status_SOC80)
				{
					stateOfCharge = SOC80;
					if ((chargeStatus & Charge_Status_SOC100_Msk) == Charge_Status_SOC100)
					{
						stateOfCharge = SOC100;
					}
				}
			}
		}
	}
	if ((chargeStatus & Charge_Status_RCA_Msk) == Charge_Status_RCA)
	{
		updateSOCLedBlink();
	} else
	{
		updateSOCLed();
	}
}

void updateSOCLedBlink()
{
	if (ledBlinkTime == 0)
	{
		ledBlinkTime = LED_BLINK_TIME;
		switch (stateOfCharge)
		{
		case SOC20:
			led20Toggle();
			led40(OFF);
			led60(OFF);
			led80(OFF);
			led100(OFF);
			break;
		case SOC40:
			led20Toggle();
			led40Toggle();
			led60(OFF);
			led80(OFF);
			led100(OFF);
			break;
		case SOC60:
			led20Toggle();
			led40Toggle();
			led60Toggle();
			led80(OFF);
			led100(OFF);
			break;
		case SOC80:
			led20Toggle();
			led40Toggle();
			led60Toggle();
			led80Toggle();
			led100(OFF);
			break;
		case SOC100:
			led20Toggle();
			led40Toggle();
			led60Toggle();
			led80Toggle();
			led100Toggle();
			break;
		default:
			led20(OFF);
			led40(OFF);
			led60(OFF);
			led80(OFF);
			led100(OFF);
			break;
		}
	}
}

void updateSOCLed()
{
	switch (stateOfCharge)
	{
	case SOC20:
		led20(ON);
		led40(OFF);
		led60(OFF);
		led80(OFF);
		led100(OFF);
		break;
	case SOC40:
		led20(ON);
		led40(ON);
		led60(OFF);
		led80(OFF);
		led100(OFF);
		break;
	case SOC60:
		led20(ON);
		led40(ON);
		led60(ON);
		led80(OFF);
		led100(OFF);
		break;
	case SOC80:
		led20(ON);
		led40(ON);
		led60(ON);
		led80(ON);
		led100(OFF);
		break;
	case SOC100:
		led20(ON);
		led40(ON);
		led60(ON);
		led80(ON);
		led100(ON);
		break;
	default:
		led20(OFF);
		led40(OFF);
		led60(OFF);
		led80(OFF);
		led100(OFF);
		break;
	}
}

void updateChargeAlert()
{
	// CHARGE_STATUS_CC
	if(((getMVoltage(Cell_Status_CVMIN_Msk) > getChargeVoltageThreshold()) && ((chargeAlert & Charge_Status_CC_Msk) != Charge_Status_CC)) || (isAllCellsBalanced() == true))
	{
		chargeAlert |= Charge_Status_CC_Msk;
		CCTime = getChargeCompletionTime();
	}
	if ((getMVoltage(Cell_Status_CVMIN_Msk) < getChargeVoltageRecovery()) && ((chargeAlert & Charge_Status_CC_Msk) == Charge_Status_CC))
	{
		chargeAlert &= ~Charge_Status_CC_Msk;
		CCTime = getChargeCompletionTime();
	}
	// CHARGE_STATUS_DC
	if (((getSafetyAlert() & Safety_Status_CUV_Msk) == Safety_Status_CUV) && ((chargeAlert & Charge_Status_DC_Msk) != Charge_Status_DC))
	{
		chargeAlert |= Charge_Status_DC_Msk;
		DCTime = getCUVTime();
	}
	if ((getMVoltage(Cell_Status_CVMIN_Msk) > getDischargeVoltageRecovery()) && ((chargeAlert & Charge_Status_DC_Msk) == Charge_Status_DC))
	{
		chargeAlert &= ~Charge_Status_DC_Msk;
		DCTime = getDischargeCompletionTime();
	}
	// CHARGE_STATUS_RCA
	chargeAlert = (stateOfCharge < getRemainingCapacityAlarmPercent())
		? chargeAlert | Charge_Status_RCA_Msk
		: chargeAlert & ~Charge_Status_RCA_Msk;
	// CHARGE_STATUS_BAL
	chargeAlert = (isCellBalanced() == true)
		? chargeAlert | Charge_Status_BAL_Msk
		: chargeAlert & ~Charge_Status_BAL_Msk;
	// CHARGE_STATUS_DOC20
	if ((getMVoltage(Cell_Status_CVMIN_Msk) > getCV20Threshold()) && ((chargeAlert & Charge_Status_SOC20_Msk) != Charge_Status_SOC20))
	{
		chargeAlert |= Charge_Status_SOC20_Msk;
		DOC20Time = getCV20Time();
	}
	if (((getMVoltage(Cell_Status_CVMIN_Msk) < getCV20Recovery()) && ((chargeAlert & Charge_Status_SOC20_Msk) == Charge_Status_SOC20))  || ((chargeStatus & Charge_Status_DC_Msk) == Charge_Status_DC))
	{
		chargeAlert &= ~Charge_Status_SOC20_Msk;
		DOC20Time = ((chargeStatus & Charge_Status_DC_Msk) == Charge_Status_DC) 
			? 0 
			: getCV20Time();
	}
	// CHARGE_STATUS_DOC40
	if ((getMVoltage(Cell_Status_CVMIN_Msk) > getCV40Threshold()) && ((chargeAlert & Charge_Status_SOC40_Msk) != Charge_Status_SOC40))
	{
		chargeAlert |= Charge_Status_SOC40_Msk;
		DOC40Time = getCV40Time();
	}
	if (((getMVoltage(Cell_Status_CVMIN_Msk) < getCV40Recovery()) && ((chargeAlert & Charge_Status_SOC40_Msk) == Charge_Status_SOC40)) || ((chargeStatus & Charge_Status_SOC20_Msk) != Charge_Status_SOC20))
	{
		chargeAlert &= ~Charge_Status_SOC40_Msk;
		DOC40Time = ((chargeStatus & Charge_Status_SOC20_Msk) != Charge_Status_SOC20) 
					? 0 
					: getCV40Time();
	}
	// CHARGE_STATUS_DOC60
	if ((getMVoltage(Cell_Status_CVMIN_Msk) > getCV60Threshold()) && ((chargeAlert & Charge_Status_SOC60_Msk) != Charge_Status_SOC60))
	{
		chargeAlert |= Charge_Status_SOC60_Msk;
		DOC60Time = getCV60Time();
	}
	if (((getMVoltage(Cell_Status_CVMIN_Msk) < getCV60Recovery()) && ((chargeAlert & Charge_Status_SOC60_Msk) == Charge_Status_SOC60)) || ((chargeStatus & Charge_Status_SOC40_Msk) != Charge_Status_SOC40))
	{
		chargeAlert &= ~Charge_Status_SOC60_Msk;
		DOC60Time = ((chargeStatus & Charge_Status_SOC40_Msk) != Charge_Status_SOC40) 
					? 0 
					: getCV60Time();
	}
	// CHARGE_STATUS_DOC80
	if ((getMVoltage(Cell_Status_CVMIN_Msk) > getCV80Threshold()) && ((chargeAlert & Charge_Status_SOC80_Msk) != Charge_Status_SOC80))
	{
		chargeAlert |= Charge_Status_SOC80_Msk;
		DOC80Time = getCV80Time();
	}
	if (((getMVoltage(Cell_Status_CVMIN_Msk) < getCV80Recovery()) && ((chargeAlert & Charge_Status_SOC80_Msk) == Charge_Status_SOC80)) || ((chargeStatus & Charge_Status_SOC60_Msk) != Charge_Status_SOC60))
	{
		chargeAlert &= ~Charge_Status_SOC80_Msk;
		DOC80Time = ((chargeStatus & Charge_Status_SOC60_Msk) != Charge_Status_SOC60) 
					? 0 
					: getCV80Time();
	}
	// CHARGE_STATUS_DOC100
	if(((getMVoltage(Cell_Status_CVMIN_Msk) > getCV100Threshold()) && ((chargeAlert & Charge_Status_SOC100_Msk) != Charge_Status_SOC100)) || ((chargeStatus & Charge_Status_CC_Msk) == Charge_Status_CC))
	{
		chargeAlert |= Charge_Status_SOC100_Msk;
		DOC100Time = ((chargeStatus & Charge_Status_CC_Msk) == Charge_Status_CC) 
					 ? 0 
					 : getCV100Time();
	}
	if (((getMVoltage(Cell_Status_CVMIN_Msk) < getCV100Recovery()) && ((chargeAlert & Charge_Status_SOC100_Msk) == Charge_Status_SOC100)) || ((chargeStatus & Charge_Status_SOC80_Msk) != Charge_Status_SOC80))
	{
		chargeAlert &= ~Charge_Status_SOC100_Msk;
		DOC100Time = ((chargeStatus & Charge_Status_SOC80_Msk) != Charge_Status_SOC80) 
					 ? 0 
					 : getCV100Time();
	}
}

void updateChargeStatus()
{
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_CC_Msk, CCTime);
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_DC_Msk, DCTime);
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_RCA_Msk, 0);
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_BAL_Msk, 0);
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_SOC20_Msk, DOC20Time);
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_SOC40_Msk, DOC40Time);
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_SOC60_Msk, DOC60Time);
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_SOC80_Msk, DOC80Time);
	updateRegisterStatus(&chargeStatus, chargeAlert, Charge_Status_SOC100_Msk, DOC100Time);
}

void decrementTimerCharge()
{
	CCTime = decrementTime(CCTime);
	DCTime = decrementTime(DCTime);
	DOC20Time = decrementTime(DOC20Time);
	DOC40Time = decrementTime(DOC40Time);
	DOC60Time = decrementTime(DOC60Time);
	DOC80Time = decrementTime(DOC80Time);
	DOC100Time = decrementTime(DOC100Time);
	ledBlinkTime = decrementTime(ledBlinkTime);
}

uint16_t getChargeAlert()
{
	return chargeAlert;
}

uint16_t getChargeStatus()
{
	return chargeStatus;
}

uint8_t getStateOfCharge()
{
	return stateOfCharge;
}