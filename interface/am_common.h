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
 * \brief 通用工具宏定义头文件
 *
 * 本文件提供常用的工具宏，例如： min(), max(), offsetof()。更多内容
 * 请参考： \ref am_if_common 。
 *
 * \internal
 * \par Modification history:
 * - 1.03 14-12-15  fft, comment format and translate to english version
 * - 1.02 13-01-15  orz, move some defines to separate files.
 * - 1.01 12-09-05  orz, fix some defines.
 * - 1.00 12-08-23  zen, first implementation.
 * \endinternal
 */

#ifndef __AM_COMMON_H
#define __AM_COMMON_H

/**
 * \addtogroup am_if_common
 * \copydoc am_common.h
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus  */

/******************************************************************************/

/**
 * \name 通用常量定义
 * @{
 */

#define AM_OK               0         /**< \brief OK               */
#define AM_ERROR          (-1)        /**< \brief 一般错误         */

#define AM_NO_WAIT          0         /**< \brief 超时：不等待     */
#define AM_WAIT_FOREVER   (-1)        /**< \brief 超时：永远等待   */

#ifndef EOF
#define EOF               (-1)        /**< \brief 文件结束         */
#endif

#define NONE              (-1)        /**< \brief 每当为空时不执行 */
#define EOS               '\0'        /**< \brief C字符串结束      */

#ifndef NULL
#define NULL             ((void *)0)  /**< \brief 空指针           */
#endif

#define AM_LITTLE_ENDIAN  1234        /**< \brief 小端模式         */
#define AM_BIG_ENDIAN     3412        /**< \brief 大端模式         */

/** @} */

/******************************************************************************/

/**
 * \name 常用宏定义
 * @{
 */

/**
 * \brief 求结构体成员的偏移
 * \attention 不同平台上，由于成员大小和内存对齐等原因，
 *            同一结构体成员的偏移可能是不一样的
 *
 * \par 示例
 * \code
 *  struct my_struct = {
 *      int  m1;
 *      char m2;
 *  };
 *  int offset_m2;
 *
 *  offset_m2 = AM_OFFSET(struct my_struct, m2);
 * \endcode
 */
#define AM_OFFSET(structure, member)    ((size_t)(&(((structure *)0)->member)))

/** \brief 求结构体成员的偏移，同 \ref AM_OFFSET 一样 */
#ifndef offsetof
#define offsetof(type, member)           AM_OFFSET(type, member)
#endif

/**
 * \brief 通过结构体成员指针获取包含该结构体成员的结构体
 *
 * \param ptr    指向结构体成员的指针
 * \param type   结构体类型
 * \param member 结构体中该成员的名称
 *
 * \par 示例
 * \code
 *  struct my_struct = {
 *      int  m1;
 *      char m2;
 *  };
 *  struct my_struct  my_st;
 *  char             *p_m2 = &my_st.m2;
 *  struct my_struct *p_st = AM_CONTAINER_OF(p_m2, struct my_struct, m2);
 * \endcode
 */
#define AM_CONTAINER_OF(ptr, type, member) \
            ((type *)((char *)(ptr) - offsetof(type,member)))

/**
 * \brief 通过结构体成员指针获取包含该结构体成员的结构体，
 *        同 \ref AM_CONTAINER_OF 一样
 */
#ifndef container_of
#define container_of(ptr, type, member)     AM_CONTAINER_OF(ptr, type, member)
#endif

/** \brief 预取 */
#ifndef prefetch
#define prefetch(x) (void)0
#endif

/**
 * \brief 计算结构体成员的大小
 *
 * \code
 *  struct a = {
 *      uint32_t m1;
 *      uint32_t m2;
 *  };
 *  int size_m2;
 *
 *  size_m2 = AM_MEMBER_SIZE(a, m2);   //size_m2 = 4
 * \endcode
 */
#define AM_MEMBER_SIZE(structure, member)  (sizeof(((structure *)0)->member))

/**
 * \brief 计算数组元素个数
 *
 * \code
 *  int a[] = {0, 1, 2, 3};
 *  int element_a = AM_NELEMENTS(a);    // element_a = 4
 * \endcode
 */
#define AM_NELEMENTS(array)               (sizeof (array) / sizeof ((array) [0]))

/**
 * \brief 永久循环
 *
 * 写永远循环的语句块时大部分程序员喜欢用while(1)这样的语句，
 * 而有些喜欢用for(;;)这样的语句。while(1)在某些老的或严格的编译器上
 * 可能会出现警告，因为1是数字而不是逻辑表达式
 *
 * \code
 * AM_FOREVER {
 *     ; // 循环做一些事情或满足某些条件后跳出
 * }
 * \endcode
 */
#define AM_FOREVER  for (;;)

/******************************************************************************/

/**
 * \brief min 和 max 是 C++ 的标准函数，由 C++ 的库提供。
 */
#ifndef __cplusplus

/**
 * \brief 获取2个数中的较大的数值
 * \param x 数字1
 * \param y 数字2
 * \return 2个数中的较大的数值
 * \note 参数不能使用++或--操作
 */
#ifndef max
#define max(x, y)               (((x) < (y)) ? (y) : (x))
#endif

/**
 * \brief 获取2个数中的较小的数值
 * \param x 数字1
 * \param y 数字2
 * \return 2个数中的较小数值
 * \note 参数不能使用++或--操作
 */
#ifndef min
#define min(x, y)               (((x) < (y)) ? (x) : (y))
#endif

#endif /* __cplusplus */

/**
 * \brief 向上舍入
 *
 * \param x     被运算的数
 * \param align 对齐因素，必须为2的乘方
 *
 * \code
 * int size = AM_ROUND_UP(15, 4);   // size = 16
 * \endcode
 */
#define AM_ROUND_UP(x, align)   (((int) (x) + (align - 1)) & ~(align - 1))

/**
 * \brief 向下舍入
 *
 * \param x     被运算的数
 * \param align 对齐因素，必须为2的乘方
 *
 * \code
 * int size = AM_ROUND_DOWN(15, 4);   // size = 12
 * \endcode
 */
#define AM_ROUND_DOWN(x, align) ((int)(x) & ~(align - 1))

/** \brief 倍数向上舍入 */
#define AM_DIV_ROUND_UP(n, d)   (((n) + (d) - 1) / (d))

/**
 * \brief 测试是否对齐
 *
 * \param x     被运算的数
 * \param align 对齐因素，必须为2的乘方
 *
 * \code
 * if (AM_ALIGNED(x, 4) {
 *     ; // x对齐
 * } else {
 *     ; // x不对齐
 * }
 * \endcode
 */
#define AM_ALIGNED(x, align)    (((int)(x) & (align - 1)) == 0)

/******************************************************************************/

/** \brief 把符号转换为字符串 */
#define AM_STR(s)               #s

/** \brief 把宏展开后的结果转换为字符串 */
#define AM_XSTR(s)              AM_STR(s)

/******************************************************************************/

/** \brief 将1字节BCD数据转换为16进制数据 */
#define AM_BCD_TO_HEX(val)     (((val) & 0x0f) + ((val) >> 4) * 10)

/** \brief 将1字节16进制数据转换为BCD数据 */
#define AM_HEX_TO_BCD(val)     ((((val) / 10) << 4) + (val) % 10)

/** @} */


/** 
 * @}  
 */
#include "am_bitops.h"
#include "am_byteops.h"
#include "am_errno.h"
#include "am_types.h"
#include "am_system.h"
#include "am_assert.h"

#ifdef __cplusplus
}
#endif    /* __cplusplus */

#endif /* __AM_COMMON_H */

/* end of file */
