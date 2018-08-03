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
 * \brief 软件定时器例程，通过标准接口实现
 *
 * - 实验现象：
 *   1. 软件定时器产生 100ms 周性期中断，并在中断时打印调试信息。
 *
 * \note
 *    1. 测试本 Demo 必须在 am_prj_config.h 内将 AM_CFG_SOFTIMER_ENABLE 定义为 1。
 *       通常该宏已经默认配置为 1；
 *
 * \par 源代码
 * \snippet demo_std_softimer.c src_std_softimer
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-27  nwt, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_softimer
 * \copydoc demo_std_softimer.c
 */

/** [src_std_softimer] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_softimer.h"

/**
 * \brief 定时器回调函数
 */
static void softimer_callback (void *p_arg)
{
    AM_DBG_INFO("softimer irq!\r\n");
}

/**
 * \brief 例程入口
 */
void demo_std_softimer_entry (void)
{
    am_softimer_t softimer;

    am_softimer_init(&softimer, softimer_callback, NULL);
    am_softimer_start(&softimer, 100);                    /* 定时时间：100ms  */

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_softimer] */

/* end of file */
