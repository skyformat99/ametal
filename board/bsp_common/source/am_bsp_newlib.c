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
 * \brief newlib  ≈‰∆˜∞Âº∂÷ß≥÷
 *
 * \internal
 * \par modification history
 * - 1.00 12-10-23  orz, first implementation
 * \endinternal
 */

#include "ametal.h"
#include "am_vdebug.h"
#include "am_newlib_adapter.h"

static int __console_send (void *p_cookie, const void *p_buf, size_t cnt)
{
    return am_uart_poll_send(p_cookie, p_buf, cnt);
}

static int __console_receive (void *p_cookie, void *p_buf, size_t cnt)
{
    return am_uart_poll_receive(p_cookie, p_buf, cnt);
}

static const am_newlib_adapter_ops_t  __g_newlib_adapter_ops = {
    __console_send,
    __console_receive
};

void am_bsp_newlib_init (am_uart_handle_t handle)
{
    if (handle != NULL) {
        am_newlib_adapter_init(&__g_newlib_adapter_ops, handle);
    } else {
        am_newlib_adapter_init(NULL, NULL);
    }
}

/* end of file */
