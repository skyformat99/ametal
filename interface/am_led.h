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
 * \brief  LED标准接口
 * 
 * \par 使用示例
 * \code
 * #include "am_led.h"
 *
 * am_led_set(0, AM_TRUE);     // 点亮LED0
 * am_led_set(0, AM_FALSE);    // 熄灭LED0
 *
 * am_led_on(1);               // 点亮LED1
 * am_led_off(1);              // 熄灭LED1
 *
 * \endcode
 *
 * \internal
 * \par Modification history
 * - 1.01 14-12-01  tee, add the LED to the standard interface
 * - 1.00 14-11-25  fft, first implementation.
 * \endinternal
 */

#ifndef __AM_LED_H
#define __AM_LED_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_common.h"


/**
 * \addtogroup am_if_led
 * \copydoc am_led.h
 * @{
 */

/**
 * \brief 设置LED的状态
 *
 * \param[in] led_id : LED编号, 可用编号由具体平台决定
 * \param[in] state  : AM_TRUE : LED亮；AM_FALSE : LED灭
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_ENODEV : 设备不存在
 */
int am_led_set (int led_id, am_bool_t state);

/**
 * \brief 点亮LED
 *
 * \param[in] led_id : LED编号
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_ENODEV : 设备不存在
 */
int am_led_on(int led_id);

/**
 * \brief 熄灭LED
 *
 * \param[in] led_id : LED编号
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_ENODEV : 设备不存在
 */
int am_led_off(int led_id);

/**
 * \brief 翻转指定LED的状态
 *
 * \param[in] led_id : LED编号
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_ENODEV : 设备不存在
 */
int am_led_toggle(int led_id);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* __AM_LED_H */

/* end of file */
