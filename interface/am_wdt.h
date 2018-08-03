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
 * \brief  WDT标准接口
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-01-07  fft, first implementation.
 * \endinternal
 */

#ifndef __AM_WDT_H
#define __AM_WDT_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
    
/** 
 * \addtogroup am_if_wdt
 * \copydoc am_wdt.h
 * @{
 */

/** 
 * \brief WDT超时信息
 */
typedef struct am_wdt_info {
    uint32_t min_timeout_ms;  /**< \brief 支持的最大超时时间，单位：ms */
    uint32_t max_timeout_ms;  /**< \brief 支持的最小超时时间，单位：ms */
} am_wdt_info_t;

/** 
 * \brief WDT驱动函数
 */
struct am_wdt_drv_funcs {
    
    /** \brief 获取WDT信息 */
    int (*pfn_wdt_info_get)(void *p_drv, am_wdt_info_t *p_info);

    /** \brief 设置超时时间并使能WDT */
    int (*pfn_wdt_enable)(void *p_drv, uint32_t timeout_ms);
    
    /** \brief 看门狗喂狗 */
    int (*pfn_wdt_feed)(void *p_drv);
};

/** 
 * \brief WDT标准服务
 */
typedef struct am_wdt_serv {

    /** \brief WDT驱动函数 */
    struct am_wdt_drv_funcs  *p_funcs; 
    
    /** \brief 驱动函数第一个入口参数 */
    void                     *p_drv; 
} am_wdt_serv_t;

/** \brief WDT标准服务操作句柄类型 */
typedef am_wdt_serv_t *am_wdt_handle_t;


/**
 * \brief 获取WDT信息
 *
 * \param[in] handle  : WDT标准服务操作句柄
 * \param[out] p_info : 指向WDT信息的指针
 *
 * \retval AM_OK      : 操作成功
 * \retval -AM_EINVAL : 无效参数
 */
am_static_inline
int am_wdt_info_get (am_wdt_handle_t handle, am_wdt_info_t *p_info)
{
    return handle->p_funcs->pfn_wdt_info_get(handle->p_drv,p_info);
}

/**
 * \brief 设置超时时间并使能WDT
 *
 * \param[in] handle     : WDT标准服务操作句柄
 * \param[in] timeout_ms : 超时时间值，单位：ms
 *
 * \retval  AM_OK        : 设置超时时间成功
 * \retval -AM_EINVAL    : 参数无效，检查设定超时时间是否在允许范围内
 *
 * \note 如果设定的超时时间值不为0，WDT将使能。若超时不喂狗，会使芯片复位
 */
am_static_inline
int am_wdt_enable (am_wdt_handle_t handle, uint32_t timeout_ms)
{
    return handle->p_funcs->pfn_wdt_enable(handle->p_drv,timeout_ms);
}

/**
 * \brief 看门狗喂狗
 *
 * \param[in] handle : WDT标准服务操作句柄
 *
 * \retval  AM_OK    : 操作成功
 *
 * \note 如果不在设定的超时时间内喂狗，看门狗将复位芯片
 */
am_static_inline
int am_wdt_feed (am_wdt_handle_t handle)
{
    return handle->p_funcs->pfn_wdt_feed(handle->p_drv);
}

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_WDT_H */

/* end of file */
