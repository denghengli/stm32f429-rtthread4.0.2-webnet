/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-21     SummerGift   add port file
 */

#include <rtthread.h>
#include <board.h>

//#define DRV_DEBUG
#define LOG_TAG             "drv.phy_port"
#include <drv_log.h>

#define ETH_RESET_IO    GET_PIN(H, 3)                //PHY RESET PIN

/* phy reset */
void phy_reset(void)
{
    rt_pin_write(ETH_RESET_IO, PIN_HIGH);
    rt_thread_mdelay(100);
    rt_pin_write(ETH_RESET_IO, PIN_LOW);
    rt_thread_mdelay(100);
}

int phy_init(void)
{
    rt_pin_mode(ETH_RESET_IO, PIN_MODE_OUTPUT);
    rt_pin_write(ETH_RESET_IO, PIN_LOW);
    return RT_EOK;
}
INIT_BOARD_EXPORT(phy_init);


