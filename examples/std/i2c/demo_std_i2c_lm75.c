/*******************************************************************************
*                                 AMetal
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2018 Guangzhou ZHIYUAN Electronics Co., Ltd.
* All rights reserved.
*
* Contact information:
* web site:    http://www.zlg.cn/
*******************************************************************************/

/**
 * \file
 * \brief I2C 主机读取温度传感器 LM75 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 串口输出温度值。
 *
 * \par 源代码
 * \snippet demo_std_i2c_lm75.c src_std_i2c_lm75
 *
 * \internal
 * \par Modification history
 * - 1.00 15-12-10 win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_i2c_lm75
 * \copydoc demo_std_i2c_lm75.c
 */

/** [src_std_i2c_lm75] */
#include "ametal.h"
#include "am_temp.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**
 * \brief 例程入口
 */
void demo_std_i2c_lm75_entry (am_temp_handle_t handle)
{
    int32_t temperature = 0;

    AM_FOREVER {
        if (am_temp_read(handle, &temperature) != AM_OK) {
            AM_DBG_INFO("am_temp_read failed!\r\n");
        } else {
            AM_DBG_INFO("Current temperature is %d.%1d°C\r\n",
                        (int)temperature / 1000,
                        (int)(temperature - temperature / 1000 * 1000) / 100);
        }

        am_mdelay(1000);
    }
}
/** [src_std_i2c_lm75] */

/* end of file */
