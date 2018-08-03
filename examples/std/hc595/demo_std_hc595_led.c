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
 * \brief HC595 输出例程，为便于观察现象，建议将HC595的8个输出连接到LED灯
 *
 * - 实验现象：
 *   1. 流水灯现象。
 *
 * \par 源代码
 * \snippet demo_std_hc595_led.c src_std_hc595_led
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_hc595_led
 * \copydoc demo_std_hc595_led.c
 */

/** [src_std_hc595_led] */
#include "ametal.h"
#include "am_hc595.h"
#include "am_delay.h"
#include "am_vdebug.h"

/**
 * \brief 例程入口
 */
void demo_std_hc595_led_entry (am_hc595_handle_t hc595_handle)
{
    uint8_t buf = 0;
    int     i   = 0;

    while(1) {
        buf = ~(1ul << i);
        am_hc595_send(hc595_handle, &buf, 1); /* 点亮当前位置 LED 灯 （低电平点亮）*/
        am_mdelay(150);                       /* 延时 150ms */
        buf = 0xFF;
        am_hc595_send(hc595_handle, &buf, 1); /* 熄灭当前位置 LED 灯（高电平熄灭） */

        i = (i + 1) % 8;                      /* 切换到下一位置 */
    }
}
/** [src_std_hc595_led] */

/* end of file */
