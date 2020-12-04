/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32f4xx_hal.h"

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
#define BEEP_Pin GPIO_PIN_3
#define BEEP_GPIO_Port GPIOE
#define T_CS_Pin GPIO_PIN_8
#define T_CS_GPIO_Port GPIOI
#define WDT_CTRL_Pin GPIO_PIN_13
#define WDT_CTRL_GPIO_Port GPIOC
#define T_BUSY_Pin GPIO_PIN_11
#define T_BUSY_GPIO_Port GPIOI
#define DO5_Pin GPIO_PIN_0
#define DO5_GPIO_Port GPIOA
#define LED0_Pin GPIO_PIN_2
#define LED0_GPIO_Port GPIOH
#define soft_I2C2_SCL_Pin GPIO_PIN_4
#define soft_I2C2_SCL_GPIO_Port GPIOH
#define soft_I2C2_SDA_Pin GPIO_PIN_5
#define soft_I2C2_SDA_GPIO_Port GPIOH
#define DI2_Pin GPIO_PIN_3
#define DI2_GPIO_Port GPIOA
#define DI1_Pin GPIO_PIN_6
#define DI1_GPIO_Port GPIOA
#define ADC1_IN8_VI0_Pin GPIO_PIN_0
#define ADC1_IN8_VI0_GPIO_Port GPIOB
#define ADC1_IN9_VI1_Pin GPIO_PIN_1
#define ADC1_IN9_VI1_GPIO_Port GPIOB
#define T_SCK_Pin GPIO_PIN_6
#define T_SCK_GPIO_Port GPIOH
#define T_PEN_Pin GPIO_PIN_7
#define T_PEN_GPIO_Port GPIOH
#define I2C3_SDA_Pin GPIO_PIN_8
#define I2C3_SDA_GPIO_Port GPIOH
#define DO2_Pin GPIO_PIN_13
#define DO2_GPIO_Port GPIOD
#define T_MISO_Pin GPIO_PIN_3
#define T_MISO_GPIO_Port GPIOG
#define soft_I2C3_SCL_Pin GPIO_PIN_8
#define soft_I2C3_SCL_GPIO_Port GPIOA
#define T_MOSI_Pin GPIO_PIN_3
#define T_MOSI_GPIO_Port GPIOI
#define DO1_Pin GPIO_PIN_3
#define DO1_GPIO_Port GPIOD
#define DO4_Pin GPIO_PIN_7
#define DO4_GPIO_Port GPIOD

#define F_CS_Pin GPIO_PIN_10
#define F_CS_GPIO_Port GPIOG
#define SPI5_SCK_Pin GPIO_PIN_12
#define SPI5_SCK_GPIO_Port GPIOG

#define LCD_BL_Pin GPIO_PIN_6
#define LCD_BL_GPIO_Port GPIOB
#define DO3_Pin GPIO_PIN_7
#define DO3_GPIO_Port GPIOB
#define soft_I2C1_SCL_Pin GPIO_PIN_8
#define soft_I2C1_SCL_GPIO_Port GPIOB
#define soft_I2C1_SDA_Pin GPIO_PIN_9
#define soft_I2C1_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
