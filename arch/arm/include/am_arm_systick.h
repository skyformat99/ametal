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
 * \brief SYSTICK 驱动，服务 Timer 标准接口
 *
 * \note 使用 SYSTICK 服务的 Timer 标准接口，就不能使用标准延时接口
 *       am_mdelay()和am_udelay()
 * \internal
 * \par Modification History
 * - 1.00 15-09-22  win, first implementation.
 * \endinternal
 */

#ifndef __AM_ARM_SYSTICK_H
#define __AM_ARM_SYSTICK_H

#include "ametal.h"
#include "am_timer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_arm_if_systick
 * \copydoc am_arm_systick.h
 * @{
 */

/**
 * \brief SYSTICK 设备信息结构体
 */
typedef struct am_arm_systick_devinfo {
    uint32_t     systick_regbase;  /**< \brief SYSTICK 寄存器块基址 */
    uint32_t     clk_id;           /**< \brief SYSTICK 时钟ID */
    uint32_t     clk_freq_src;     /**< \brief 时钟源 */
    
    /** \brief 平台初始化函数 */
    void     (*pfn_plfm_init)(void);

    /** \brief 平台解初始化函数 */
    void     (*pfn_plfm_deinit)(void);
} am_arm_systick_devinfo_t;

/**
 * \brief SYSTICK 设备
 */
typedef struct am_arm_systick_dev {

    am_timer_serv_t timer_serv;    /**< \brief 标准定时(Timer)服务 */
    am_pfnvoid_t    pfn_callback;  /**< \brief 回调函数 */
    void           *p_arg;         /**< \brief 回调函数的用户参数 */

    /** \brief 指向 SYSTICK 设备信息常量的指针 */
    const am_arm_systick_devinfo_t  *p_devinfo;

} am_arm_systick_dev_t;

/**
 * \brief 初始化 SYSTICK 为定时功能
 *
 * \param[in] p_dev     : 指向 SYSTICK 设备的指针
 * \param[in] p_devinfo : 指向 SYSTICK 设备信息常量的指针
 *
 * \return Timer标准服务操作句柄，值为NULL时表明初始化失败
 */
am_timer_handle_t 
am_arm_systick_init(am_arm_systick_dev_t           *p_dev,
                    const am_arm_systick_devinfo_t *p_devinfo);

/**
 * \brief 不使用 SYSTICK 时，解初始化 SYSTICK，释放相关资源
 *
 * \param[in] handle  : 定时器标准服务操作句柄
 *
 * \return 无
 */
void am_arm_systick_deinit(am_timer_handle_t handle);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ARM_SYSTICK_H */

/* end of file */
