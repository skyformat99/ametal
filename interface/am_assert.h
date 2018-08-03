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
 * \brief 断言的简单实现
 *
 * 使用本服务需要包含头文件 am_assert.h
 *
 * // 更多内容待添加。。。
 *
 * \internal
 * \par modification history
 * - 1.00 16-10-27  tee, first implementation
 * \endinternal
 */

#ifndef __AM_ASSERT_H
#define __AM_ASSERT_H

/*lint ++flb */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_assert
 * \copydoc am_assert.h
 * @{
 */

#ifdef AM_DEBUG

/* help macro */
#define _AM_VTOS(n)     #n          /* Convert n to string */
#define _AM_SVAL(m)     _AM_VTOS(m) /* Get string value of macro m */

/**
 * \brief 断言一个表达式
 *
 * 当表达式为真时这个宏不做任何事情，为假时会调用am_assert_msg()输出断言信息，
 * 断言信息格式为：
 * 文件名：行号：表达式
 * \hideinitializer
 */
#define am_assert(e) \
    ((e) ? (void)0 : am_assert_msg(__FILE__":"_AM_SVAL(__LINE__)":("#e")\n"))

extern void am_assert_msg (const char *msg);

#else

/**
 * \brief 断言一个表达式
 *
 * 当表达式为真时这个宏不做任何事情，为假时会调用am_assert_msg()输出断言信息，
 * 断言信息格式为：
 * 文件名：行号：表达式
 * \hideinitializer
 */
#define am_assert(e)    ((void)0)
#endif

#ifdef __cplusplus
}
#endif

/** @}  am_if_assert */

#endif /* __AM_ASSERT_H */

/* end of file */
