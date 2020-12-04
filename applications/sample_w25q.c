#include <rtthread.h>
#include <stdlib.h>
#include <string.h>
#include <drivers/spi.h>
#include "spi_flash.h"

#define SAMPLE_W25Q_NAME "W25Q128"

#define SAMPLE_W25Q_DEBUG	1

#define DBG_ENABLE
#define DBG_COLOR
#define DBG_SECTION_NAME    "sample.w25q"
#if SAMPLE_W25Q_DEBUG
#define DBG_LEVEL           DBG_LOG
#else
#define DBG_LEVEL           DBG_ERROR
#endif /* ONENET_DEBUG */
#include <rtdbg.h>

static struct spi_flash_device  *spi_flash_w25q;
//static struct rt_spi_device *spi_dev_w25q;
static char w25q_data[] = "Test w25q write";

static int w25q_samp_write(int argc, char *argv[])
{
	rt_err_t ret = RT_EOK;
	char spiflash_name[RT_NAME_MAX];
	char *send_data = NULL;
	
	if (argc == 2)
	{
		rt_strncpy(spiflash_name,argv[1],RT_NAME_MAX);
	}
	else
	{
		rt_strncpy(spiflash_name,SAMPLE_W25Q_NAME,RT_NAME_MAX);
	}
	
	/* 查找设备 */
	spi_flash_w25q = (struct spi_flash_device*)rt_device_find(spiflash_name);
	if (!spi_flash_w25q)
	{
		LOG_E("find %s failed!\n",spiflash_name);
		return RT_ERROR;
	}
	
	/* 申请内存，赋值写入数据 */
	send_data = rt_malloc(1024);
	if (send_data == RT_NULL)
	{
		LOG_E("No memory\n");
		return RT_ENOMEM;
	}
	rt_strncpy(send_data,w25q_data,sizeof(w25q_data));
	
	/* 写入，SPIFLASH是块类型设备，每次的是以块为单位进行读写*/
	rt_device_write(&spi_flash_w25q->flash_device,0,send_data,1);
	
	if (send_data)
		rt_free(send_data);
	
	return ret;
}


static int w25q_samp_read(int argc, char *argv[])
{
	rt_err_t ret = RT_EOK;
	char spiflash_name[RT_NAME_MAX];
	char *read_data = NULL,read_len=0;
	
	if (argc == 2)
	{
		rt_strncpy(spiflash_name,argv[1],RT_NAME_MAX);
	}
	else
	{
		rt_strncpy(spiflash_name,SAMPLE_W25Q_NAME,RT_NAME_MAX);
	}
	
	/* 查找设备 */
	spi_flash_w25q = (struct spi_flash_device*)rt_device_find(spiflash_name);
	if (!spi_flash_w25q)
	{
		LOG_E("find %s failed!\n",spiflash_name);
		return RT_ERROR;
	}
	
	/* 申请内存，赋值写入数据 */
	read_data = rt_malloc(1024);
	if (read_data == RT_NULL)
	{
		LOG_E("No memory\n");
		return RT_ENOMEM;
	}
	
	/* 读取，SPIFLASH是块类型设备，每次的是以块为单位进行读写*/
	read_len = rt_device_read(&spi_flash_w25q->flash_device,0,read_data,1);
	if (read_len)
	{
		LOG_D("w25q read data: %*s",sizeof(w25q_data),read_data);
	}
	
	if (read_data)
		rt_free(read_data);
	
	return ret;
}

/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(w25q_samp_write, w25q write sample);
MSH_CMD_EXPORT(w25q_samp_read,  w25q read  sample);


