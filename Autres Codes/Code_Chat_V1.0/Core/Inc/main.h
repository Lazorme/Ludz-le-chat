/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g0xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define USART4_RX_LIDAR_Pin GPIO_PIN_11
#define USART4_RX_LIDAR_GPIO_Port GPIOC
#define PWM_MOT_LIDAR_Pin GPIO_PIN_12
#define PWM_MOT_LIDAR_GPIO_Port GPIOC
#define Bouton1_Pin GPIO_PIN_0
#define Bouton1_GPIO_Port GPIOC
#define Bouton1_EXTI_IRQn EXTI0_1_IRQn
#define PWM_MOT2_PH1_Pin GPIO_PIN_1
#define PWM_MOT2_PH1_GPIO_Port GPIOC
#define PWM_MOT2_PH2_Pin GPIO_PIN_2
#define PWM_MOT2_PH2_GPIO_Port GPIOC
#define Bouton2_Pin GPIO_PIN_3
#define Bouton2_GPIO_Port GPIOC
#define Bouton2_EXTI_IRQn EXTI2_3_IRQn
#define ADC_Shunt1_Pin GPIO_PIN_0
#define ADC_Shunt1_GPIO_Port GPIOA
#define ADC_Shunt2_Pin GPIO_PIN_1
#define ADC_Shunt2_GPIO_Port GPIOA
#define Bouton3_Pin GPIO_PIN_4
#define Bouton3_GPIO_Port GPIOA
#define Bouton3_EXTI_IRQn EXTI4_15_IRQn
#define PWM_MOT1_PH1_Pin GPIO_PIN_6
#define PWM_MOT1_PH1_GPIO_Port GPIOA
#define PWM_MOT1_PH2_Pin GPIO_PIN_7
#define PWM_MOT1_PH2_GPIO_Port GPIOA
#define USART1_TX_Debug_Pin GPIO_PIN_4
#define USART1_TX_Debug_GPIO_Port GPIOC
#define USART1_RX_Debug_Pin GPIO_PIN_5
#define USART1_RX_Debug_GPIO_Port GPIOC
#define Contact1_Pin GPIO_PIN_13
#define Contact1_GPIO_Port GPIOB
#define Contact1_EXTI_IRQn EXTI4_15_IRQn
#define Contact2_Pin GPIO_PIN_14
#define Contact2_GPIO_Port GPIOB
#define Contact2_EXTI_IRQn EXTI4_15_IRQn
#define Contact3_Pin GPIO_PIN_15
#define Contact3_GPIO_Port GPIOB
#define Contact3_EXTI_IRQn EXTI4_15_IRQn
#define CODEUR1_PH1_Pin GPIO_PIN_8
#define CODEUR1_PH1_GPIO_Port GPIOA
#define CODEUR1_PH2_Pin GPIO_PIN_9
#define CODEUR1_PH2_GPIO_Port GPIOA
#define CODEUR2_PH1_Pin GPIO_PIN_6
#define CODEUR2_PH1_GPIO_Port GPIOC
#define CODEUR2_PH2_Pin GPIO_PIN_7
#define CODEUR2_PH2_GPIO_Port GPIOC
#define SPI1_CS_Pin GPIO_PIN_10
#define SPI1_CS_GPIO_Port GPIOA
#define LED_RED_Pin GPIO_PIN_0
#define LED_RED_GPIO_Port GPIOD
#define LED_YELLOW_Pin GPIO_PIN_1
#define LED_YELLOW_GPIO_Port GPIOD
#define LED_WHITE_Pin GPIO_PIN_2
#define LED_WHITE_GPIO_Port GPIOD
#define LED_BLUE_Pin GPIO_PIN_3
#define LED_BLUE_GPIO_Port GPIOD
#define LED_GREEN_Pin GPIO_PIN_4
#define LED_GREEN_GPIO_Port GPIOD
#define Buzzer_Pin GPIO_PIN_5
#define Buzzer_GPIO_Port GPIOD
#define DEV_EN_Pin GPIO_PIN_3
#define DEV_EN_GPIO_Port GPIOB
#define M_EN_Pin GPIO_PIN_4
#define M_EN_GPIO_Port GPIOB
#define Contact4_Pin GPIO_PIN_5
#define Contact4_GPIO_Port GPIOB
#define Contact4_EXTI_IRQn EXTI4_15_IRQn
#define Bordure1_Pin GPIO_PIN_8
#define Bordure1_GPIO_Port GPIOB
#define Bordure1_EXTI_IRQn EXTI4_15_IRQn
#define Bordure2_Pin GPIO_PIN_9
#define Bordure2_GPIO_Port GPIOB
#define Bordure2_EXTI_IRQn EXTI4_15_IRQn
#define USART4_TX_LIDAR_Pin GPIO_PIN_10
#define USART4_TX_LIDAR_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
