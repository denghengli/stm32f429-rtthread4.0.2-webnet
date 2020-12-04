/**
  ******************************************************************************
  * @file    stm32f4xx_hal_ltdc_ex.h
  * @author  MCD Application Team
  * @brief   Header file of LTDC HAL Extension module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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
#ifndef __STM32F4xx_HAL_LTDC_EX_H
#define __STM32F4xx_HAL_LTDC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @defgroup LTDC_HSPolarity 
  * @{
  */
#define LTDC_HSPolarity_AL                ((uint32_t)0x00000000)                /*!< Horizontal Synchronization is active low. */
#define LTDC_HSPolarity_AH                LTDC_GCR_HSPOL                        /*!< Horizontal Synchronization is active high. */

#define LTDC_VSPolarity_AL                ((uint32_t)0x00000000)                /*!< Vertical Synchronization is active low. */
#define LTDC_VSPolarity_AH                LTDC_GCR_VSPOL     
	 
	 
#define LTDC_DEPolarity_AL                ((uint32_t)0x00000000)                /*!< Data Enable, is active low. */
#define LTDC_DEPolarity_AH                LTDC_GCR_DEPOL    
	 
	 
/** @defgroup LTDC_PCPolarity 
  * @{
  */
#define LTDC_PCPolarity_IPC               ((uint32_t)0x00000000)                /*!< input pixel clock. */
#define LTDC_PCPolarity_IIPC              LTDC_GCR_PCPOL  	 
	 
	 
/** @defgroup LTDC_BlendingFactor1 
  * @{
  */

#define LTDC_BlendingFactor1_CA                       ((uint32_t)0x00000400)
#define LTDC_BlendingFactor1_PAxCA                    ((uint32_t)0x00000600)	 
	 
	 
	 
/** @defgroup LTDC_BlendingFactor2
  * @{
  */

#define LTDC_BlendingFactor2_CA                       ((uint32_t)0x00000005)
#define LTDC_BlendingFactor2_PAxCA                    ((uint32_t)0x00000007)	 
	 
	 
	 
	 
	 
	 
/** 
  * @brief  LTDC Init structure definition  
  */

typedef struct
{
  uint32_t LTDC_HSPolarity;                 /*!< configures the horizontal synchronization polarity.
                                                 This parameter can be one value of @ref LTDC_HSPolarity */

  uint32_t LTDC_VSPolarity;                 /*!< configures the vertical synchronization polarity.
                                                 This parameter can be one value of @ref LTDC_VSPolarity */

  uint32_t LTDC_DEPolarity;                 /*!< configures the data enable polarity. This parameter can
                                                 be one of value of @ref LTDC_DEPolarity */

  uint32_t LTDC_PCPolarity;                 /*!< configures the pixel clock polarity. This parameter can
                                                 be one of value of @ref LTDC_PCPolarity */

  uint32_t LTDC_HorizontalSync;             /*!< configures the number of Horizontal synchronization 
                                                 width. This parameter must range from 0x000 to 0xFFF. */

  uint32_t LTDC_VerticalSync;               /*!< configures the number of Vertical synchronization 
                                                 height. This parameter must range from 0x000 to 0x7FF. */

  uint32_t LTDC_AccumulatedHBP;             /*!< configures the accumulated horizontal back porch width.
                                                 This parameter must range from LTDC_HorizontalSync to 0xFFF. */

  uint32_t LTDC_AccumulatedVBP;             /*!< configures the accumulated vertical back porch height.
                                                 This parameter must range from LTDC_VerticalSync to 0x7FF. */
            
  uint32_t LTDC_AccumulatedActiveW;         /*!< configures the accumulated active width. This parameter 
                                                 must range from LTDC_AccumulatedHBP to 0xFFF. */

  uint32_t LTDC_AccumulatedActiveH;         /*!< configures the accumulated active height. This parameter 
                                                 must range from LTDC_AccumulatedVBP to 0x7FF. */

  uint32_t LTDC_TotalWidth;                 /*!< configures the total width. This parameter 
                                                 must range from LTDC_AccumulatedActiveW to 0xFFF. */

  uint32_t LTDC_TotalHeigh;                 /*!< configures the total height. This parameter 
                                                 must range from LTDC_AccumulatedActiveH to 0x7FF. */
            
  uint32_t LTDC_BackgroundRedValue;         /*!< configures the background red value.
                                                 This parameter must range from 0x00 to 0xFF. */

  uint32_t LTDC_BackgroundGreenValue;       /*!< configures the background green value.
                                                 This parameter must range from 0x00 to 0xFF. */ 

   uint32_t LTDC_BackgroundBlueValue;       /*!< configures the background blue value.
                                                 This parameter must range from 0x00 to 0xFF. */
} LTDC_InitExTypeDef;	 
	 

/** 
  * @brief  LTDC Layer structure definition  
  */

typedef struct
{
  uint32_t LTDC_HorizontalStart;            /*!< Configures the Window Horizontal Start Position.
                                                 This parameter must range from 0x000 to 0xFFF. */
            
  uint32_t LTDC_HorizontalStop;             /*!< Configures the Window Horizontal Stop Position.
                                                 This parameter must range from 0x0000 to 0xFFFF. */
  
  uint32_t LTDC_VerticalStart;              /*!< Configures the Window vertical Start Position.
                                                 This parameter must range from 0x000 to 0xFFF. */

  uint32_t LTDC_VerticalStop;               /*!< Configures the Window vaertical Stop Position.
                                                 This parameter must range from 0x0000 to 0xFFFF. */
  
  uint32_t LTDC_PixelFormat;                /*!< Specifies the pixel format. This parameter can be 
                                                 one of value of @ref LTDC_Pixelformat */

  uint32_t LTDC_ConstantAlpha;              /*!< Specifies the constant alpha used for blending.
                                                 This parameter must range from 0x00 to 0xFF. */

  uint32_t LTDC_DefaultColorBlue;           /*!< Configures the default blue value.
                                                 This parameter must range from 0x00 to 0xFF. */

  uint32_t LTDC_DefaultColorGreen;          /*!< Configures the default green value.
                                                 This parameter must range from 0x00 to 0xFF. */
            
  uint32_t LTDC_DefaultColorRed;            /*!< Configures the default red value.
                                                 This parameter must range from 0x00 to 0xFF. */

  uint32_t LTDC_DefaultColorAlpha;          /*!< Configures the default alpha value.
                                                 This parameter must range from 0x00 to 0xFF. */

  uint32_t LTDC_BlendingFactor_1;           /*!< Select the blending factor 1. This parameter 
                                                 can be one of value of @ref LTDC_BlendingFactor1 */

  uint32_t LTDC_BlendingFactor_2;           /*!< Select the blending factor 2. This parameter 
                                                 can be one of value of @ref LTDC_BlendingFactor2 */
            
  uint32_t LTDC_CFBStartAdress;             /*!< Configures the color frame buffer address */

  uint32_t LTDC_CFBLineLength;              /*!< Configures the color frame buffer line length. 
                                                 This parameter must range from 0x0000 to 0x1FFF. */

  uint32_t LTDC_CFBPitch;                   /*!< Configures the color frame buffer pitch in bytes.
                                                 This parameter must range from 0x0000 to 0x1FFF. */
                                                 
  uint32_t LTDC_CFBLineNumber;              /*!< Specifies the number of line in frame buffer. 
                                                 This parameter must range from 0x000 to 0x7FF. */
} LTDC_Layer_InitTypeDef;

/** 
  * @brief  LTDC Position structure definition  
  */

typedef struct
{
  uint32_t LTDC_POSX;                         /*!<  Current X Position */
  uint32_t LTDC_POSY;                         /*!<  Current Y Position */
} LTDC_PosTypeDef;

typedef struct
{
  uint32_t LTDC_BlueWidth;                        /*!< Blue width */
  uint32_t LTDC_GreenWidth;                       /*!< Green width */
  uint32_t LTDC_RedWidth;                         /*!< Red width */
} LTDC_RGBTypeDef;

typedef struct
{
  uint32_t LTDC_ColorKeyBlue;               /*!< Configures the color key blue value. 
                                                 This parameter must range from 0x00 to 0xFF. */

  uint32_t LTDC_ColorKeyGreen;              /*!< Configures the color key green value. 
                                                 This parameter must range from 0x00 to 0xFF. */
            
  uint32_t LTDC_ColorKeyRed;                /*!< Configures the color key red value. 
                                                 This parameter must range from 0x00 to 0xFF. */
} LTDC_ColorKeying_InitTypeDef;

typedef struct
{
  uint32_t LTDC_CLUTAdress;                 /*!< Configures the CLUT address.
                                                 This parameter must range from 0x00 to 0xFF. */

  uint32_t LTDC_BlueValue;                  /*!< Configures the blue value. 
                                                 This parameter must range from 0x00 to 0xFF. */
                                                 
  uint32_t LTDC_GreenValue;                 /*!< Configures the green value. 
                                                 This parameter must range from 0x00 to 0xFF. */

  uint32_t LTDC_RedValue;                   /*!< Configures the red value.
                                                 This parameter must range from 0x00 to 0xFF. */
} LTDC_CLUT_InitTypeDef;	  
	  
	  
	 
	 
void LTDC_ITConfig(uint32_t LTDC_IT, FunctionalState NewState);	 
	 
void LTDC_LayerInit(LTDC_Layer_TypeDef* LTDC_Layerx, LTDC_Layer_InitTypeDef* LTDC_Layer_InitStruct);	 
	 
void LTDC_LayerCmd(LTDC_Layer_TypeDef* LTDC_Layerx, FunctionalState NewState);	 
	 
void LTDC_ReloadConfig(uint32_t LTDC_Reload);	 
	 
	 
#if defined(STM32F469xx) || defined(STM32F479xx)
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_dsi.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @addtogroup LTDCEx
  * @{
  */

/* Exported types ------------------------------------------------------------*/ 
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup LTDCEx_Exported_Functions
  * @{
  */

/** @addtogroup LTDCEx_Exported_Functions_Group1
  * @{
  */
HAL_StatusTypeDef HAL_LTDCEx_StructInitFromVideoConfig(LTDC_HandleTypeDef* hltdc, DSI_VidCfgTypeDef *VidCfg);
HAL_StatusTypeDef HAL_LTDCEx_StructInitFromAdaptedCommandConfig(LTDC_HandleTypeDef* hltdc, DSI_CmdCfgTypeDef *CmdCfg);
/**
  * @}
  */ 
 
/**
  * @}
  */ 

/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @}
  */ 

/**
  * @}
  */

	  
	  
	  	  
	  
	  
	  
	  
#endif /* STM32F469xx || STM32F479xx */ 
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_LTDC_EX_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
