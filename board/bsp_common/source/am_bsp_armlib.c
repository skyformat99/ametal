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
 * \brief ARM LIB
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-17  pea, first implementation.
 * \endinternal
 */

#ifndef __MICROLIB

#include "ametal.h"
#include "am_bsp_armlib.h"
#include "am_armlib_adapter.h"

/**
 * \brief 输出一个字符
 *
 * \param[in] p_arg UART标准服务操作句柄
 * \param[in] ch    需要输出的字符
 *
 * \return 被输出的字符
 */
static int __dbg_fputc (void *p_arg, const char ch)
{
    while (-AM_EAGAIN == am_uart_poll_putchar(p_arg, ch)) {
        ;
    }

    return ch;
}

/**
 * \brief 输入一个字符
 *
 * \param[in] p_arg UART标准服务操作句柄
 *
 * \return 输入的字符。否则返回EOF
 */
static int __dbg_fgetc (void *p_arg)
{
    char ch;

    while (-AM_EAGAIN == am_uart_poll_getchar(p_arg, &ch)) {
        ;
    }

    return ch;
}

/**
 * \brief 初始化ARM LIB适配器结构
 */
static am_armlib_adapter_ops_t g_armlib_adapter_ops = {
    __dbg_fputc,
    __dbg_fgetc
};

/**
 * \brief ARM LIB初始化
 *
 * \param[in] uart_handle UART服务
 *
 * \return AM_OK
 */
int am_bsp_armlib_init (am_uart_handle_t uart_handle)
{
    return am_armlib_adapter_init(&g_armlib_adapter_ops, uart_handle);
}

#endif /* __MICROLIB */

/* end of file */
