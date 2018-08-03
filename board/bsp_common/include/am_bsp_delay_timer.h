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
 * \brief 基于通用定时器实现的延时函数
 *
 * \internal
 * \par Modification history
 * - 1.00 17-11-22  pea, first implementation
 * \endinternal
 */
#ifndef __AM_BSP_DELAY_TIMER_H
#define __AM_BSP_DELAY_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_timer.h"

/**
 * \brief 延时函数初始化
 *
 * 延时通过获取当前count值实现，不会对定时器本身作任何改变，因此，用于延时的定时
 * 器通常可以用作其它功能，例如，用于开启 1KHz 的周期性定时中断，用作系统节拍。
 *
 * 该延时阶段不会关闭中断，因此，若延时期间产生了中断，中断会正常执行，且可能
 * 导致延时时间延长。
 */
void am_bsp_delay_timer_init (am_timer_handle_t timer_handle, uint8_t chan);

#ifdef __cplusplus
}
#endif

#endif /* __AM_BSP_DELAY_TIMER_H */

/* end of file */
