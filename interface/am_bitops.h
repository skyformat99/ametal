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
 * \brief 位操作相关定义头文件
 *
 * 该文件提供了常用的位操作工具宏，例如：AM_BIT_SET()， AM_BIT_GET()，更多内容
 * 请参考 \ref am_if_bitops 。
 *
 * \internal
 * \par Modification history:
 * - 1.01 14-12-15  fft, comment formate and translate to English version
 * - 1.00 13-01-15  orz, move defines from am_common.h
 * \endinternal
 */

#ifndef __AM_BITOPS_H
#define __AM_BITOPS_H

/**
 * \addtogroup am_if_bitops
 * \copydoc am_bitops.h
 * @{
 */

/******************************************************************************/

/**
 * \name 通用位常数定义
 * @{
 */

/** \brief 长整数位数 */
#ifndef AM_BITS_PER_LONG
#define AM_BITS_PER_LONG       32
#endif

/** \brief 字节位数 */
#define AM_BITS_PER_BYTE       8

/** @} */

/******************************************************************************/

/**
 * \name 通用位操作
 * @{
 */

#ifndef AM_BIT_MASK

/** \brief 计算n bits掩码 */
#define AM_BIT_MASK(nr)              (1UL << ((nr) % BITS_PER_LONG))
#endif

#ifndef AM_BIT_WORD

/** \brief bits转换为字个数 */
#define AM_BIT_WORD(nr)              ((nr) / BITS_PER_LONG)
#endif

#ifndef AM_BITS_TO_LONGS

/** \brief bit在long型bit map中的索引 */
#define AM_BITS_TO_LONGS(nr)         AM_DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#endif

/** \brief bit移位 */
#define AM_BIT(bit)                  (1u << (bit))

/** \brief 值移位 */
#define AM_SBF(value, field)         ((value) << (field))

/** \brief bit置位 */
#define AM_BIT_SET(data, bit)        ((data) |= AM_BIT(bit))

/** \brief bit清零 */
#define AM_BIT_CLR(data, bit)        ((data) &= ~AM_BIT(bit))

/** \brief bit置位, 根据 mask 指定的位 */
#define AM_BIT_SET_MASK(data, mask)  ((data) |= (mask))

/** \brief bit清零, 根据 mask 指定的位 */
#define AM_BIT_CLR_MASK(data, mask)  ((data) &= ~(mask))

/** \brief bit翻转 */
#define AM_BIT_TOGGLE(data, bit)     ((data) ^= AM_BIT(bit))

/** \brief bit修改 */
#define AM_BIT_MODIFY(data, bit, value) \
    ((value) ? AM_BIT_SET(data, bit) : AM_BIT_CLR(data, bit))

/** \brief 测试bit是否置位 */
#define AM_BIT_ISSET(data, bit)      ((data) & AM_BIT(bit))

/** \brief 获取bit值 */
#define AM_BIT_GET(data, bit)        (AM_BIT_ISSET(data, bit) ? 1 : 0)

/** \brief 获取 n bits 掩码值 */
#define AM_BITS_MASK(n)              (~((~0u) << (n)))

/** \brief 获取位段值 */
#define AM_BITS_GET(data, start, len)  \
    (((data) >> (start)) & AM_BITS_MASK(len))

/** \brief 设置位段值 */
#define AM_BITS_SET(data, start, len, value) \
    ((data) = (((data) & ~AM_SBF(AM_BITS_MASK(len), (start))) | \
        AM_SBF((value) & (AM_BITS_MASK(len)), (start))))

/** @} */ 

/**
 * @}  
 */

#endif /* __AM_BITOPS_H */

/* end of file */
