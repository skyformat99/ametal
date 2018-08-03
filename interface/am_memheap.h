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
 * \brief 堆管理器
 *
 * \internal
 * \par modification history:
 * - 1.00 16-10-27  tee, copy from amorks
 * \endinternal
 */

#ifndef __AM_MEMHEAP_H
#define __AM_MEMHEAP_H

#include "ametal.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus	*/

/**
 * \addtogroup am_if_memheap
 * \copydoc am_memheap.h
 * @{
 */

/**
 * \brief memory item on the memory heap
 */
typedef struct am_memheap_item
{
    uint32_t                magic;           /**< magic number for memheap */
    struct am_memheap      *pool_ptr;        /**< point of pool            */

    struct am_memheap_item *next;            /**< next memheap item        */
    struct am_memheap_item *prev;            /**< prev memheap item        */

    struct am_memheap_item *next_free;       /**< next free memheap item   */
    struct am_memheap_item *prev_free;       /**< prev free memheap item   */
} am_memheap_item_t;

/**
 * \brief memory heap
 */
typedef struct am_memheap
{
    const char             *name;               /**< pool name */
    void                   *start_addr;         /**< pool start address and size */

    uint32_t                pool_size;          /**< pool size */
    uint32_t                available_size;     /**< available size */
    uint32_t                max_used_size;      /**< maximum allocated size */

    struct am_memheap_item *block_list;         /**< used block list */

    struct am_memheap_item *free_list;          /**< free block list */
    struct am_memheap_item  free_header;        /**< free block list header */
 
} am_memheap_t;

/**
 * \brief 初始化一个堆管理器
 *
 * \param[in] memheap    ：指向待初始化的堆管理器
 * \param[in] name       : 堆管理器的名字
 * \param[in] start_addr : 该堆管理器管理内存空间的起始地址
 * \param[in] size       : 该堆管理器管理内存空间的大小（字节数）
 *
 * \retval AM_OK : 初始化成功
 * \retval  < 0  : 初始化失败
 */
am_err_t am_memheap_init(struct am_memheap *memheap,
                         const char        *name,
                         void              *start_addr,
                         uint32_t           size);
 
/**
 * \brief 从堆中分配空间
 *
 * \param[in] heap   ：指向堆管理器
 * \param[in] size   : 分配空间的大小
 *
 * \return 分配空间的首地址，NULL表明分配失败
 */
void *am_memheap_alloc(struct am_memheap *heap, uint32_t size);

/**
 * \brief 获取分配空间的大小
 *
 * \param[in] heap   ：指向堆管理器
 * \param[in] ptr    : 分配空间的首地址
 *
 * \return 分配空间的大小（字节数）
 */
size_t am_memheap_memsize(struct am_memheap *heap, void *ptr);

/**
 * \brief 动态内存调整，改变已分配空间的大小
 *
 * \param[in] heap    ：指向堆管理器
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
void *am_memheap_realloc(struct am_memheap *heap, void *ptr, size_t newsize);

/**
 * \brief 释放动态分配的堆内存
 * \param[in] ptr : 分配的空间的首地址
 * \return 无
 */
void am_memheap_free(void *ptr);

/** @}  am_if_memheap */

#ifdef __cplusplus
}
#endif	/* __cplusplus 	*/

#endif /* __AM_MEMHEAP_H    */

/* end of file */
