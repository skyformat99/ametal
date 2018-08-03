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
 * \brief  软件定时器标准接口
 *
 * 软件定时器使用一个硬件定时器资源，提供定时服务。
 * 
 * \internal
 * \par Modification history
 * - 1.00 15-07-31  tee,  first implementation.
 * \endinternal
 */

#ifndef __AM_SOFTIMER_H
#define __AM_SOFTIMER_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_common.h"
#include "am_list.h"
#include "am_timer.h"


/**
 * \addtogroup am_if_softimer
 * \copydoc am_softimer.h
 * @{
 */

/**
 * \brief 软件定时器结构体，包含了必要的信息
 */
struct am_softimer {
    struct am_list_head node;          /**< \brief 用于形成链表结构           */
    unsigned int        ticks;         /**< \brief 剩余时间tick值             */
    unsigned int        repeat_ticks;  /**< \brief 保存下重复定时的tick数     */
    void (*timeout_callback)( void *); /**< \brief 定时时间到回调函数         */
    void               *p_arg;         /**< \brief 回调函数的参数             */
};

typedef struct am_softimer am_softimer_t;

/**
 * \brief 软件定时器模块初始化
 *
 * \param[in] clkrate : 运行频率，必须按照该频率调用  am_softimer_module_tick()函数。
 *
 * \retval AM_OK     : 模块初始化成功，可以正常使用
 * \retval -AM_EINVAL: 参数无效
 */
int am_softimer_module_init (unsigned int clkrate);

/**
 * \brief 软件定时器处理函数，必须按照初始化软件定时器模块时指定的频率调用该函数
 */
void am_softimer_module_tick (void);
    
/**
 * \brief 初始化一个软件定时器
 *
 * \param[in] p_timer  : 指向一个软件定时器的指针
 * \param[in] p_func   : 定时时间到的回调函数
 * \param[in] p_arg    : 回调函数参数
 *
 * \retval AM_OK      : 软件定时器初始化成功
 * \retval -AM_EINVAL : 无效参数
 * \retval -AM_EPERM  : 软件定时器模块还未初始化，操作不运行，请先确保模块正确初始化
 */
int am_softimer_init(am_softimer_t *p_timer, am_pfnvoid_t p_func, void *p_arg);


/**
 * \brief 启动一个软件定时器
 * \param[in] p_timer : 指向一个软件定时器的指针
 * \param[in] ms      : 定时时间(ms)
 * \return 无
 */
void am_softimer_start(am_softimer_t *p_timer, unsigned int ms);

/**
 * \brief 停止一个软件定时器
 * \param[in] p_timer : 指向一个软件定时器的指针
 * \return 无
 */
void am_softimer_stop(am_softimer_t *p_timer);

/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SOFTIMER_H */

/* end of file */
