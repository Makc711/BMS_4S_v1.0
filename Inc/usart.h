/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
/* ###########################################################################*/
/* Defines -------------------------------------------------------------*/
#define TX_BUFFER_SIZE   52 // sizeof(statusRegisters)=52
#define RX_BUFFER_SIZE   40 // sizeof(settingsCharge)=40, sizeof(settingsAlarm)=36

#define max(a, b) ((a) > (b) ? (a) : (b))
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
/********************  UART command codes  *********************/
typedef enum
{ // Commands: 0x80 - 0xEF
	UART_COMMAND_PC_ENABLE_DEBUGGING         = 0x80, /*!< Command from PC. Enable debugging discrete devices. */
	UART_COMMAND_PC_DISABLE_DEBUGGING        = 0x81, /*!< Command from PC. Disable debugging discrete devices. */
	UART_COMMAND_PC_BALANCING_CELL1_ENABLE   = 0x82, /*!< Command from PC. Enable balancing cell 1. */
	UART_COMMAND_PC_BALANCING_CELL2_ENABLE   = 0x83, /*!< Command from PC. Enable balancing cell 2. */
	UART_COMMAND_PC_BALANCING_CELL3_ENABLE   = 0x84, /*!< Command from PC. Enable balancing cell 3. */
	UART_COMMAND_PC_BALANCING_CELL4_ENABLE   = 0x85, /*!< Command from PC. Enable balancing cell 4. */
	UART_COMMAND_PC_BALANCING_CELL1_DISABLE  = 0x86, /*!< Command from PC. Disable balancing cell 1. */
	UART_COMMAND_PC_BALANCING_CELL2_DISABLE  = 0x87, /*!< Command from PC. Disable balancing cell 2. */
	UART_COMMAND_PC_BALANCING_CELL3_DISABLE  = 0x88, /*!< Command from PC. Disable balancing cell 3. */
	UART_COMMAND_PC_BALANCING_CELL4_DISABLE  = 0x89, /*!< Command from PC. Disable balancing cell 4. */
	UART_COMMAND_PC_CONTACTOR_ENABLE         = 0x8A, /*!< Command from PC. Enable contactor. */
	UART_COMMAND_PC_CONTACTOR_DISABLE        = 0x8B, /*!< Command from PC. Disable contactor. */
	UART_COMMAND_PC_CHARGER_ENABLE           = 0x8C, /*!< Command from PC. Enable charger. */
	UART_COMMAND_PC_CHARGER_DISABLE          = 0x8D, /*!< Command from PC. Disable charger. */
	UART_COMMAND_PC_MOTOR_ENABLE             = 0x8E, /*!< Command from PC. Enable motor. */
	UART_COMMAND_PC_MOTOR_DISABLE            = 0x8F, /*!< Command from PC. Disable motor. */

	UART_COMMAND_PC_CALIBRATION_U1           = 0xE1, /*!< Command from PC. Calibration U1. */
	UART_COMMAND_PC_CALIBRATION_U2           = 0xE2, /*!< Command from PC. Calibration U2. */
	UART_COMMAND_PC_CALIBRATION_U3           = 0xE3, /*!< Command from PC. Calibration U3. */
	UART_COMMAND_PC_CALIBRATION_U4           = 0xE4, /*!< Command from PC. Calibration U4. */
	UART_COMMAND_PC_CALIBRATION_TEMP         = 0xE5, /*!< Command from PC. Calibration temperature. */
	UART_COMMAND_PC_SEND_STATUS_REGISTERS    = 0xE6, /*!< Command from PC. Send the status registers struct. */
	UART_COMMAND_PC_SEND_SETTINGS_CHARGE     = 0xE7, /*!< Command from PC. Send settings charge. */
	UART_COMMAND_PC_SEND_SETTINGS_ALARM      = 0xE8, /*!< Command from PC. Send settings alarm. */
	UART_COMMAND_PC_UPDATE_SETTINGS_CHARGE   = 0xE9, /*!< Command from PC. Update settings BMS. */
	UART_COMMAND_PC_UPDATE_SETTINGS_ALARM    = 0xEA, /*!< Command from PC. Update settings alarm. */
	UART_COMMAND_PC_SAVE_SETTINGS            = 0xEB, /*!< Command from PC. Save settings to flash. */
	UART_COMMAND_PC_DEFAULT_CALIBRATION      = 0xEC, /*!< Command from PC. Set default calibration settings. */
	UART_COMMAND_PC_DEFAULT_CHARGE           = 0xED, /*!< Command from PC. Set default charge settings. */
	UART_COMMAND_PC_DEFAULT_ALARM            = 0xEE, /*!< Command from PC. Set default alarm settings. */

	UART_COMMAND_OK                          = 0xFA
} UART_Command;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void waitNewCommand();
void UART_TransmitData(uint8_t *pData, const uint16_t size);
bool getDebuggingStatus();
uint8_t calculateChecksum(uint8_t *data, const uint16_t size);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
