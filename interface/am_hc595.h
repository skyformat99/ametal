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
 * \brief 74HC595通用操作接口
 *
 *        该类用于描述一块74HC595芯片, 属于抽象类. 根据不同的驱动方式, 由子类实现对
 *    其的操作, 例如, 使用GPIO驱动74HC595, 使用SPI驱动74HC595.
 *
 *  74HC595引脚及其定义
 *
 *               -------------
 *               | O         |
 *          Q1 --|1        16|-- VCC
 *          Q2 --|2        15|-- Q0
 *          Q3 --|3        14|-- SDI    (DATA Pin)
 *          Q4 --|4   595  13|-- OE     (OE Pin)
 *          Q5 --|5        12|-- RCK    (LOCK Pin)
 *          Q6 --|6        11|-- SCK    (CLOCK Pin)
 *          Q7 --|7        10|-- SCLR
 *         GND --|8         9|-- Q7'
 *               |           |
 *               -------------
 *
 *       Q0 ~ Q7 :  并行数据输出.
 *        GND(8) :  地.
 *        Q7'(9) :  串行数据输入(输入到下一个级联的74HC595).
 *      SCLR(10) :  当SCLR是低电平时, 将移位寄存器中的内容清除.
 *       SCK(11) :  移位寄存器串行输入时钟信号, 移位发生在SCK的上升沿.
 *       RCK(12) :  输出寄存器锁存时钟信号.
 *        OE(13) :  输出使能, 低电平有效.
 *       SDI(14) :  串行数据输入.
 *           VCC :  2 ~ 5v.
 *
 * \par 使用示例
 * \code
 *
 *  int am_main (void)
 *  {
 *      uint8_t buf[2] = {0x02, 0x06};
 *
 *      // 获得句柄，不同平台中的实例初始化函数名(am_hc595_inst_init)可能存在不同
 *      am_hc595_handle_t handle = am_hc595_inst_init();
 *
 *      // 发送数据0x02，0x06， 用于两个HC595级联
 *      am_hc595_send(handle, buf, 2);
 *
 *      // ...
 *  }
 *
 * \endcode
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-23  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_HC595_H
#define __AM_HC595_H

#include "am_common.h"
#include "am_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_hc595
 * \copydoc am_hc595.h
 * @{
 */

/**
 * \brief HC595驱动函数结构体
 */
struct am_hc595_drv_funcs {

    /** \brief 使能HC595输出 */
    int (*pfn_hc595_enable) (void *p_cookie);

    /** \brief 禁能HC595输出（默认，输出保持高阻状态） */
    int (*pfn_hc595_disable) (void *p_cookie);

    /** \brief HC595数据发送  */
    int (*pfn_send) (void *p_cookie, const void *p_data, size_t nbytes);
};

/**
 * \brief HC595 标准服务结构体
 */
typedef struct am_hc595_dev {
    const struct am_hc595_drv_funcs *p_funcs;  /**< \brief 设备驱动函数     */
    void                            *p_cookie; /**< \brief 设备驱动函数参数 */
} am_hc595_dev_t;

/** \brief HC595 标准服务操作句柄定义 */
typedef am_hc595_dev_t *am_hc595_handle_t;

/**
 * \brief 使能HC595输出
 * \param[in] handle : HC595标准服务句柄
 * \return 标准错误号
 */
am_static_inline
int am_hc595_enable (am_hc595_handle_t handle)
{
    if (handle && handle->p_funcs && handle->p_funcs->pfn_hc595_enable) {
        return handle->p_funcs->pfn_hc595_enable(handle->p_cookie);
    }
    return -AM_EINVAL;
}

/**
 * \brief 禁能HC595输出（默认，输出保持高阻状态）
 * \param[in] handle : HC595标准服务句柄
 * \return 标准错误号
 */
am_static_inline
int am_hc595_disable (am_hc595_handle_t handle)
{
    if (handle && handle->p_funcs && handle->p_funcs->pfn_hc595_disable) {
        return handle->p_funcs->pfn_hc595_disable(handle->p_cookie);
    }
    return -AM_EINVAL;
}

/**
 * \brief HC595数据发送
 *
 * \param[in] handle : HC595标准服务句柄
 * \param[in] p_data : 待发送的数据缓存
 * \param[in] nbytes : 数据字节数
 *
 * \return 标准错误号
 */
am_static_inline
int am_hc595_send (am_hc595_handle_t handle, const void *p_data, size_t nbytes)
{
    if (handle && handle->p_funcs && handle->p_funcs->pfn_send) {
        return handle->p_funcs->pfn_send(handle->p_cookie, p_data, nbytes);
    }
    return -AM_EINVAL;
}

/** @} am_if_hc595 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_HC595_H */
