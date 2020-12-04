/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 * 2019-01-03     zylx         modify DMA support
 */

#ifndef __SOFT_SPI_CONFIG_H__
#define __SOFT_SPI_CONFIG_H__

#include <rtthread.h>

#ifdef __cplusplus
extern "C" {
#endif

// struct stm32_spi_config
// {
    // SPI_TypeDef *Instance;
    // char *bus_name;
    // struct dma_config *dma_rx, *dma_tx;
// };
// struct stm32_soft_spi_pin
// {
    // GPIO_TypeDef* GPIOx;
    // uint16_t GPIO_Pin;
// };
// struct stm32_soft_spi_config
// {
    // struct stm32_soft_spi_pin mosi_pin;
	// struct stm32_soft_spi_pin miso_pin;
	// struct stm32_soft_spi_pin sclk_pin;
    // char *bus_name;
// };
// CS(PG10) SCK(PG12)  MISO(PF8) MOSI(PF9)

#ifdef BSP_USING_SOFT_SPI1
#ifndef SOFT_SPI1_BUS_CONFIG
#define SOFT_SPI1_BUS_CONFIG                             \
    {                                                    \
        .mosi_pin.GPIOx = GPIOF,                         \
		.mosi_pin.GPIO_Pin = GPIO_PIN_9,                 \
		.miso_pin.GPIOx = GPIOF,                         \
		.miso_pin.GPIO_Pin = GPIO_PIN_8,                 \
		.sclk_pin.GPIOx = GPIOG,                         \
		.sclk_pin.GPIO_Pin = GPIO_PIN_12,                \
        .bus_name = "softspi1",                         \
    }
#endif /* SOFT_SPI1_BUS_CONFIG */
#endif /* BSP_USING_SOFT_SPI1 */

#ifdef __cplusplus
}
#endif

#endif /*__SPI_CONFIG_H__ */
