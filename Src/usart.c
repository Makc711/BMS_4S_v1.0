/**
  ******************************************************************************
  * File Name          : USART.c
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "cell.h"
#include "memory.h"
#include "discreteDevices.h"
#include "statusRegisters.h"
#include "adc.h"
#include <string.h>

/* ###########################################################################*/
/* Local defines -------------------------------------------------------------*/
#define CHECKSUM_CONSTANT 44111

/* ###########################################################################*/
/* Local variables -----------------------------------------------------------*/
volatile bool isWaitingIncomingPackageSize = true;
uint8_t receivedByte = 0;
uint8_t command = 0;
uint8_t step = 0;
uint8_t receiveBuffer[RX_BUFFER_SIZE];
uint8_t transmitBuffer[TX_BUFFER_SIZE];
bool isDebugging = false;

/* ###########################################################################*/
/* Local prototypes ----------------------------------------------------------*/
void UART_TransmitCommand(const UART_Command command);
void UART_Receive(uint8_t *pData, const uint16_t size);
bool UART_SaveReceivedData();
bool calibrationU(const CellIndex index);
bool calibration(const ADCIndex index);
bool calibrationTemp();
bool updateSettings(const uint8_t size, void(*copyDataToSettingsStruct)(uint8_t*, const uint8_t));
void UART_WaitReceiveByte();
void debuggingDiscreteDevices(const uint8_t command);
void executeUARTCommand(const uint8_t command);

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart1_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel2;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel3;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmatx);
    HAL_DMA_DeInit(uartHandle->hdmarx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void UART_TransmitData(uint8_t *pData, const uint16_t size)
{
	memcpy(transmitBuffer, pData, size);
	/* If data is still being transmitted, wait here. */
	while (HAL_UART_Transmit_DMA(&huart1, transmitBuffer, size) != HAL_OK) {}
}

void UART_TransmitCommand(const UART_Command command)
{
	UART_TransmitData((uint8_t*)&command, sizeof(uint8_t));
}

void UART_Receive(uint8_t *pData, const uint16_t size)
{
	while (HAL_UART_Receive_DMA(&huart1, pData, size) != HAL_OK) {}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{
		if (command == 0)
		{
			command = receivedByte;
		}
		if (isDebugging == true)
		{
			debuggingDiscreteDevices(command);
		} else
		{
			executeUARTCommand(command);
		}
	}
}

void debuggingDiscreteDevices(const uint8_t command)
{
	switch (command) 
	{
	case UART_COMMAND_PC_DISABLE_DEBUGGING:
		isDebugging = false;
		waitNewCommand();
		break;
	case UART_COMMAND_PC_BALANCING_CELL1_ENABLE:
		balance(Cell_1, BalanceON);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_BALANCING_CELL2_ENABLE:
		balance(Cell_2, BalanceON);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_BALANCING_CELL3_ENABLE:
		balance(Cell_3, BalanceON);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_BALANCING_CELL4_ENABLE:
		balance(Cell_4, BalanceON);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_BALANCING_CELL1_DISABLE:
		balance(Cell_1, BalanceOFF);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_BALANCING_CELL2_DISABLE:
		balance(Cell_2, BalanceOFF);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_BALANCING_CELL3_DISABLE:
		balance(Cell_3, BalanceOFF);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_BALANCING_CELL4_DISABLE:
		balance(Cell_4, BalanceOFF);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_CONTACTOR_ENABLE:
		contactor(ON);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_CONTACTOR_DISABLE:
		contactor(OFF);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_CHARGER_ENABLE:
		charger(ON);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_CHARGER_DISABLE:
		charger(OFF);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_MOTOR_ENABLE:
		motor(ON);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_MOTOR_DISABLE:
		motor(OFF);
		waitNewCommand();
		break;
	case UART_COMMAND_PC_SEND_STATUS_REGISTERS:
		canSendStatusRegistersUART();
		waitNewCommand();
		break;
	default:
		waitNewCommand();
	}
}

void executeUARTCommand(const uint8_t command)
{
	switch (command) 
	{
	case UART_COMMAND_PC_ENABLE_DEBUGGING:
		isDebugging = true;
		waitNewCommand();
		break;
	case UART_COMMAND_PC_CALIBRATION_U1:
		if (calibration(ADC_U1) == true)
		{
			waitNewCommand();
		}
		break;
	case UART_COMMAND_PC_CALIBRATION_U2:
		if (calibration(ADC_U2) == true)
		{
			waitNewCommand();
		}
		break;
	case UART_COMMAND_PC_CALIBRATION_U3:
		if (calibration(ADC_U3) == true)
		{
			waitNewCommand();
		}
		break;
	case UART_COMMAND_PC_CALIBRATION_U4:
		if (calibration(ADC_U4) == true)
		{
			waitNewCommand();
		}
		break;
	case UART_COMMAND_PC_CALIBRATION_TEMP:
		if (calibration(ADC_Temper) == true)
		{
			waitNewCommand();
		}
		break;
	case UART_COMMAND_PC_SEND_STATUS_REGISTERS:
		canSendStatusRegistersUART();
		waitNewCommand();
		break;
	case UART_COMMAND_PC_SEND_SETTINGS_CHARGE:
		sendSettingsChargeUART();
		waitNewCommand();
		break;
	case UART_COMMAND_PC_SEND_SETTINGS_ALARM:
		sendSettingsAlarmUART();
		waitNewCommand();
		break;
	case UART_COMMAND_PC_UPDATE_SETTINGS_CHARGE:
		if (updateSettings(getSizeSettingsBMS(), &copyDataToSettingsBMS) == true)
		{
			waitNewCommand();
		}
		break;
	case UART_COMMAND_PC_UPDATE_SETTINGS_ALARM:
		if (updateSettings(getSizeSettingsAlarm(), &copyDataToSettingsAlarm) == true)
		{
			waitNewCommand();
		}
		break;
	case UART_COMMAND_PC_SAVE_SETTINGS:
		saveSettingsInFlash();
		waitNewCommand();
		break;
	case UART_COMMAND_PC_DEFAULT_CALIBRATION:
		setDefaultSettingsADC();
		waitNewCommand();
		break;
	case UART_COMMAND_PC_DEFAULT_CHARGE:
		setDefaultSettingsCharge();
		waitNewCommand();
		break;
	case UART_COMMAND_PC_DEFAULT_ALARM:
		setDefaultSettingsAlarm();
		waitNewCommand();
		break;
	default: 
		waitNewCommand();
	}
}

void waitNewCommand()
{
	command = 0;
	UART_WaitReceiveByte();
}

//bool calibrationU(const CellIndex index)
//{
//	bool finish = false;
//	switch (step) 
//	{
//	case 0:
//		UART_Receive(receiveBuffer, sizeof(uint16_t));
//		transmitBuffer[0] = UART_COMMAND_OK;
//		UART_TransmitData(transmitBuffer, sizeof(uint8_t));
//		step++;
//		break;
//	case 1:
//		UART_WaitReceiveByte();
//		UART_TransmitData(receiveBuffer, sizeof(uint16_t));
//		step++;
//		break;
//	case 2:
//		if (receivedByte == UART_COMMAND_OK)
//		{
//			const uint16_t calibrationU = receiveBuffer[0] << 8 | receiveBuffer[1];
//			const uint16_t ADC_U = getADCData(ADC_U1 + index);
//			uint32_t ADC_U_K = (uint64_t) calibrationU * DIGITS_AFTER_POIT_ADC_K / ADC_U;
//			setADC_U_K(index, ADC_U_K);
//			UART_TransmitData((uint8_t*)&ADC_U_K, sizeof(uint32_t));
//		}
//		step = 0;
//		finish = true;
//		break;
//	default:
//		step = 0;
//		finish = true;
//	}
//	return finish;
//}

bool calibration(const ADCIndex index)
{
	bool finish = false;
	switch (step) 
	{
	case 0:
		UART_Receive(receiveBuffer, (index == ADC_Temper) ? sizeof(int8_t) : sizeof(uint16_t));
		transmitBuffer[0] = UART_COMMAND_OK;
		UART_TransmitData(transmitBuffer, sizeof(uint8_t));
		step++;
		break;
	case 1:
		UART_WaitReceiveByte();
		UART_TransmitData(receiveBuffer, (index == ADC_Temper) ? sizeof(int8_t) : sizeof(uint16_t));
		step++;
		break;
	case 2:
		if (receivedByte == UART_COMMAND_OK)
		{
			const uint16_t calibration = (index == ADC_Temper) 
				? receiveBuffer[0]
				: receiveBuffer[0] << 8 | receiveBuffer[1];
			const uint16_t ADCData = getADCData(index);
			uint32_t ADC_K = (uint64_t) calibration * DIGITS_AFTER_POIT_ADC_K / ADCData;
			if (index == ADC_Temper)
			{
				setADC_TEMP_K(ADC_K);
			} else
			{
				setADC_U_K((CellIndex)(index - ADC_U1), ADC_K);
			}
			UART_TransmitData((uint8_t*)&ADC_K, sizeof(uint32_t));
		}
		step = 0;
		finish = true;
		break;
	default:
		step = 0;
		finish = true;
	}
	return finish;
}

//bool calibrationTemp()
//{
//	bool finish = false;
//	switch (step) 
//	{
//	case 0:
//		UART_Receive(receiveBuffer, sizeof(int8_t));
//		transmitBuffer[0] = UART_COMMAND_OK;
//		UART_TransmitData(transmitBuffer, sizeof(uint8_t));
//		step++;
//		break;
//	case 1:
//		UART_WaitReceiveByte();
//		UART_TransmitData(receiveBuffer, sizeof(int8_t));
//		step++;
//		break;
//	case 2:
//		if (receivedByte == UART_COMMAND_OK)
//		{
//			const int8_t calibrationTemp = receiveBuffer[0];
//			const uint16_t ADC_Temp = getADCData(ADC_Temper);
//			uint32_t ADC_TEMP_K = (uint64_t) calibrationTemp * DIGITS_AFTER_POIT_ADC_K / ADC_Temp;
//			setADC_TEMP_K(ADC_TEMP_K);
//			UART_TransmitData((uint8_t*)&ADC_TEMP_K, sizeof(uint32_t));
//		}
//		step = 0;
//		finish = true;
//		break;
//	default:
//		step = 0;
//		finish = true;
//	}
//	return finish;
//}

bool updateSettings(const uint8_t size, void(*copyDataToSettingsStruct)(uint8_t*, const uint8_t))
{
	bool finish = false;
	switch (step) 
	{
	case 0:
		UART_Receive(receiveBuffer, size);
		transmitBuffer[0] = UART_COMMAND_OK;
		UART_TransmitData(transmitBuffer, sizeof(uint8_t));
		step++;
		break;
	case 1:
		UART_WaitReceiveByte();
		const uint8_t checksum = calculateChecksum(receiveBuffer, size);
		UART_TransmitData((uint8_t*)&checksum, sizeof(uint8_t));
		step++;
		break;
	case 2:
		if (receivedByte == UART_COMMAND_OK)
		{
			copyDataToSettingsStruct(receiveBuffer, size);
		}
		step = 0;
		finish = true;
		break;
	default:
		step = 0;
		finish = true;
	}
	return finish;
}

uint8_t calculateChecksum(uint8_t *data, const uint16_t size)
{
	uint16_t checksum = 0;
	for (uint8_t i = 0; i < size; i++)
	{
		checksum += (uint16_t)(data[i] * CHECKSUM_CONSTANT * (i + 1));
	}
	return (uint8_t)checksum;
}

void UART_WaitReceiveByte()
{
	UART_Receive(&receivedByte, sizeof(receivedByte));
}

bool getDebuggingStatus()
{
	return isDebugging;
}

//bool UART_SaveReceivedData()
//{
//	const uint8_t addressOfSavingData = receiveBuffer[Index_Of_Command_byte] & ~UART_Command_State_Data;
//	return writeDataToSettingsArray(receiveBuffer + COMMAND_SIZE, receivedByte - COMMAND_SIZE, addressOfSavingData);
//}






//bool UART_TransmitData(uint8_t *data, const uint8_t size)
//{
//	bool isDataBufferedTX = false;
//	__disable_irq();
//	const uint16_t txCount = (uint16_t)countOfUnsentBytes + size;
//	if (txCount <= TX_BUFFER_SIZE)
//	{
//		for (int i = 0; i < size; i++)
//		{
//			transmitBuffer[txBufferTail] = data[i];
//			txBufferTail = (uint8_t)((txBufferTail + 1) & (TX_BUFFER_SIZE - 1));
//		}
//		countOfUnsentBytes = (uint8_t)txCount;
//		if (isUART_TxReady == true)
//		{
//			isUART_TxReady = false;
//			UART_TransmitNextPackage();
//		}
//		isDataBufferedTX = true;
//	}
//	__enable_irq();
//	return isDataBufferedTX;
//}
//
//void UART_TransmitNextPackage()
//{
//	uint8_t numberOfUnsentBytes;
//	if (countOfUnsentBytes <= PACKAGE_SIZE)
//	{
//		numberOfUnsentBytes = countOfUnsentBytes;
//	} else
//	{
//		numberOfUnsentBytes = PACKAGE_SIZE;
//	}
//	for (int i = 0; i < numberOfUnsentBytes; i++)
//	{
//		txBufferOfPackage[i] = transmitBuffer[txBufferHead];
//		txBufferHead = (uint8_t)((txBufferHead + 1) & (TX_BUFFER_SIZE - 1));
//	}
//	countOfUnsentBytes -= numberOfUnsentBytes;
//	UART_Transmit(txBufferOfPackage, numberOfUnsentBytes);
//}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
