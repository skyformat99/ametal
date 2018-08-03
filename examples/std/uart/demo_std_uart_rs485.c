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
 * \brief RS485 例程（使用UART标准接口）
 *
 * - 实验现象：
 *   1. 串口输出"RS485 Test:"；
 *   2. 串口输出接收到的字符串。
 *
 *   RS485为半双工通信，因此从双方必须定义好合适的发送实际，避免冲突。本例程
 *   作为一种简单的范例，设计为收到一个字符就回发一个字符，因此，发送端只能单
 *   个单个字符的发送，即发送一个字符后接收到回传的字符再传输下一个字符。
 *
 * \par 源代码
 * \snippet demo_std_uart_rs485.c src_std_uart_rs485
 *
 * \internal
 * \par Modification History
 * - 1.00 17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_uart_rs485
 * \copydoc demo_std_uart_rs485.c
 */

/** [src_std_uart_rs485] */
#include "ametal.h"
#include "am_uart.h"

/**
 * \brief 例程入口
 */
void demo_std_uart_rs485_entry (am_uart_handle_t uart_handle)
{
    uint8_t  rcv_tmp;           /* 接收数据暂存器 */
    int      rcv_cnt;

    /**
     * 使能 485 模式，会自动控制方向引脚
     */
    am_uart_ioctl(uart_handle, AM_UART_RS485_SET, (void *)AM_TRUE);
    am_uart_ioctl(uart_handle, AM_UART_MODE_SET, (void *)AM_UART_MODE_POLL);

    am_uart_poll_send(uart_handle, (const uint8_t *)"\r\n RS485 Test:\r\n",
                                   sizeof("\r\n RS485 Test:\r\n") - 1);
    while (1) {

        /* 接收字符 */
        rcv_cnt = am_uart_poll_receive(uart_handle, &rcv_tmp, 1);
        if (rcv_cnt) {
            am_uart_poll_send(uart_handle, &rcv_tmp, 1);
        }
    }
}
/** [src_std_uart_rs485] */

/* end of file */
