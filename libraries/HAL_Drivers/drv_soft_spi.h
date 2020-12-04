/*
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-20      Roy.yu   first version
 */

#ifndef __DRV_SOFT_SPI_H_
#define __DRV_SOFT_SPI_H_

#include <rtthread.h>
#include "rtdevice.h"
#include <rthw.h>
#include <drv_common.h>
#include "drv_dma.h"

rt_err_t rt_soft_spi_device_attach(const char *bus_name, const char *device_name, GPIO_TypeDef* cs_gpiox, uint16_t cs_gpio_pin);

struct stm32_soft_spi_pin
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
};

struct stm32_soft_spi_config
{
    struct stm32_soft_spi_pin mosi_pin;
	struct stm32_soft_spi_pin miso_pin;
	struct stm32_soft_spi_pin sclk_pin;
    char *bus_name;
};

struct stm32_soft_spi_device
{
    rt_uint8_t cs_pin;
    char *bus_name;
    char *device_name;
};

/* stm32 soft spi dirver class */
struct stm32_soft_spi
{
	uint8_t mode;
	uint8_t cpha;
	uint8_t cpol;
	uint8_t data_width;
	uint8_t msb;
	uint16_t dummy_data;
	uint32_t spi_delay;
    struct stm32_soft_spi_config *config;
    struct rt_spi_configuration *cfg;
    struct rt_spi_bus spi_bus;
};

#endif /*__DRV_SOFT_SPI_H_ */
