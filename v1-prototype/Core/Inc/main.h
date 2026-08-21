/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define STBY_Pin GPIO_PIN_14
#define STBY_GPIO_Port GPIOC
#define fu_Pin GPIO_PIN_0
#define fu_GPIO_Port GPIOA
#define zheng_Pin GPIO_PIN_1
#define zheng_GPIO_Port GPIOA
#define N20PWM_T2C3_Pin GPIO_PIN_2
#define N20PWM_T2C3_GPIO_Port GPIOA
#define N20_T3C1_Pin GPIO_PIN_6
#define N20_T3C1_GPIO_Port GPIOA
#define N20_T3C2_Pin GPIO_PIN_7
#define N20_T3C2_GPIO_Port GPIOA
#define Key_Pin GPIO_PIN_15
#define Key_GPIO_Port GPIOB
#define EC11_T1C1_Pin GPIO_PIN_8
#define EC11_T1C1_GPIO_Port GPIOA
#define EC11_T1C2_Pin GPIO_PIN_9
#define EC11_T1C2_GPIO_Port GPIOA
#define buzzer_T4C4_Pin GPIO_PIN_9
#define buzzer_T4C4_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
