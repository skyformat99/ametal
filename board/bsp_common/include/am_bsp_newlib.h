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
 * \brief newlib适配器板级支持
 *
 * \internal
 * \par modification history
 * - 1.00 12-10-23  orz, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_uart.h"

#ifndef __AM_BSP_NEWLIB_H
#define __AM_BSP_NEWLIB_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief newlib 初始化，使用标准库函数前，必须调用该函数
 */
void am_bsp_newlib_init (am_uart_handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __AM_BSP_NEWLIB_H */

/* end of file */
