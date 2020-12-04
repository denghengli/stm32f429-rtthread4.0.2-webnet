/*
 * Change Logs:
 * Date           Author       Notes
 * 2020-05-20     Roy.yu       first version
 */

#include "board.h"

#ifdef RT_USING_SOFT_SPI

#include "drv_soft_spi.h"
#include "drv_config.h"
#include <string.h>

//#define DRV_DEBUG
#define LOG_TAG              "drv.spisoft"
#include <drv_log.h>

enum{
#ifdef BSP_USING_SOFT_SPI1
    SOFT_SPI1_INDEX,
#endif
};

static struct stm32_soft_spi_config soft_spi_config[] ={
#ifdef BSP_USING_SOFT_SPI1
    SOFT_SPI1_BUS_CONFIG,
#endif
};

static struct stm32_soft_spi soft_spi_bus_obj[sizeof(soft_spi_config) / sizeof(soft_spi_config[0])] = {0};

static rt_err_t stm32_spi_init(struct stm32_soft_spi *spi_drv, struct rt_spi_configuration *cfg){
    RT_ASSERT(spi_drv != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

	//mode = master
    if (cfg->mode & RT_SPI_SLAVE){
		return RT_EIO;
    }
	else
		spi_drv->mode = RT_SPI_MASTER;

    if (cfg->mode & RT_SPI_3WIRE){
        return RT_EIO;
    }
	
    if (cfg->data_width == 8 || cfg->data_width == 16)
		spi_drv->data_width = cfg->data_width;
	else{
        return RT_EIO;
    }

	
    if (cfg->mode & RT_SPI_CPHA){
		spi_drv->cpha = 1;
    }
    else{
        spi_drv->cpha = 0;
    }

    if (cfg->mode & RT_SPI_CPOL){
		spi_drv->cpol = 1;
    }
    else{
        spi_drv->cpol = 0;
    }

    if (cfg->mode & RT_SPI_NO_CS){
    }
    else{
    }

    if (cfg->max_hz >= 1200000){
		spi_drv->spi_delay = 0;
    }else if (cfg->max_hz >= 1000000){
        spi_drv->spi_delay = 8;
    }else if (cfg->max_hz >= 830000){
        spi_drv->spi_delay = 16;
    }

	
//    LOG_D("SPI limiting freq: %d, BaudRatePrescaler: %d",
//          cfg->max_hz,
//          spi_drv->max_hz);

    if (cfg->mode & RT_SPI_MSB){
		spi_drv->msb = 1;
    }
    else{
        spi_drv->msb = 0;
    }

	//init spi pin
	GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin = spi_drv->config->mosi_pin.GPIO_Pin;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(spi_drv->config->mosi_pin.GPIOx, &GPIO_Initure);
    HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx, spi_drv->config->mosi_pin.GPIO_Pin, GPIO_PIN_RESET);
	
    GPIO_Initure.Pin = spi_drv->config->miso_pin.GPIO_Pin;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(spi_drv->config->miso_pin.GPIOx, &GPIO_Initure);

    GPIO_Initure.Pin = spi_drv->config->sclk_pin.GPIO_Pin;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(spi_drv->config->sclk_pin.GPIOx, &GPIO_Initure);
	if(spi_drv->cpol)
		HAL_GPIO_WritePin(spi_drv->config->sclk_pin.GPIOx,spi_drv->config->sclk_pin.GPIO_Pin,GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(spi_drv->config->sclk_pin.GPIOx,spi_drv->config->sclk_pin.GPIO_Pin,GPIO_PIN_RESET);
    LOG_D("%s init done", spi_drv->config->bus_name);
    return RT_EOK;
}

static inline void spi_delay(uint8_t time){
	switch(time){
#if defined(__CC_ARM) || defined(__CLANG_ARM)
		case 16:__nop();
		case 15:__nop();
		case 14:__nop();
		case 13:__nop();
		case 12:__nop();
		case 11:__nop();
		case 10:__nop();
		case 9:__nop();
		case 8:__nop();
		case 7:__nop();
		case 6:__nop();
		case 5:__nop();
		case 4:__nop();
		case 3:__nop();
		case 2:__nop();
		case 1:__nop();
#elif __ICCARM__
        case 16:__no_operation();
        case 15:__no_operation();
        case 14:__no_operation();
        case 13:__no_operation();
        case 12:__no_operation();
        case 11:__no_operation();
        case 10:__no_operation();
        case 9:__no_operation();
        case 8:__no_operation();
        case 7:__no_operation();
        case 6:__no_operation();
        case 5:__no_operation();
        case 4:__no_operation();
        case 3:__no_operation();
        case 2:__no_operation();
        case 1:__no_operation();
#endif
		default:
			break;
	}
}

static rt_uint32_t soft_spi_read_write_bytes(struct stm32_soft_spi *spi_drv, uint8_t* send_buff, uint8_t* recv_buff, uint32_t len){
	uint32_t dataIndex = 0;
	uint8_t time = 0;
	for(uint32_t i = 0; i<len; i++){
		if(spi_drv->cpha){	//CPHA=1
			HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
		}
		
		if(spi_drv->data_width == 16) time = 1;
		else time = 0;
		
		do{
//			for(uint8_t j = 0; j < 8; j++){
//				if ((send_buff[dataIndex] & 0x80) != 0){
//					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_SET);
//				}else{
//					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_RESET);
//				}
//				send_buff[dataIndex] <<= 1;
//				spi_delay(spi_drv->spi_delay);
//				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
//				
//				recv_buff[dataIndex] <<= 1;
//				if (HAL_GPIO_ReadPin(spi_drv->config->miso_pin.GPIOx, spi_drv->config->miso_pin.GPIO_Pin))
//					recv_buff[dataIndex] |= 0x01;
//				spi_delay(spi_drv->spi_delay);
//				if(time != 0 || j != 7){
//					HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
//				}
//			}
			for(uint8_t j = 0; j < 8; j++){
				if ((send_buff[dataIndex] & 0x80) != 0){
					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_SET);
				}else{
					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_RESET);
				}
				send_buff[dataIndex] <<= 1;
				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
				spi_delay(spi_drv->spi_delay);
				
				recv_buff[dataIndex] <<= 1;
				if (HAL_GPIO_ReadPin(spi_drv->config->miso_pin.GPIOx, spi_drv->config->miso_pin.GPIO_Pin))
					recv_buff[dataIndex] |= 0x01;
				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
				spi_delay(spi_drv->spi_delay);
			}
			dataIndex++;
		}while(time--);
		spi_delay(spi_drv->spi_delay);
	}
    return len;
}

static rt_uint32_t soft_spi_read_bytes(struct stm32_soft_spi *spi_drv, uint8_t* recv_buff, uint32_t len){
	uint8_t send_buff = spi_drv->dummy_data;
	uint32_t dataIndex = 0;
	uint8_t time = 0;
	for(uint32_t i = 0; i<len; i++){
		if(spi_drv->cpha){	//CPHA=1
			HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
		}
		
		if(spi_drv->data_width == 16) time = 1;
		else time = 0;
		
		do{
//			for(uint8_t j = 0; j < 8; j++){
//				if ((send_buff & 0x80) != 0){
//					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_SET);
//				}else{
//					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_RESET);
//				}
//				send_buff <<= 1;
//				spi_delay(spi_drv->spi_delay);
//				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
//				
//				recv_buff[dataIndex] <<= 1;
//				if (HAL_GPIO_ReadPin(spi_drv->config->miso_pin.GPIOx, spi_drv->config->miso_pin.GPIO_Pin))
//					recv_buff[dataIndex] |= 0x01;
//				spi_delay(spi_drv->spi_delay);
//				if(time != 0 || j != 7){
//					HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
//				}
//			}
			for(uint8_t j = 0; j < 8; j++){
				if ((send_buff & 0x80) != 0){
					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_SET);
				}else{
					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_RESET);
				}
				send_buff <<= 1;
				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
				spi_delay(spi_drv->spi_delay);
				
				recv_buff[dataIndex] <<= 1;
				if (HAL_GPIO_ReadPin(spi_drv->config->miso_pin.GPIOx, spi_drv->config->miso_pin.GPIO_Pin))
					recv_buff[dataIndex] |= 0x01;
				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
				spi_delay(spi_drv->spi_delay);
			}
			dataIndex++;
		}while(time--);
		spi_delay(spi_drv->spi_delay);
	}
    return len;
}

static rt_uint32_t soft_spi_write_bytes(struct stm32_soft_spi *spi_drv, uint8_t* send_buff, uint32_t len){
	uint8_t recv_buff = 0;
	uint32_t dataIndex = 0;
	uint8_t time = 0;
	for(uint32_t i = 0; i<len; i++){
		if(spi_drv->cpha){	//CPHA=1
			HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
		}
		
		if(spi_drv->data_width == 16) time = 1;
		else time = 0;
		
		do{
//			for(uint8_t j = 0; j < 8; j++){
//				if ((send_buff[dataIndex] & 0x80) != 0){
//					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_SET);
//				}else{
//					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_RESET);
//				}
//				send_buff[dataIndex] <<= 1;
//				spi_delay(spi_drv->spi_delay);
//				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
//				
//				recv_buff <<= 1;
//				if (HAL_GPIO_ReadPin(spi_drv->config->miso_pin.GPIOx, spi_drv->config->miso_pin.GPIO_Pin))
//					recv_buff |= 0x01;
//				spi_delay(spi_drv->spi_delay);
//				if(time != 0 || j != 7){
//					HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
//				}
//			}
			for(uint8_t j = 0; j < 8; j++){
				if ((send_buff[dataIndex] & 0x80) != 0){
					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_SET);
				}else{
					HAL_GPIO_WritePin(spi_drv->config->mosi_pin.GPIOx,spi_drv->config->mosi_pin.GPIO_Pin,GPIO_PIN_RESET);
				}
				send_buff[dataIndex] <<= 1;
				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
				spi_delay(spi_drv->spi_delay);
				
				recv_buff <<= 1;
				if (HAL_GPIO_ReadPin(spi_drv->config->miso_pin.GPIOx, spi_drv->config->miso_pin.GPIO_Pin))
					recv_buff |= 0x01;
				HAL_GPIO_TogglePin(spi_drv->config->sclk_pin.GPIOx, spi_drv->config->sclk_pin.GPIO_Pin);
				spi_delay(spi_drv->spi_delay);
			}
			dataIndex++;
		}while(time--);
		spi_delay(spi_drv->spi_delay);
	}
    return len;
}

static rt_uint32_t spixfer(struct rt_spi_device *device, struct rt_spi_message *message){
    rt_uint32_t state;
    rt_size_t message_length;
    rt_uint8_t *recv_buf;
    const rt_uint8_t *send_buf;

    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);
    RT_ASSERT(message != RT_NULL);

    struct stm32_soft_spi *spi_drv =  rt_container_of(device->bus, struct stm32_soft_spi, spi_bus);
    struct stm32_soft_spi_pin *cs = device->parent.user_data;

    if (message->cs_take){
        HAL_GPIO_WritePin(cs->GPIOx, cs->GPIO_Pin, GPIO_PIN_RESET);
    }

    LOG_D("%s transfer prepare and start", spi_drv->config->bus_name);
    LOG_D("%s sendbuf: %X, recvbuf: %X, length: %d",
          spi_drv->config->bus_name,
          (uint32_t)message->send_buf,
          (uint32_t)message->recv_buf, message->length);

    message_length = message->length;
    recv_buf = message->recv_buf;
    send_buf = message->send_buf;
	
    if(message_length){
        if (message->send_buf && message->recv_buf){
            state = soft_spi_read_write_bytes(spi_drv, (uint8_t *)send_buf, (uint8_t *)recv_buf, message_length);
        }
        else if (message->send_buf){
            state = soft_spi_write_bytes(spi_drv, (uint8_t *)send_buf, message_length);
        }
        else{
            memset((uint8_t *)recv_buf, 0xff, message_length);
            state = soft_spi_read_bytes(spi_drv, (uint8_t *)recv_buf, message_length);
        }

        if (state != message_length){
            LOG_I("spi transfer error : %d", state);
            message->length = 0;
        }
        else{
            LOG_D("%s transfer done", spi_drv->config->bus_name);
        }
    }

    if (message->cs_release){
        HAL_GPIO_WritePin(cs->GPIOx, cs->GPIO_Pin, GPIO_PIN_SET);
    }

    return message->length;
}

static rt_err_t spi_configure(struct rt_spi_device *device,
                              struct rt_spi_configuration *configuration){
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(configuration != RT_NULL);

    struct stm32_soft_spi *spi_drv =  rt_container_of(device->bus, struct stm32_soft_spi, spi_bus);
    spi_drv->cfg = configuration;

    return stm32_spi_init(spi_drv, configuration);
}

static const struct rt_spi_ops stm_spi_ops ={
    .configure = spi_configure,
    .xfer = spixfer,
};

static int rt_soft_spi_bus_init(void){
    rt_err_t result;
    for (int i = 0; i < sizeof(soft_spi_config) / sizeof(soft_spi_config[0]); i++){
        soft_spi_bus_obj[i].config = &soft_spi_config[i];
        soft_spi_bus_obj[i].spi_bus.parent.user_data = &soft_spi_config[i];
        result = rt_spi_bus_register(&soft_spi_bus_obj[i].spi_bus, soft_spi_config[i].bus_name, &stm_spi_ops);
        RT_ASSERT(result == RT_EOK);

        LOG_D("%s bus init done", soft_spi_config[i].bus_name);
    }

    return result;
}

/**
  * Attach the spi device to SPI bus, this function must be used after initialization.
  */
rt_err_t rt_soft_spi_device_attach(const char *bus_name, const char *device_name, GPIO_TypeDef *cs_gpiox, uint16_t cs_gpio_pin){
    RT_ASSERT(bus_name != RT_NULL);
    RT_ASSERT(device_name != RT_NULL);

    rt_err_t result;
    struct rt_spi_device *spi_device;
    struct stm32_soft_spi_pin *cs_pin;

    /* initialize the cs pin && select the slave*/
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin = cs_gpio_pin;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(cs_gpiox, &GPIO_Initure);
    HAL_GPIO_WritePin(cs_gpiox, cs_gpio_pin, GPIO_PIN_SET);

    /* attach the device to spi bus*/
    spi_device = (struct rt_spi_device *)rt_malloc(sizeof(struct rt_spi_device));
    RT_ASSERT(spi_device != RT_NULL);
    cs_pin = (struct stm32_soft_spi_pin *)rt_malloc(sizeof(struct stm32_soft_spi_pin));
    RT_ASSERT(cs_pin != RT_NULL);
    cs_pin->GPIOx = cs_gpiox;
    cs_pin->GPIO_Pin = cs_gpio_pin;
    result = rt_spi_bus_attach_device(spi_device, device_name, bus_name, (void *)cs_pin);

    if (result != RT_EOK){
        LOG_E("%s attach to %s faild, %d\n", device_name, bus_name, result);
    }

    RT_ASSERT(result == RT_EOK);

    LOG_D("%s attach to %s done", device_name, bus_name);

    return result;
}

int rt_soft_spi_init(void){
    return rt_soft_spi_bus_init();
}
INIT_BOARD_EXPORT(rt_soft_spi_init);

#endif /* RT_USING_SOFT_SPI */
