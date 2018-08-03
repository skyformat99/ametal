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
 * \brief NVIC标准接口
 *
 * \internal
 * \par Modification history
 * - 1.00 14-12-04  hbt, first implementation.
 * \endinternal
 */

#ifndef __AM_INT_H
#define __AM_INT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/** 
 * \addtogroup am_if_int
 * \copydoc am_int.h
 * @{
 */

/**
 * \brief 中断连接
 * 
 * \param[in] inum    : 中断号
 * \param[in] pfn_isr : 中断回调函数指针
 * \param[in] p_arg   : 回调函数的入口参数
 * 
 * \retval  AM_OK     : 连接成功
 * \retval -AM_EINVAL : 无效参数
 * \retval -AM_EPERM  : 操作失败
 */
int am_int_connect(int inum, am_pfnvoid_t pfn_isr, void *p_arg);

/**
 * \brief 删除中断连接
 * 
 * \param[in] inum    : 中断号
 * \param[in] pfn_isr : 中断回调函数指针
 * \param[in] p_arg   : 回调函数的入口参数
 * 
 * \retval  AM_OK     : 删除连接成功
 * \retval -AM_EINVAL : 无效参数
 * \retval -AM_EPERM  : 操作失败
 */
int am_int_disconnect(int inum, am_pfnvoid_t pfn_isr, void *p_arg);

/**
 * \brief 使能中断
 * \param[in] inum  : 中断号
 * \retval    AM_OK : 操作成功
 */
int am_int_enable(int inum);

/**
 * \brief 禁能中断
 * \param[in] inum  : 中断号
 * \retval    AM_OK : 操作成功
 */
int am_int_disable(int inum);

/**
 * \brief 关闭本地CPU中断
 *
 * \attention
 * - 关闭本地CPU中断的时间一定要短
 * - 必须与 am_int_cpu_unlock() 函数搭配使用
 *
 * \return 中断关闭信息
 *
 * \par 示例：
 * \code
 * uint32_t key;
 * 
 * key = am_int_cpu_lock();
 * // do something.
 * am_int_cpu_unlock(key);
 * \endcode
 *
 * \par 嵌套使用示例：
 * \code 
 * uint32_t key0, key1;
 *
 * key0 = am_int_cpu_lock();  // 中断马上关闭
 * key1 = am_int_cpu_lock();  // 中断保持关闭
 * 
 * am_int_cpu_unlock(key1);   // 中断保持关闭
 * am_int_cpu_unlock(key0);   // 中断马上打开
 * \endcode
 */
uint32_t am_int_cpu_lock(void);

/**
 * \brief 打开本地CPU中断
 *
 * \attention
 *  必须与 am_int_cpu_lock() 函数搭配使用
 *
 * \param[in] key : 中断关闭信息
 * \return    无
 */
void am_int_cpu_unlock(uint32_t key);

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_INT_H */

/* end of file */
