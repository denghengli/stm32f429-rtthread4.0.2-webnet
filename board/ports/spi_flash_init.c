/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-27     SummerGift   add spi flash port file
 */

#include <rtthread.h>
#include "spi_flash.h"
#include "spi_flash_sfud.h"
#include "drv_soft_spi.h"
#include "fal.h"
#include "dfs_fs.h"

#define DRV_DEBUG
#define LOG_TAG              "flash-init"
#include <drv_log.h>

#if defined(BSP_USING_SPI_FLASH)
static int rt_hw_spi_flash_init(void)
{
    __HAL_RCC_GPIOG_CLK_ENABLE();
    rt_soft_spi_device_attach("softspi1", "softspi10", GPIOG, GPIO_PIN_10);

    if (RT_NULL == rt_sfud_flash_probe("W25Q64", "softspi10"))
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_init);
#endif

#if defined(PKG_USING_FAL)
int fs_init(void)
{
    /* partition initialized */
    fal_init();
	return 0;
}
INIT_COMPONENT_EXPORT(fs_init);
#endif

#if defined(RT_USING_DFS_ELMFAT)
#define FS_PARTITION_NAME "fs"
int elm_fatfs_init(void)
{
    /* partition initialized */
//    elm_init();
	//dfs_mkfs("elm", "fs"); /* 在fs块设备上创建elm文件系统*/
  
	/* Create a block device on the "fs" partition of spi flash */
    struct rt_device *flash_dev = fal_blk_device_create(FS_PARTITION_NAME);
    if (flash_dev == NULL)
    {
        rt_kprintf("Can't create a block device on '%s' partition.\n", FS_PARTITION_NAME);
    }
    else
    {
        rt_kprintf("Create a block device on the %s partition of flash successful.\n", FS_PARTITION_NAME);
    }
	
	/* mount the file system from "fs" partition of spi flash. */
    if (dfs_mount(FS_PARTITION_NAME, "/", "elm", 0, 0) == 0)
    {
        LOG_I("Filesystem initialized!");
    }
    else
    {
        LOG_E("Failed to initialize filesystem!");
        LOG_D("You should create a filesystem on the block device first!");
    }
	return 0;
}
INIT_COMPONENT_EXPORT(elm_fatfs_init);
#endif
