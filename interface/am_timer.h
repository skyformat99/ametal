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
 * \brief 定时器标准接口
 *
 * \internal
 * \par Modification History
 * - 1.01 15-12-07  hgo, del am_timer_connect().
 * - 1.00 15-01-05  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_TIMER_H
#define __AM_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup am_if_timer
 * \copydoc am_timer.h
 * @{
 */

/**
 * \name 定时器特性
 *  
 *  - 低8位代表定时器位数，如32位定时器，则低8位值为32
 *  - 8 ~ 15 位表示定时器通道数
 *  - 其它定时器特性仅占一位，从第16位开始。 AM_TIMER_FEATURE(0) ~ AM_TIMER_FEATURE(15)
 * @{
 */
 
/** \brief 用于其它定时器特性定义         */
#define AM_TIMER_FEATURE(feature_id)           ((uint32_t)(1u << (feature_id)))

/** \brief 定时器可以产生中断             */
#define AM_TIMER_CAN_INTERRUPT                  AM_TIMER_FEATURE(0)

/** \brief 定时器可以读取当前计数器的值   */
#define AM_TIMER_INTERMEDIATE_COUNT             AM_TIMER_FEATURE(1)

/** \brief 定时器不能被停止               */
#define AM_TIMER_CANNOT_DISABLE                 AM_TIMER_FEATURE(2)

/** \brief 定时器在读取计数值时会停止     */
#define AM_TIMER_STOP_WHILE_READ                AM_TIMER_FEATURE(3)

/** \brief 定时器支持自动重装载计数值     */
#define AM_TIMER_AUTO_RELOAD                    AM_TIMER_FEATURE(4)

/** \brief 定时器自由运行，不能改变翻转值 */
#define AM_TIMER_CANNOT_MODIFY_ROLLOVER         AM_TIMER_FEATURE(5)
 
/** 
 * \brief 定时器的预分频值支持1至最大值之间的任意值
 * 
 * - 若定时器支持该特性，则定时器信息中的 prescaler (am_timer_info_t)表示支持的
 *   最大预分频值，例如，prescaler的值为 256，表示支持 1 ~ 256 所有预分频值；
 *
 * - 若定时器不支持该特性，则定时器信息中的 prescaler 表示具体支持哪些预分频值，
 *   若第N位（N的范围： 0 ~ 31）为 1，则表示支持预分频值：2 的 N次方。例如，
 *   prescaler的值为0xF，bit0、1、2、3的值为1，则支持的分频值有：1(2^0)、2(2^1)、
 *   4(2^2)、8(2^3)
 *
 * - 特别地，若 prescaler 的值为0，表示定时器不支持预分频。这种情况不应将 prescaler
 *   设置为 0x01。
 */
#define AM_TIMER_SUPPORT_ALL_PRESCALE_1_TO_MAX  AM_TIMER_FEATURE(6)

/** @} */ 


/** 
 * \brief 定时器信息结构体
 */
typedef struct am_timer_info {

    uint8_t   counter_width;               /**< \brief 定时器位数             */
    uint8_t   chan_num;                    /**< \brief 定时器通道数           */
    uint16_t  features;                    /**< \brief 定时器特性             */
 
    /** 
     * \brief 预分频器信息
     *
     * 该值表示预分频器支持的预分频值，该值的含义与定时器的预分频器特性相关，
     * 详见该特性的解释 #AM_TIMER_SUPPORT_ALL_PRESCALE_1_TO_MAX
     */
    uint32_t   prescaler; 
    
} am_timer_info_t;


/**
 * \brief 定时器驱动函数结构体
 */
struct am_timer_drv_funcs {

    /**
     * \brief 获取定时器基础信息
     *
     * 该信息是定时器的固有属性，在驱动内部完成定义，该函数用以返回指向该常量信
     * 息的指针，以便用户使用。
     */
    const am_timer_info_t * (*pfn_info_get) (void *p_drv);
    
    /**
     * \brief 获取定时器的输入时钟频率（所有定时器通道共用一个输入频率）
     *
     * 输入时钟频率与时钟设置、具体硬件等相关，该频率并非定时器的计时频率，定时
     * 器的实际计时频率可能是通过一个预分频器分频后的频率。
     */
    int (*pfn_clkin_freq_get) (void *p_drv, uint32_t *p_freq);
    
    /**
     * \brief 设置定时器的预分频值
     *
     * 若定时器不支持预分频，则 该函数可以为 NULL。
     */
    int (*pfn_prescale_set) (void *p_drv, uint8_t chan, uint32_t prescale);

    /**
     * \brief 获取定时器当前使用的预分频值
     *
     * 使用输入频率除以该预分频值，即可得到定时器的计数频率
     */
    int (*pfn_prescale_get) (void *p_drv, uint8_t chan, uint32_t *p_prescale);

    /**
     * \brief 获取当前定时器计数值
     *
     * 若定时器的位数不超过32位，则  p_count 为  uint32_t* 类型
     * 若定时器的位数超过32位，则  p_count 为  uint64_t* 类型
     */
    int (*pfn_count_get) (void *p_drv, uint8_t chan, void *p_count);

    /** 
     * \brief 获取定时器翻转值
     * 
     * 若定时器的位数不超过32位，则  p_count 为  uint32_t* 类型
     * 若定时器的位数超过32位，则  p_count 为  uint64_t* 类型
     */
    int (*pfn_rollover_get) (void *p_drv, uint8_t chan, void *p_rollover);
 
    /** 
     * \brief 使能定时器，同时设定定时count值
     *
     * 若定时器的位数不超过32位，则  p_count 为  uint32_t* 类型
     * 若定时器的位数超过32位，则  p_count 为  uint64_t* 类型
     *
     * 计数器从0开始计数，达到该值时将调用用户设定的回调函数（若回调函数有效）
     * 同时，计数器的值回到0重新开始计数。
     */
    int (*pfn_enable) (void *p_drv, uint8_t chan, void *p_count);

    /** \brief 禁能定时器                         */
    int (*pfn_disable) (void *p_drv, uint8_t chan);

    /** \brief 设置回调函数，每当一个定时周期完成时调用 */
    int (*pfn_callback_set)(void    *p_drv,
                            uint8_t  chan,
                            void   (*pfn_callback)(void *),
                            void    *p_arg);
};

/** 
 * \brief 定时器服务
 */
typedef struct am_timer_serv {

    /** \brief 定时器驱动函数结构体指针 */
    struct am_timer_drv_funcs *p_funcs; 
    
    /** \brief 用于驱动函数的第一个参数 */
    void                      *p_drv;   
} am_timer_serv_t;

/** \brief 定时器标准服务操作句柄类型定义 */
typedef am_timer_serv_t *am_timer_handle_t;

/** 
 * \brief 获取定时器信息
 * \param[in]  handle : 定时器标准服务操作句柄
 * \return 指向定时器信息常量的指针，若为NULL，则表示信息获取失败
 */
am_static_inline
const am_timer_info_t * am_timer_info_get (am_timer_handle_t handle)
{
    return handle->p_funcs->pfn_info_get(handle->p_drv);
}

/**
 * \brief 获取当前定时器的输入时钟频率（所有定时器通道共用一个输入频率）
 *
 * 定时器的输入频率并非定时器的计数频率，实际定时器计数频率是由该频率分频的结果，
 * 支持的分频值详见特性：#AM_TIMER_SUPPORT_ALL_PRESCALE_1_TO_MAX 的描述。具体分频
 * 值可以通过  am_timer_prescale_set() 接口设置。
 *
 * \param[in]  handle : 定时器标准服务操作句柄
 * \param[out] p_freq : 获取定时器输入频率的指针
 *
 * \return 无
 */
am_static_inline
int am_timer_clkin_freq_get (am_timer_handle_t handle, uint32_t *p_freq)
{
    return handle->p_funcs->pfn_clkin_freq_get(handle->p_drv, p_freq);
}

/** 
 * \brief 设置定时器的预分频值
 *
 * \param[in] handle   : 定时器标准服务操作句柄
 * \param[in] chan     : 定时器通道
 * \param[in] prescale : 预分频值
 *
 * \retval  AM_OK      : 设置成功
 * \retval -AM_EINVAL  : 设置失败, 参数错误
 * \retval -AM_ENOTSUP : 设置失败，不支持的预分频值，支持的预分频值详见定时器信息
 */
am_static_inline
int am_timer_prescale_set (am_timer_handle_t handle, 
                           uint8_t           chan, 
                           uint32_t          prescale)
{
    return handle->p_funcs->pfn_prescale_set(handle->p_drv, chan, prescale); 
}

/** 
 * \brief 设置定时器的预分频值
 *
 * \param[in] handle   : 定时器标准服务操作句柄
 * \param[in] chan     : 定时器通道
 * \param[in] prescale : 预分频值
 *
 * \retval  AM_OK      : 设置成功
 * \retval -AM_EINVAL  : 设置失败, 参数错误
 * \retval -AM_ENOTSUP : 设置失败，不支持的预分频值，支持的预分频值详见定时器信息
 */
am_static_inline
int am_timer_prescale_get (am_timer_handle_t   handle,
                           uint8_t             chan,
                           uint32_t           *p_prescale)
{
    return handle->p_funcs->pfn_prescale_get(handle->p_drv, chan, p_prescale);
}

/**
 * \brief 获取定时器当前的计数频率
 *
 * \param[in]  handle  : 定时器标准服务操作句柄
 * \param[in]  chan    : 定时器通道
 * \param[out] p_freq  : 用于获取定时器当前计数频率的指针
 *
 * \retval  AM_OK      : 获取定时器当前计数值成功
 * \retval -AM_EINVAL  : 获取失败, 参数错误
 */
int am_timer_count_freq_get (am_timer_handle_t handle,
                             uint8_t           chan,
                             uint32_t         *p_freq);

/** 
 * \brief 获取定时器当前的计数值
 * 
 * 返回的计数值应该是假设定时器工作在向上计数模式下的当前计数值
 *
 * \param[in]  handle  : 定时器标准服务操作句柄
 * \param[in]  chan    : 定时器通道
 * \param[out] p_count : 用于获取定时器当前计数值的指针
 *
 * \retval  AM_OK      : 获取定时器当前计数值成功
 * \retval -AM_EINVAL  : 获取失败, 参数错误
 */
am_static_inline
int am_timer_count_get (am_timer_handle_t handle, 
                        uint8_t           chan, 
                        uint32_t         *p_count)
{
    return handle->p_funcs->pfn_count_get(handle->p_drv, chan, (void*)p_count);
}

/** 
 * \brief 获取定时器翻转值
 *
 * \param[in]  handle     : 定时器标准服务操作句柄
 * \param[in]  chan       : 定时器通道
 * \param[out] p_rollover : 用于获取定时器翻转值的指针
 *
 * \retval  AM_OK         : 获取定时器翻转值成功
 * \retval -AM_EINVAL     : 获取失败, 参数错误
 */
am_static_inline
int am_timer_rollover_get (am_timer_handle_t  handle, 
                           uint8_t            chan, 
                           uint32_t          *p_rollover)
{
    return handle->p_funcs->pfn_rollover_get(handle->p_drv,
                                             chan,
                                            (void *)p_rollover);
}

/** 
 * \brief 禁能定时器（停止计时）
 *
 * \param[in] handle  : 定时器标准服务操作句柄
 * \param[in] chan    : 定时器通道
 *
 * \retval  AM_OK     : 禁能定时器成功
 * \retval -AM_EINVAL : 禁能失败, 参数错误
 */
am_static_inline
int am_timer_disable (am_timer_handle_t handle, uint8_t chan)
{
    return handle->p_funcs->pfn_disable(handle->p_drv, chan);
}

/** 
 * \brief 使能定时器，同时设定定时count值 
 *
 * \param[in] handle : 定时器标准服务操作句柄
 * \param[in] chan   : 定时器通道
 * \param[in] count  : 定时count值 
 *
 * \retval  AM_OK     : 使能成功
 * \retval -AM_EINVAL : 失败, 参数错误
 *
 * \note 定时器计数值达到设定的定时count值时，会产生中断。同时，复位计数值
 *       为0，紧接着继续计时。
 */
am_static_inline
int am_timer_enable (am_timer_handle_t handle, uint8_t chan, uint32_t count)
{
     return handle->p_funcs->pfn_enable(handle->p_drv, chan, (void *)(&count));
}

/** 
 * \brief 设置回调函数，每当一个定时周期完成时调用
 *
 * \param[in] handle       : 定时器标准服务操作句柄
 * \param[in] chan         : 定时器通道
 * \param[in] pfn_callback : 回调函数
 * \param[in] p_arg        : 回调函数的用户参数
 *
 * \retval  AM_OK     : 设置成功
 * \retval -AM_EINVAL : 设置失败, 参数错误
 *
 */
am_static_inline
int am_timer_callback_set (am_timer_handle_t  handle, 
                           uint8_t            chan,
                           void             (*pfn_callback)(void *),
                           void              *p_arg)
{
    return handle->p_funcs->pfn_callback_set(handle->p_drv,
                                             chan,
                                             pfn_callback,
                                             p_arg);
}

/** 
 * \brief 获取33 ~ 64位定时器的当前计数值
 *
 * 返回的计数值应该是假设定时器工作在向上计数模式下的当前计数值
 *
 * \param[in]  handle  : 定时器标准服务操作句柄
 * \param[in]  chan    : 定时器通道
 * \param[out] p_count : 用于获取定时器当前计数值的指针
 *
 * \retval  AM_OK      : 获取到的当前定时器计数值
 * \retval -AM_EINVAL  : 获取失败, 参数错误
 *
 * \note 如果定时器位数在33到64之间，必须调用该函数获取定时器的当前计数值
 */
am_static_inline
int am_timer_count_get64 (am_timer_handle_t  handle, 
                          uint8_t            chan,
                          uint64_t          *p_count)
{
    return handle->p_funcs->pfn_count_get(handle->p_drv, chan, (void *)p_count);
}

/** 
 * \brief 获取33 ~ 64位定时器的翻转值
 *
 * \param[in]  handle     : 定时器标准服务操作句柄
 * \param[in]  chan       : 定时器通道
 * \param[out] p_rollover : 用于获取定时器翻转值的指针
 *
 * \retval  AM_OK         : 获取翻转值成功
 * \retval -AM_EINVAL     : 获取翻转值失败, 参数错误
 *
 * \note 如果定时器位数在33到64之间，必须调用该函数获取翻转值
 */
am_static_inline
int am_timer_rollover_get64 (am_timer_handle_t  handle, 
                             uint8_t            chan,
                             uint64_t          *p_rollover)
{
    return handle->p_funcs->pfn_rollover_get(handle->p_drv,
                                             chan,
                                            (void *)p_rollover);
}

/** 
 * \brief 使能33到64位定时器，并设置定时count值
 *
 * \param[in] handle : 定时器标准服务操作句柄
 * \param[in] chan   : 定时器通道
 * \param[in] count  : 定时器count值
 *
 * \retval  AM_OK     : 使能成功
 * \retval -AM_EINVAL : 使能失败, 参数错误
 *
 * \note 如果定时器位数在33到64之间，必须调用该函数使能定时器
 */
am_static_inline
int am_timer_enable64 (am_timer_handle_t handle, uint8_t chan, uint64_t count)
{
    return handle->p_funcs->pfn_enable(handle->p_drv, chan, (void *)(&count));
}

/** 
 * \brief 使能定时器，直接设定定时时间
 *
 * 一般情况下，用户需要使用一个定时时间，需要根据输入频率和定时器位数，计算好预
 * 分频值和定时 count 值，然后使用 am_timer_prescale_set() 设置预分频值，最后使
 * 用 am_timer_enable() 或 am_timer_enable64() 设置定时count值并使能定时器。
 * 
 * 手动计算需要考虑定时器输入频率、定时器位数、定时器特性（部分定时器可能不支持
 * 或只支持几种预分频值），较为繁琐。基于此，用户可以直接使用该函数按照指定的时
 * 间值（单位：us）使能定时器。系统会自动根据定时器位数、定时器特性等设置好定时
 * 器。
 *
 * 计时时间可能存在偏差，实际定时时间可以由 计数频率 和 翻转值 得到：翻转值乘以
 * 计数频率的倒数即可。
 *
 * 由于定时时间到会产生中断，因此，不建议将该时间设置得过短，至少应在几百us，建议
 * 设置到 ms 级别。
 *
 * \param[in] handle : 定时器标准服务操作句柄
 * \param[in] chan   : 定时器通道
 * \param[in] nus    : 设置的定时时间（单位：us）
 *
 * \retval  AM_OK     : 使能成功
 * \retval -AM_EINVAL : 失败, 参数错误
 * \retval -AM_NOTSUP ：不支持该定时时间，时间太长（定时器位数不够）
 */
int am_timer_enable_us (am_timer_handle_t handle, uint8_t chan, uint32_t nus);

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_TIMER_H */

/* end of file */
