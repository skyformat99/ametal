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
 * \brief PWM标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_PWM_H
#define __AM_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup am_if_pwm
 * \copydoc am_pwm.h
 * @{
 */

/**
 * \brief PWM驱动函数结构体
 */
struct am_pwm_drv_funcs {

    /** \brief 配置一个PWM通道  */
    int (*pfn_pwm_config) (void          *p_drv,
                           int            chan,
                           unsigned long  duty_ns,
                           unsigned long  period_ns);
    
    /** \brief 使能PWM输出     */
    int (*pfn_pwm_enable) (void *p_drv, int chan);
    
    /** \brief 禁能PWM输出     */
    int (*pfn_pwm_disable) (void *p_drv,int chan);
};

/** 
 * \brief PWM服务
 */
typedef struct am_pwm_serv {

    /** \brief PWM驱动函数结构体指针   */
    struct am_pwm_drv_funcs   *p_funcs;
    
    /** \brief 用于驱动函数的第一个参数 */
    void                      *p_drv; 
} am_pwm_serv_t;

/** \brief PWM标准服务操作句柄类型定义 */
typedef am_pwm_serv_t *am_pwm_handle_t;

/** 
 * \brief 配置一个PWM通道
 * 
 * \param[in] handle    : PWM标准服务操作句柄
 * \param[in] chan      : PWM通道
 * \param[in] duty_ns   : PWM脉宽时间（单位；纳秒）
 * \param[in] period_ns : PWM周期时间（单位；纳秒）
 *
 * \retval  AM_OK       : 配置PWM通道成功
 * \retval -AM_EINVAL   : 配置失败, 参数错误
 */
am_static_inline
int am_pwm_config (am_pwm_handle_t handle, 
                   int             chan,
                   unsigned long   duty_ns,
                   unsigned long   period_ns)
{
    return handle->p_funcs->pfn_pwm_config(handle->p_drv,
                                           chan,
                                           duty_ns,
                                           period_ns);
}

/** 
 * \brief 使能PWM输出
 * 
 * \param[in] handle  : PWM标准服务操作句柄
 * \param[in] chan    : PWM通道
 *
 * \retval  AM_OK     : 使能PWM输出成功
 * \retval -AM_EINVAL : 失败, 参数错误
 */
am_static_inline
int am_pwm_enable (am_pwm_handle_t handle, int chan)
{
    return handle->p_funcs->pfn_pwm_enable(handle->p_drv, chan);
}

/** 
 * \brief 禁能PWM输出
 * 
 * \param[in] handle  : PWM标准服务操作句柄
 * \param[in] chan    : PWM通道
 *
 * \retval  AM_OK     : 禁能PWM输出成功
 * \retval -AM_EINVAL : 失败, 参数错误
 */
am_static_inline
int am_pwm_disable (am_pwm_handle_t handle, int chan)
{
    return handle->p_funcs->pfn_pwm_disable(handle->p_drv, chan);
}

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_PWM_H */

/* end of file */
