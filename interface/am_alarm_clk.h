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
 * \brief  ALARM_CLK 标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 17-08-09  vir, first implementation.
 * \endinternal
 */
#ifndef __AM_ALARM_CLK_H
#define __AM_ALARM_CLK_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup am_if_alarm_clk
 * \copydoc am_alarm_clk.h
 * @{
 */

/**
 * \name 闹钟时间的星期取值
 * \anchor grp_alarm_clk_week
 *
 * \note  如果要设置星期三和星期星期天 的闹钟   则 tm_wday 的值为 ：
 *        AM_ALARM_CLK_WEDNESDAY | AM_ALARM_CLK_SUNDAY
 * @{
 */

#define AM_ALARM_CLK_SUNDAY    0X01    /**< \brief 星期日 */
#define AM_ALARM_CLK_MONDAY    0X02    /**< \brief 星期一 */
#define AM_ALARM_CLK_TUESDAY   0X04    /**< \brief 星期二 */
#define AM_ALARM_CLK_WEDNESDAY 0X08    /**< \brief 星期三 */
#define AM_ALARM_CLK_THURSDAY  0X10    /**< \brief 星期四 */
#define AM_ALARM_CLK_FRIDAY    0X20    /**< \brief 星期五 */
#define AM_ALARM_CLK_SATURDAY  0X40    /**< \brief 星期六 */
#define AM_ALARM_CLK_WORKDAY   0X3E    /**< \brief 工作日：星期一 ~ 星期五 */
#define AM_ALARM_CLK_EVERYDAY  0X7F    /**< \brief 每一天  */

 /** @} */

/**
 * \brief 闹钟时间结构体
 *
 * \note 时间是24小时制
 */
typedef struct am_alarm_clk_tm {
    int min;      /**< \brief 分    值范围： [0, 59]  */
    int hour;     /**< \brief 小时  值范围：[0, 23]  */
    int wdays;    /**< \brief星期   值请参考  \ref grp_alarm_clk_week */
} am_alarm_clk_tm_t;

/**
 * \brief ALARM_CLK 驱动函数
 */
struct am_alarm_clk_drv_funcs {

    /** \brief 设置闹钟时间 */
    int (*pfn_alarm_clk_time_set)(void *p_drv, am_alarm_clk_tm_t *p_tm);

    /** \brief 设置闹钟回调函数 */
    int (*pfn_alarm_clk_callback_set)(void          *p_drv,
                                      am_pfnvoid_t   pfn_callback,
                                      void          *p_arg);
    /** \brief 开启闹钟  */
    int (*pfn_alarm_clk_on)(void *p_drv);

    /** \brief 关闭闹钟  */
    int (*pfn_alarm_clk_off)(void *p_drv);

};


/**
 * \brief ALARM_CLK 标准服务
 */
typedef struct am_alarm_clk_serv {

    /** \brief RTC驱动函数 */
    struct am_alarm_clk_drv_funcs *p_funcs;

    /** \brief 驱动函数第一个入口参数 */
    void                      *p_drv;
} am_alarm_clk_serv_t;

/** \brief ALARM_CLK 标准服务操作句柄类型 */
typedef am_alarm_clk_serv_t *am_alarm_clk_handle_t;


/**
 * \brief 设置闹钟时间
 *
 * \param[in] handle : ALARM_CLK 标准服务操作句柄
 * \param[in] p_tm   : 要设置的时间
 *
 * \return    AM_OK    : 设置成功
 * \retval  -AM_EINVAL : 参数错误
 */
am_static_inline
int am_alarm_clk_time_set (am_alarm_clk_handle_t handle, am_alarm_clk_tm_t *p_tm)
{
    return handle->p_funcs->pfn_alarm_clk_time_set(handle->p_drv, p_tm);
}

/**
 * \brief 设置闹钟时间
 *
 * \param[in] handle         : ALARM_CLK 标准服务操作句柄
 * \param[in] pfn_callback   : 闹钟中断的回调函数
 * \param[in] p_arg          : 回调函数的参数
 *
 * \return    AM_OK    : 设置成功
 * \retval  -AM_EINVAL : 参数错误
 */
am_static_inline
int am_alarm_clk_callback_set (am_alarm_clk_handle_t handle,
                               am_pfnvoid_t          pfn_callback,
                               void                 *p_arg)
{
    return handle->p_funcs->pfn_alarm_clk_callback_set(handle->p_drv, pfn_callback, p_arg);
}

/**
 * \brief 开启闹钟
 *
 * \param[in]  handle  : ALARM_CLK标准服务操作句柄
 *
 * \return   AM_OK     : 获取时间成功
 * \retval  -AM_EINVAL : 参数错误
 */
am_static_inline
int am_alarm_clk_on (am_alarm_clk_handle_t handle)
{
    return handle->p_funcs->pfn_alarm_clk_on(handle->p_drv);
}

/**
 * \brief 关闭闹钟
 *
 * \param[in]  handle  : ALARM_CLK标准服务操作句柄
 *
 * \return   AM_OK     : 获取时间成功
 * \retval  -AM_EINVAL : 参数错误
 */
am_static_inline
int am_alarm_clk_off (am_alarm_clk_handle_t handle)
{
    return handle->p_funcs->pfn_alarm_clk_off(handle->p_drv);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ALARM_CLK_H */

/* end of file */
