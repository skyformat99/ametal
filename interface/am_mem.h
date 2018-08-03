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
 * \brief 内存管理标准接口
 *
 * 使用本服务需要包含以下头文件：
 * \code
 * #include "am_mem.h"
 * \endcode
 * 本服务提供了动态内存管理的接口
 *
 * // 更多内容待添加。。。
 *
 * \internal
 * \par modification history:
 * - 1.00 16-10-27  tee, first implementation
 * \endinternal
 */

#ifndef __AM_MEM_H
#define __AM_MEM_H

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus	*/

#include "am_memheap.h"

/**
 * \addtogroup am_if_mem
 * \copydoc am_mem.h
 * @{
 */

/**
 * \brief 分配一段满足align字节对齐的内存空间
 *
 * \param[in] size    ：分配空间的大小
 * \param[in] align   : 指定分配空间对齐的字节数（首地址将按照align字节对齐）
 *
 * \return 分配空间的首地址，NULL表明分配失败
 */
void *am_mem_align(size_t size, size_t align);

/**
 * \brief 分配一段内存空间
 * \param[in] size ：分配空间的大小
 * \return 分配空间的首地址，NULL表明分配失败
 */
void *am_mem_alloc(size_t size);

/**
 * \brief 分配一段内存空间（内存空间会被初始化为0）
 *
 * \param[in] nelem ：元素个数
 * \param[in] size  : 一个元素的大小，分配内存空间的大小即为  nelem * size
 *
 * \return 分配空间的首地址，NULL表明分配失败
 *
 * \note 分配的内存空间会被初始化为0
 */
void *am_mem_calloc(size_t nelem, size_t size);

/**
 * \brief 释放已分配的内存空间
 * \param[in] ptr ：已分配的内存空间的首地址
 * \return 无
 */
void am_mem_free(void *ptr);

/**
 * \brief 获取分配空间的大小
 * \param[in] ptr    : 分配空间的首地址
 * \return 分配空间的大小（字节数）
 */
size_t am_mem_size(void *ptr);

/**
 * \brief 动态内存调整，改变已分配空间的大小
 *
 * \param[in] ptr     : 已分配空间的首地址
 * \param[in] newsize : 重新分配的空间的大小
 *
 * \return 重新分配空间的首地址，NULL表明分配失败
 *
 * \note 函数先判断当前的指针是否有足够的连续空间，如果有，扩大 ptr 指向地址的
 * 空间，直接将ptr的值返回。如果空间不够，先按照new_size指定的大小分配空间，将
 * 原有数据从头到尾拷贝到新分配的内存区域，而后释放原来ptr所指内存区域（原来
 * 指针自动释放，不需要 am_memheap_free()），同时返回新分配的内存区域的首地址。
 */
void *am_mem_realloc(void *ptr, size_t newsize);

/** @}  am_if_mem */

#ifdef __cplusplus
}
#endif	/* __cplusplus 	*/

#endif	/* __AM_MEM_H */

/* end of file */

