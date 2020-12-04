/**
  ******************************************************************************
  * File Name          : TouchGFXHAL.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#include <TouchGFXHAL.hpp>

/* USER CODE BEGIN TouchGFXHAL.cpp */

#include "stm32f4xx.h"
#include <fal.h>
//#include <dfs_posix.h>

using namespace touchgfx;

void TouchGFXHAL::initialize()
{
    // Calling parent implementation of initialize().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.
    // Please note, HAL::initialize() must be called to initialize the framework.

    TouchGFXGeneratedHAL::initialize();
}

/**
 * Gets the frame buffer address used by the TFT controller.
 *
 * @return The address of the frame buffer currently being displayed on the TFT.
 */
uint16_t* TouchGFXHAL::getTFTFrameBuffer() const
{
    // Calling parent implementation of getTFTFrameBuffer().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    return TouchGFXGeneratedHAL::getTFTFrameBuffer();
}

/**
 * Sets the frame buffer address used by the TFT controller.
 *
 * @param [in] address New frame buffer address.
 */
void TouchGFXHAL::setTFTFrameBuffer(uint16_t* address)
{
    // Calling parent implementation of setTFTFrameBuffer(uint16_t* address).
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::setTFTFrameBuffer(address);
}

/**
 * This function is called whenever the framework has performed a partial draw.
 *
 * @param rect The area of the screen that has been drawn, expressed in absolute coordinates.
 *
 * @see flushFrameBuffer().
 */
void TouchGFXHAL::flushFrameBuffer(const touchgfx::Rect& rect)
{
    // Calling parent implementation of flushFrameBuffer(const touchgfx::Rect& rect).
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.
    // Please note, HAL::flushFrameBuffer(const touchgfx::Rect& rect) must
    // be called to notify the touchgfx framework that flush has been performed.

    TouchGFXGeneratedHAL::flushFrameBuffer(rect);
}

/**
 * Configures the interrupts relevant for TouchGFX. This primarily entails setting
 * the interrupt priorities for the DMA and LCD interrupts.
 */
void TouchGFXHAL::configureInterrupts()
{
    // Calling parent implementation of configureInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::configureInterrupts();
}

/**
 * Used for enabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::enableInterrupts()
{
    // Calling parent implementation of enableInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::enableInterrupts();
}

/**
 * Used for disabling interrupts set in configureInterrupts()
 */
void TouchGFXHAL::disableInterrupts()
{
    // Calling parent implementation of disableInterrupts().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::disableInterrupts();
}

/**
 * Configure the LCD controller to fire interrupts at VSYNC. Called automatically
 * once TouchGFX initialization has completed.
 */
void TouchGFXHAL::enableLCDControllerInterrupt()
{
    // Calling parent implementation of enableLCDControllerInterrupt().
    //
    // To overwrite the generated implementation, omit call to parent function
    // and implemented needed functionality here.

    TouchGFXGeneratedHAL::enableLCDControllerInterrupt();
}

bool TouchGFXHAL::blockCopy(void* RESTRICT dest, const void* RESTRICT src, uint32_t numBytes)
{
	uint32_t dataOffset = (uint32_t)src;
	if (dataOffset >= 0x90000000 && dataOffset < 0x92000000)
	{
		const struct fal_partition *part;
		part = fal_partition_find("tgfx");

		dataOffset =  dataOffset - 0x90000000;
		// for copying data from there.
		if (part != RT_NULL)
		{
			fal_partition_read(part, dataOffset, (uint8_t *)dest, numBytes);
		}
		return true;
	}	
	else
	{
		// For all other addresses, just use the default implementation.
		// This is important, as blockCopy is also used for other things in the core framework.
		return HAL::blockCopy(dest, src, numBytes);
	}	
	
	
//	uint32_t dataOffset = (uint32_t)src;
//	if (dataOffset >= 0x90000000 && dataOffset < 0x92000000)
//	{
//		char *updata_pciture;

//		/* get the boot count number from Env */
//		updata_pciture = ef_get_env("updata_pciture");	
//		
//		if(0 == strcmp(updata_pciture, "spi flash"))
//		{
//			const struct fal_partition *part;
//			part = fal_partition_find("tgfx");

//			dataOffset =  dataOffset - 0x90000000;
//			// for copying data from there.
//			if (part != RT_NULL)
//			{
//				fal_partition_read(part, dataOffset, (uint8_t *)dest, numBytes);
//			}
//			return true;
//		}
//		else
//		{
//			int fd;
//			struct statfs buffer;
//			if(rt_device_find("sd0") != RT_NULL)
//			{
//				if ((dfs_statfs("/",&buffer) == RT_EOK)|(dfs_mount("sd0", "/", "elm", 0, 0) == RT_EOK))
//				{
//					fd = open("/ER_EROM1", O_RDONLY, 0);						
//					if (fd < 0)
//					{
//						rt_kprintf("open file for read failed\n");
//						return false;
//					}
//					dataOffset =  dataOffset - 0x90000000;
//					lseek(fd, dataOffset, SEEK_SET);
//					// for copying data from there.
//					read(fd, (uint8_t *)dest, numBytes);
//					close(fd);
//				}
//				return true;	
//			}
//			else
//			{
//				return false;
//			}
//		}
//	}	
//	else
//	{
//		// For all other addresses, just use the default implementation.
//		// This is important, as blockCopy is also used for other things in the core framework.
//		return HAL::blockCopy(dest, src, numBytes);
//	}	
}

/* USER CODE END TouchGFXHAL.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
