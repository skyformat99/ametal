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
 * \brief 定时器实现捕获功能，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 PWM 输出连接到捕获输入引脚。
 *
 * - 实验现象：
 *   1. 调试串口输出捕获到的 PWM 信号的周期和频率。
 *
 * \par 源代码
 * \snippet demo_std_timer_cap.c src_std_timer_cap
 *
 * \internal
 * \par Modification history
 * - 1.00 16-04-22  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_std_timer_cap
 * \copydoc demo_std_timer_cap.c
 */

/** [src_std_timer_cap] */
#include "ametal.h"
#include "am_cap.h"
#include "am_vdebug.h"

static volatile am_bool_t __g_flag    = AM_FALSE;    /**< \brief 捕获标志 */
static volatile uint32_t  __g_time_ns = 0;           /**< \brief 捕获计数值 */

/**
 * \brief 捕获回调函数
 */
static void __cap_callback (void *p_arg, unsigned int cap_val)
{
    static uint32_t  s_count    = 0;
    static am_bool_t s_first    = AM_TRUE;
    am_cap_handle_t  cap_handle = (am_cap_handle_t)p_arg;

    if (__g_flag == AM_FALSE) {

        if (s_first == AM_TRUE) {
            s_count = cap_val;
            s_first  = AM_FALSE;
        } else {

            /* 定时器TIM不是32位计数器时, 避免溢出时数据错误 */
            if(s_count < cap_val) {
               am_cap_count_to_time(cap_handle,
                                    0,
                                    s_count,
                                    cap_val,
                                    (unsigned int *)&__g_time_ns);
            }

            s_first = AM_TRUE;

            /* 置标志为真，表明捕获完成 */
            __g_flag = AM_TRUE;
        }
    }
}

/**
 * \brief 例程入口
 */
void demo_std_timer_cap_entry (am_cap_handle_t cap_handle, int cap_chan)
{
    uint32_t freq = 0;

    /* 捕获输入配置 */
    am_cap_config(cap_handle,
                  cap_chan,
                  AM_CAP_TRIGGER_RISE,
                  __cap_callback,
                  (void *)cap_handle);
    am_cap_enable(cap_handle, cap_chan);

    while (1) {
        if (__g_flag == AM_TRUE) {
            freq = 1000000000 / __g_time_ns;

            AM_DBG_INFO("The period is %d ns, The freq is %d Hz \r\n",
                         __g_time_ns,
                         freq);

            __g_flag = AM_FALSE;
        }
    }
}
/** [src_std_timer_cap] */

/* end of file */
