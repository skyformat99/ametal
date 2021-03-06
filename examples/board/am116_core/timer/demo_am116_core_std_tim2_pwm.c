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
 * \brief TIM2 定时器 PWM 例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. PIOA_0(TIM2_CH1)输出 4KHz 的 PWM，占空比为 50%；
 *
 * \par 源代码
 * \snippet demo_am116_core_std_tim2_pwm.c src_am116_core_std_tim2_pwm
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-22  nwt, first implementation
 * \endinternal
 */ 

/**
 * \addtogroup demo_if_am116_core_std_tim2_pwm
 * \copydoc demo_am116_core_std_tim2_pwm.c
 */

/** [src_am116_core_std_tim2_pwm] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg116_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am116_core_std_tim2_pwm_entry (void)
{
    AM_DBG_INFO("demo am116_core std tim2 pwm!\r\n");

    demo_std_timer_pwm_entry(am_zlg116_tim2_pwm_inst_init(), 3);
}
/** [src_am116_core_std_tim2_pwm] */

/* end of file */
