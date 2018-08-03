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
 * \brief DAC标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-04-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DAC_H
#define __AM_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup am_if_dac
 * \copydoc am_dac.h
 * @{
 */

/**
 * \brief DAC驱动函数
 */
struct am_dac_drv_funcs {

    /** \brief 获取DAC转换精度           */
    uint32_t (*pfn_bits_get)(void *p_drv);

    /** \brief 获取DAC参考电压           */
    uint32_t (*pfn_vref_get)(void *p_drv);

    /** \brief 设置通道的DAC转换值       */
    int (*pfn_dac_val_set)(void *p_drv, int chan, uint32_t value);

    /** \brief 使能DAC转换，输出设置电压  */
    int (*pfn_enable)(void *p_drv, int chan);

    /** \brief 禁能DAC转换               */
    int (*pfn_disable)(void *p_drv, int chan);
    
};

/**
 * \brief DAC标准服务
 */
typedef struct am_dac_serv {

    /** \brief 指向DAC驱动函数的指针     */
    const struct am_dac_drv_funcs *p_funcs;

    /** \brief DAC驱动函数第一个入口参数 */
    void                          *p_drv;
} am_dac_serv_t;

/** \brief DAC标准服务操作句柄类型定义 */
typedef am_dac_serv_t *am_dac_handle_t;


/**
 * \brief 获取DAC转换位数
 * \param[in] handle : DAC标准服务操作句柄
 * \return DAC转换位数
 */
am_static_inline
uint32_t am_dac_bits_get (am_dac_handle_t handle)
{
    return handle->p_funcs->pfn_bits_get(handle->p_drv);
}

/**
 * \brief 获取DAC参考电压，单位：mV
 * \param[in] handle : DAC标准服务操作句柄
 * \return DAC参考电压(mV)
 */
am_static_inline
uint32_t am_dac_vref_get (am_dac_handle_t handle)
{
    return handle->p_funcs->pfn_vref_get(handle->p_drv);
}

/**
 * \brief 使能DAC转换，输出设置电压
 *
 * \param[in] handle : DAC标准服务操作句柄
 * \param[in] chan   : DAC通道号
 *
 * \retval   AM_OK      操作成功
 * \retval  -AM_ENXIO   DAC通道号不存在
 */
am_static_inline
uint32_t am_dac_enable (am_dac_handle_t handle, int chan)
{
    return handle->p_funcs->pfn_enable(handle->p_drv, chan);
}


/**
 * \brief 禁能DAC转换
 * \param[in] handle : DAC标准服务操作句柄
 * \param[in] chan   : DAC通道号
 *
 * \retval   AM_OK      操作成功
 * \retval  -AM_ENXIO   DAC通道号不存在
 */
am_static_inline
uint32_t am_dac_disable (am_dac_handle_t handle, int chan)
{
    return handle->p_funcs->pfn_disable(handle->p_drv, chan);
}

/**
 * \brief 设置DAC通道的转换值
 *
 * \param[in] handle : DAC标准服务操作句柄
 * \param[in] chan   : DAC通道号
 * \param[in] val    : 设置的DAC转换值
 *
 * \retval   AM_OK      操作成功
 * \retval  -AM_ENXIO   DAC通道号不存在
 */
am_static_inline
int am_dac_val_set (am_dac_handle_t handle, int chan, uint32_t val)
{
    return handle->p_funcs->pfn_dac_val_set(handle->p_drv, chan, val);
}

/**
 * \brief 设置DAC通道的电压值（单位：mV）
 *
 * \param[in] handle : DAC标准服务操作句柄
 * \param[in] chan   : DAC通道号
 * \param[in] mv     : 设置的电压值
 *
 * \retval   AM_OK      操作成功
 * \retval  -AM_ENXIO   DAC通道号不存在
 */
int am_dac_mv_set(am_dac_handle_t handle, int chan, uint32_t mv);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DAC_H */

/* end of file */
