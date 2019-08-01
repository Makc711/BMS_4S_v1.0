/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
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
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#ifndef __cplusplus
typedef enum 
{
	false = 0, 
	true  = !false
} bool;
#endif
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ADC_Temper_Pin GPIO_PIN_0
#define ADC_Temper_GPIO_Port GPIOA
#define Gate_Pin GPIO_PIN_1
#define Gate_GPIO_Port GPIOA
#define Balance4_Pin GPIO_PIN_2
#define Balance4_GPIO_Port GPIOA
#define Balance3_Pin GPIO_PIN_3
#define Balance3_GPIO_Port GPIOA
#define ADC_U1_Pin GPIO_PIN_4
#define ADC_U1_GPIO_Port GPIOA
#define ADC_U2_Pin GPIO_PIN_5
#define ADC_U2_GPIO_Port GPIOA
#define ADC_U3_Pin GPIO_PIN_6
#define ADC_U3_GPIO_Port GPIOA
#define ADC_U4_Pin GPIO_PIN_7
#define ADC_U4_GPIO_Port GPIOA
#define Balance2_Pin GPIO_PIN_0
#define Balance2_GPIO_Port GPIOB
#define Balance1_Pin GPIO_PIN_1
#define Balance1_GPIO_Port GPIOB
#define ChargeOn_Pin GPIO_PIN_8
#define ChargeOn_GPIO_Port GPIOA
#define Motor_Pin GPIO_PIN_15
#define Motor_GPIO_Port GPIOA
#define Led20_Pin GPIO_PIN_3
#define Led20_GPIO_Port GPIOB
#define Led40_Pin GPIO_PIN_4
#define Led40_GPIO_Port GPIOB
#define Led60_Pin GPIO_PIN_5
#define Led60_GPIO_Port GPIOB
#define Led80_Pin GPIO_PIN_6
#define Led80_GPIO_Port GPIOB
#define Led100_Pin GPIO_PIN_7
#define Led100_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
