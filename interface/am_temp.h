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
 * \brief 温度传感器操作接口
 *
 * 使用本服务需要包含以下头文件:
 * \code
 * #include "am_temp.h"
 * \endcode
 *
 * \internal
 * \par modification history
 * - 1.00 17-06-19  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_TEMP_H
#define __AM_TEMP_H

#ifdef __cplusplus
extern "C" {
#endif
    
/**
 * \addtogroup am_if_temp
 * \copydoc am_temp.h
 * @{
 */

#include "am_common.h"

/**
 * \brief 温度传感器驱动函数结构体
 */
struct am_temp_drv_funcs {

    /** \brief 读取温度，得到的温度值为实际温度值的1000倍  */
    int (*pfn_temp_read) (void *p_drv, int32_t *p_temp);
};
 
/** 
 * \brief 温度服务
 */
typedef struct am_temp_dev {

    /** \brief 温度传感器驱动函数   */
    const struct am_temp_drv_funcs  *p_funcs;
    
    /** \brief 用于驱动函数的第一个参数 */
    void                            *p_cookie;

} am_temp_dev_t;

/** \brief 温度标准服务操作句柄类型定义 */
typedef am_temp_dev_t *am_temp_handle_t;
 
/**
 * \brief 读取一个温度传感器设备的温度值
 *
 * 使用该接口可以方便的获取硬件外设上对应温度传感器的温度值。
 *
 * \param[in] handle  : 标准温度服务句柄
 * \param[out] p_temp : 返回当前读取到的温度值，为实际温度值的1000倍
 *
 * \retval AM_OK : 读取温度成功
 * \retval  < 0  ： 读取温度出错
 */
am_static_inline
int am_temp_read (am_temp_handle_t handle, int32_t *p_temp)
{
    return handle->p_funcs->pfn_temp_read(handle->p_cookie, p_temp);
}

/** @} am_if_temp */

#ifdef __cplusplus
}
#endif

#endif /* __AM_TEMP_H */

/* end of file */
