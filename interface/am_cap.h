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
 * \brief 输入捕获标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-09  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_CAP_H
#define __AM_CAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup am_if_cap
 * \copydoc am_cap.h
 * @{
 */

/** \brief 上升沿触发捕获 */
#define AM_CAP_TRIGGER_RISE          0x01
    
/** \brief 下降沿触发捕获 */
#define AM_CAP_TRIGGER_FALL          0x02

/** \brief 双边沿触发捕获 */
#define AM_CAP_TRIGGER_BOTH_EDGES    0x04

/** \brief 捕获事件的回调函数类型 */
typedef void (*am_cap_callback_t) (void *p_arg, unsigned int count); 
    
/**
 * \brief 输入捕获驱动函数结构体
 */
struct am_cap_drv_funcs {

    /** \brief 配置一个捕获通道               */
    int (*pfn_cap_config) (void             *p_cookie,
                           int               chan,
                           unsigned int      options,
                           am_cap_callback_t pfn_callback,
                           void             *p_arg);

    /** \brief 使能捕获通道                   */
    int (*pfn_cap_enable) (void *p_cookie, int chan);

    /** \brief 禁能捕获通道                   */
    int (*pfn_cap_disable) (void *p_cookie, int chan);
                             
    /** \brief 复位一个捕获通道，使计数值归零         */
    int (*pfn_cap_reset) (void *p_cookie,int chan);
    
    /** \brief 转换两次捕获值为时间值（时间单位：ns） */
    int (*pfn_cap_count_to_time)(void         *p_cookie,
                                 int           chan,
                                 unsigned int  count1,
                                 unsigned int  count2,
                                 unsigned int *p_time_ns);
};


/** 
 * \brief 输入捕获服务
 */
typedef struct am_cap_serv {

    /** \brief 输入捕获驱动函数结构体指针    */
    struct am_cap_drv_funcs   *p_funcs;   
    
    /** \brief 用于驱动函数的第一个参数      */
    void                      *p_drv;    
} am_cap_serv_t;

/** \brief 输入捕获标准服务操作句柄类型定义  */
typedef am_cap_serv_t *am_cap_handle_t;


/** 
 * \brief 配置一个输入捕获通道
 * 
 * \param[in] handle       : 输入捕获标准服务操作句柄
 * \param[in] chan         : 输入捕获通道
 * \param[in] options      : AM_CAP_TRIGGER_*宏值，如：  #AM_CAP_TRIGGER_RISE
 * \param[in] pfn_callback : 回调函数
 * \param[in] p_arg        : 回调函数的用户参数
 *
 * \retval  AM_OK       : 配置CAP通道成功
 * \retval -AM_EINVAL   : 配置失败，参数错误
 */
am_static_inline
int am_cap_config (am_cap_handle_t   handle,
                   int               chan,
                   unsigned int      options,
                   am_cap_callback_t pfn_callback,
                   void             *p_arg)
{
    return handle->p_funcs->pfn_cap_config(handle->p_drv,
                                           chan,
                                           options,
                                           pfn_callback,
                                           p_arg);
                      
}

/** 
 * \brief 使能捕获通道
 * 
 * \param[in] handle  : 输入捕获标准服务操作句柄
 * \param[in] chan    : 输入捕获通道
 *
 * \retval  AM_OK     : 使能捕获通道成功
 * \retval -AM_EINVAL : 使能捕获通道失败，参数错误
 */
am_static_inline
int am_cap_enable (am_cap_handle_t handle, int chan)
{
    return handle->p_funcs->pfn_cap_enable(handle->p_drv, chan);
}

/** 
 * \brief 禁能捕获通道
 * 
 * \param[in] handle  : 输入捕获标准服务操作句柄
 * \param[in] chan    : 输入捕获通道
 *
 * \retval  AM_OK     : 禁能捕获通道成功
 * \retval -AM_EINVAL : 禁能捕获通道失败，参数错误
 */
am_static_inline
int am_cap_disable(am_cap_handle_t handle, int chan)
{
    return handle->p_funcs->pfn_cap_disable(handle->p_drv, chan);
}


/** 
 * \brief 复位捕获通道计数值为0
 *
 * \param[in] handle  : 输入捕获标准服务操作句柄
 * \param[in] chan    : 输入捕获通道
 *
 * \retval  AM_OK     : 复位成功
 * \retval -AM_EINVAL : 复位失败, 参数错误
 */
am_static_inline
int am_cap_reset(am_cap_handle_t handle, int chan)
{
    return handle->p_funcs->pfn_cap_reset(handle->p_drv, chan);
}

/** 
 * \brief 转换两次捕获值为时间值（时间单位：ns）
 *
 * \param[in]  handle    : 输入捕获标准服务操作句柄
 * \param[in]  chan      : 输入捕获通道
 * \param[in]  count1    : 第一次捕获值
 * \param[in]  count2    : 第二次捕获值
 * \param[out] p_time_ns : 用于获取转换结果时间值
 *
 * \retval  AM_OK        : 转换成功
 * \retval -AM_EINVAL    : 转换失败，参数错误
 */
am_static_inline
int am_cap_count_to_time(am_cap_handle_t handle,
                         int             chan,
                         unsigned int    count1,
                         unsigned int    count2,
                         unsigned int   *p_time_ns)
{
    return handle->p_funcs->pfn_cap_count_to_time(handle->p_drv, 
                                                  chan,
                                                  count1,
                                                  count2,
                                                  p_time_ns);
}

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_CAP_H */

/* end of file */
