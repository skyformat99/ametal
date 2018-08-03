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
 * \brief 蜂鸣器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 蜂鸣器以 0.5s 的时间间隔鸣叫。
 *
 * \note
 *    测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_BUZZER_ENABLE 定义为 1，
 *    通常该宏已经默认配置为 1。
 *
 * \par 源代码
 * \snippet demo_std_buzzer.c src_std_buzzer
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-20  win, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_buzzer
 * \copydoc demo_std_buzzer.c
 */

/** [src_std_buzzer] */
#include "ametal.h"
#include "am_delay.h"
#include "am_buzzer.h"

/**
 * \brief 例程入口
 */
void demo_std_buzzer_entry (void)
{
    am_buzzer_on();
    am_mdelay(500);
    am_buzzer_off();
    am_mdelay(500);

    AM_FOREVER {

        am_buzzer_beep(500);  /* 蜂鸣器鸣叫 0.5s */
        am_mdelay(500);
    }
}
/** [src_std_buzzer] */

/* end of file */
