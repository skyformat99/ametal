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
 * \brief 字节操作相关定义头文件
 *
 * 本文件提供了常用的位操作工具宏，例如：AM_LSB(), AM_LONGSWAP()。更多内容
 * 请参考 \ref am_if_byteops 。
 *
 * \internal
 * \par Modification history:
 * - 1.01 14-12-15  fft, comment formate and translate to english version
 * - 1.00 13-01-15  orz, move defines from am_common.h
 * \endinternal
 */

#ifndef __AM_BYTEOPS_H
#define __AM_BYTEOPS_H

/**
 * \addtogroup am_if_byteops
 * \copydoc am_byteops.h
 * @{
 */

/**
 * \brief 取2-byte整数的高位byte
 *
 * \par 示例
 * \code
 *  uint16_t a = 0x1234;
 *  uint16_t b;
 *
 *  b = AM_MSB(a);  //b=0x12
 * \endcode
 */
#define AM_MSB(x)       (((x) >> 8) & 0xff)

/**
 * \brief 取2-byte整数的低位byte
 *
 * \par 示例
 * \code
 *  uint16_t a = 0x1234;
 *  uint16_t b;
 *
 *  b = AM_LSB(a);  //b=0x34
 * \endcode
 */
#define AM_LSB(x)       ((x) & 0xff)

/**
 * \brief 取2-word整数的高位word
 *
 * \par 示例
 * \code
 *  uint32_t a = 0x12345678;
 *  uint32_t b;
 *
 *  b = AM_MSW(a);  //b=0x1234
 * \endcode
 */
#define AM_MSW(x)       (((x) >> 16) & 0xffff)

/**
 * \brief 取2-word整数的低位word
 *
 * \par 示例
 * \code
 *  uint32_t a = 0x12345678;
 *  uint32_t b;
 *
 *  b = AM_LSW(a);  //b=0x5678
 * \endcode
 */
#define AM_LSW(x)       ((x) & 0xffff)

/**
 * \brief 交换32-bit整数的高位word和低位word
 *
 * \par 示例
 * \code
 *  uint32_t a = 0x12345678;
 *  uint32_t b;
 *
 *  b = AM_WORDSWAP(a); //b=0x56781234
 * \endcode
 */
#define AM_WORDSWAP(x)  (AM_MSW(x) | (AM_LSW(x) << 16))

/**
 * \brief 交换32-bit整数的字节顺序
 *
 * \par 示例
 * \code
 *  uint32_t a = 0x12345678;
 *  uint32_t b;
 *
 *  b = AM_LONGSWAP(a); //b=0x78563412
 * \endcode
 */
#define AM_LONGSWAP(x)  ((AM_LLSB(x) << 24) | \
                        (AM_LNLSB(x) << 16) | \
                        (AM_LNMSB(x) << 8)  | \
                        (AM_LMSB(x)))

#define AM_LLSB(x)      ((x) & 0xff)            /**< \brief 取32bit整数第1个字节 */
#define AM_LNLSB(x)     (((x) >> 8) & 0xff)     /**< \brief 取32bit整数第2个字节 */
#define AM_LNMSB(x)     (((x) >> 16) & 0xff)    /**< \brief 取32bit整数第3个字节 */
#define AM_LMSB(x)      (((x) >> 24) & 0xff)    /**< \brief 取32bit整数第4个字节 */

/** 
 * @}  
 */

#endif /* __AM_BYTEOPS_H */

/* end of file */
