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
 * \brief 等待(wait)标准接口
 *
 * 该接口的使用方式是，首先定义一个等待信号：am_wait_t wait;
 * 在需要等待的地方调用:am_wait_on(&wait);
 * 在需要结束之前开启等待的地方调用：am_wait_done(&wait);
 *
 *
 * \internal
 * \par Modification History
 * - 1.01 15-09-07  tee, add am_wait_on_timeout() interface.
 * - 1.00 15-06-12  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_WAIT_H
#define __AM_WAIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "am_types.h"
#include "am_softimer.h"
    
/**
 * \addtogroup am_if_wait
 * \copydoc am_wait.h
 * @{
 */

 /**
  * \brief 定义等待类型结构体，应用程序不应直接操作该结构体成员
  */
typedef struct am_wait {
    
    /** \brief 等待值                  */
    volatile uint8_t  val;
    
    /** \brief 用于超时计算的软件定时器 */
    am_softimer_t     timer;
    
    /** \brief 标志当前的等待状态       */
    uint8_t           stat;

} am_wait_t;
    
 
/** 
 * \brief 等待信号初始化
 *
 * 使用等待之前，必须先使用该函数初始化，初始化一次即可
 *
 * \param[in] p_wait  : 指向等待信号的指针
 *
* \retval  AM_OK      : 初始化成功
 * \retval -AM_EINVAL ：初始化失败，参数错误
 */
int am_wait_init(am_wait_t *p_wait);

/** 
 * \brief 启动等待事件
 *
 * 程序会一直等待，直到调用 am_wait_done() 指明等待完成
 * 
 * \param[in] p_wait  : 指向等待信号的指针
 *
 * \retval  AM_OK     ：等待成功完成，等待到了相应的事件
 * \retval -AM_EINVAL ：开启等待失败，参数错误
 */
int am_wait_on(am_wait_t *p_wait);

/** 
 * \brief 启动等待事件（可设置超时时间）
 *
 * 程序会一直等待，直到调用 am_wait_done() 指明等待完成
 * 
 * \param[in] p_wait      : 指向等待信号的指针
 * \param[in] timeout_ms  : 等待超时的时间（单位：ms），若为0，则一直等待。
 *
 * \retval  AM_OK     ：等待成功完成，等待到了相应的事件
 * \retval -AM_ETIME  ：等待超时，指定ms的时间内为接收到等待结束信息
 * \retval -AM_EINVAL ：开启等待失败，参数错误
 */
int am_wait_on_timeout(am_wait_t *p_wait, uint32_t timeout_ms);

/** 
 * \brief 结束等待事件
 *
 *    调用该函数后，am_wait_on() 或 am_wait_on_timeout() 
 * 启动的等待将成功等待完成，等待结束。
 * 
 * \param[in] p_wait  : 指向等待信号的指针
 *
 * \retval  AM_OK     ：成功结束了已经启动的等待
 * \retval -AM_EINVAL ：结束等待失败，参数错误
 */
int am_wait_done(am_wait_t *p_wait);
 

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_WAIT_H */

/* end of file */
