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
 * \brief 可视化调试标准接口
 *
 * 使用本服务需要包含头文件 am_vdebug.h
 *
 * \par 简单示例
 * \code
 *  #include "ametal.h"
 *  #include "am_vdebug.h"
 *
 *  int age = 18;
 *  am_kprintf("I'm %d years old!\n", age);
 * \endcode
 *
 * \internal
 * \par modification history:
 * - 1.00 15-01-16  tee, first implementation
 * \endinternal
 */

#ifndef __AM_VDEBUG_H
#define __AM_VDEBUG_H

#include <stdarg.h>
#include <stdlib.h>
#include "am_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_vdebug
 * \copydoc am_vdebug.h
 * @{
 */

/**
 * \brief 如果定义了AM_VDEBUG宏,则 AM_DBG_INFO() 宏将输出相关信息，否则
 *        AM_DBG_INFO()宏实际为空
 */
#ifdef AM_VDEBUG
#define AM_DBG_INFO(...)    (void)am_kprintf(__VA_ARGS__)
#else
#define AM_DBG_INFO(...)
#endif

/** \brief format log message and output */
#ifdef AM_VDEBUG
#define AM_LOGF(msg)    (void)am_kprintf msg
#else
#define AM_LOGF(msg)
#endif

/** \brief format information message */
#ifdef AM_VDEBUG_INFO
#define AM_INFOF(info)  AM_LOGF(info)
#else
#define AM_INFOF(info)
#endif

/** \brief format information message */
#ifdef AM_VDEBUG_DEBUG
#define AM_DBGF(info)   AM_LOGF(info)
#else
#define AM_DBGF(info)
#endif

/** \brief format warning message */
#ifdef AM_VDEBUG_WARN
#define AM_WARNF(info)  AM_LOGF(info)
#else
#define AM_WARNF(info)
#endif

/** \brief format error message */
#ifdef AM_VDEBUG_ERROR
#define AM_ERRF(info)   AM_LOGF(info)
#else
#define AM_ERRF(info)
#endif

/**
 * \brief 调试初始化
 * \param[in] handle   : 标准的串口服务Handle, 调试信息输出至该串口
 * \param[in] baudrate : 使用的波特率
 * \return 无
 * \note 在使用调试输出信息前，确保调试串口已调用该函数初始化 
 */
void am_debug_init (am_uart_handle_t handle, uint32_t baudrate);

/**
 * \brief 格式化字符串并输出
 *
 * 该函数是 vXXXprintf() 函数的基础函数，其它所有 vXXXprintf() 均基于此函数。
 *
 * \param[in] fil     :  句柄，作为 f_putc 和 f_puts 函数的第二个参数
 * \param[in] f_putc  :  函数指针，指向的函数用于输出一个字符
 * \param[in] f_puts  :  函数指针，指向的函数用于输出一个字符串
 * \param[in] str     :  格式化字符串
 * \param[in] arp     :  格式化字符串的参数
 *
 * \return 实际输出的字符个数
 */
int am_vfprintf_do (void       *fil,
                    int       (*f_putc) (const char  c, void *fil),
                    int       (*f_puts) (const char *s, void *fil),
                    const char *str,
                    va_list     arp);

/**
 * \brief 格式化字符串到一个指定长度的字符串缓冲区中
 *
 * \param[in] buf  : 保存格式化字符串的缓冲区
 * \param[in] sz   : 缓冲区大小
 * \param[in] fmt  : 格式化字符串
 * \param[in] args : 格式化字符串的参数
 *
 * \return 实际输入缓冲区字符的个数
 */
int am_vsnprintf (char *buf, size_t sz, const char *fmt, va_list args);

/**
 * \brief 格式化字符串到一个指定长度的字符串缓冲区中
 *
 * \param[in] buf  : 保存格式化字符串的缓冲区
 * \param[in] sz   : 缓冲区大小
 * \param[in] fmt  : 格式化字符串
 * \param[in] ...  : 格式化字符串的参数列表
 *
 * \return 实际输入缓冲区字符的个数
 *
 * \par 示例
 * \code
 *  #include "ametal.h"
 *  #include "am_vdebug.h"
 *  #include <string.h>
 *
 *  char buf[64];
 *  int age = 18;
 *  am_snprintf(&buf[0], sizeof(buf), "I'm %d years old!\n", age);
 * \endcode
 */
#ifndef AM_SNPRINTF_DEFINED
int am_snprintf (char *buf, size_t sz, const char *fmt, ...);
#endif  /* AM_SNPRINTF_DEFINED */

/**
 * \brief 内核格式化打印函数
 *
 * 本函数将格式化字符串输出到当前的控制台。
 *
 * \param[in] fmt : 格式化字符串
 * \param[in] ... : 格式化字符串的参数列表
 *
 * \return 输出到控制台的字符个数
 *
 * \par 示例
 * \code
 *  #include "ametal.h"
 *  #include "am_vdebug.h"
 *
 *  int age = 18;
 *  am_kprintf("I'm %d years old!\n", age);
 * \endcode
 */
#ifndef AM_KPRINTF_DEFINED
int am_kprintf (const char *fmt, ...);
#endif  /* AM_KPRINTF_DEFINED */

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_VDEBUG_H */

/* end of file */
