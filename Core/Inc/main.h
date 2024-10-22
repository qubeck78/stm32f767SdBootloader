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
#include "stm32f7xx_hal.h"

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
#define VCC12_MEASUREMENT_Pin GPIO_PIN_1
#define VCC12_MEASUREMENT_GPIO_Port GPIOC
#define VCC24_MEASUREMENT_Pin GPIO_PIN_2
#define VCC24_MEASUREMENT_GPIO_Port GPIOC
#define VCC5_MEASUREMENT_Pin GPIO_PIN_3
#define VCC5_MEASUREMENT_GPIO_Port GPIOC
#define GPI0_Pin GPIO_PIN_0
#define GPI0_GPIO_Port GPIOA
#define LIGHT_SENSOR_Pin GPIO_PIN_0
#define LIGHT_SENSOR_GPIO_Port GPIOB
#define VCC_SOLAR_MEASUREMENT_Pin GPIO_PIN_1
#define VCC_SOLAR_MEASUREMENT_GPIO_Port GPIOB
#define VCC_PRINTER_ON_Pin GPIO_PIN_8
#define VCC_PRINTER_ON_GPIO_Port GPIOE
#define BUZZER_TTL_Pin GPIO_PIN_9
#define BUZZER_TTL_GPIO_Port GPIOE
#define VCC_COIN_ON_Pin GPIO_PIN_10
#define VCC_COIN_ON_GPIO_Port GPIOE
#define CJ_MOTOR_TTL_Pin GPIO_PIN_11
#define CJ_MOTOR_TTL_GPIO_Port GPIOE
#define CJ_SENSOR_Pin GPIO_PIN_12
#define CJ_SENSOR_GPIO_Port GPIOE
#define ES_SENSOR_Pin GPIO_PIN_13
#define ES_SENSOR_GPIO_Port GPIOE
#define ES_DECL_MAG_TTL_Pin GPIO_PIN_14
#define ES_DECL_MAG_TTL_GPIO_Port GPIOE
#define ES_ACC_MAG_TTL_Pin GPIO_PIN_15
#define ES_ACC_MAG_TTL_GPIO_Port GPIOE
#define AP_MAGNET_Pin GPIO_PIN_9
#define AP_MAGNET_GPIO_Port GPIOH
#define AP_COIN_Pin GPIO_PIN_10
#define AP_COIN_GPIO_Port GPIOH
#define AP_ERROR_Pin GPIO_PIN_11
#define AP_ERROR_GPIO_Port GPIOH
#define VCC_MDM_ON_Pin GPIO_PIN_12
#define VCC_MDM_ON_GPIO_Port GPIOH
#define MDM_ON_Pin GPIO_PIN_12
#define MDM_ON_GPIO_Port GPIOD
#define MDM_RING_Pin GPIO_PIN_13
#define MDM_RING_GPIO_Port GPIOD
#define LCD_RS_Pin GPIO_PIN_3
#define LCD_RS_GPIO_Port GPIOI
#define LCD_RW_Pin GPIO_PIN_0
#define LCD_RW_GPIO_Port GPIOD
#define LCD_E_Pin GPIO_PIN_1
#define LCD_E_GPIO_Port GPIOD
#define LCD_D0_Pin GPIO_PIN_3
#define LCD_D0_GPIO_Port GPIOD
#define LCD_D1_Pin GPIO_PIN_4
#define LCD_D1_GPIO_Port GPIOD
#define LCD_D2_Pin GPIO_PIN_5
#define LCD_D2_GPIO_Port GPIOD
#define LCD_D3_Pin GPIO_PIN_12
#define LCD_D3_GPIO_Port GPIOJ
#define LCD_D4_Pin GPIO_PIN_13
#define LCD_D4_GPIO_Port GPIOJ
#define LCD_D5_Pin GPIO_PIN_14
#define LCD_D5_GPIO_Port GPIOJ
#define LCD_D6_Pin GPIO_PIN_15
#define LCD_D6_GPIO_Port GPIOJ
#define LCD_D7_Pin GPIO_PIN_13
#define LCD_D7_GPIO_Port GPIOG
#define LCD_BL_R_Pin GPIO_PIN_14
#define LCD_BL_R_GPIO_Port GPIOG
#define LCD_BL_G_Pin GPIO_PIN_3
#define LCD_BL_G_GPIO_Port GPIOK
#define LCD_BL_B_Pin GPIO_PIN_4
#define LCD_BL_B_GPIO_Port GPIOK
#define GPI3_Pin GPIO_PIN_5
#define GPI3_GPIO_Port GPIOK
#define GPI2_Pin GPIO_PIN_6
#define GPI2_GPIO_Port GPIOK
#define GPI1_Pin GPIO_PIN_7
#define GPI1_GPIO_Port GPIOK
#define GPI5_Pin GPIO_PIN_4
#define GPI5_GPIO_Port GPIOI
#define GPI4_Pin GPIO_PIN_5
#define GPI4_GPIO_Port GPIOI
#define GPI7_Pin GPIO_PIN_6
#define GPI7_GPIO_Port GPIOI
#define GPI6_Pin GPIO_PIN_7
#define GPI6_GPIO_Port GPIOI

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
