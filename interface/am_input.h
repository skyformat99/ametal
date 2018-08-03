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
 * \brief 输入子系统通用标准接口
 *
 *    当前包含按键设备接口。
 *
 *    todo: 添加其他通用接口待，如鼠标、键盘、摇杆、触摸设备等
 *
 * \par 简单示例
 * \code
 *
 * #include "am_input.h"
 *
 * (1) 按键驱动函数中上报按键事件（驱动函数实现，用户无需关心）， 如检测到KEY1键按下：
 *
 * am_input_key_report(KEY_1, AM_INPUT_KEY_STATE_PRESSED);
 *
 * (2) 应用程序中使用回调函数处理按键事件
 *
 * static void __input_key_proc (void *p_arg, int key_code, int key_state)
 * {
 *      if (key_code == KEY_1) {
 *           if (key_state == AM_INPUT_KEY_STATE_PRESSED) {
 *               am_led_on(0);
 *           } else if (key_state == AM_INPUT_KEY_STATE_RELEASED){
 *               am_led_off(0);
 *           }
 *      }
 * }
 *
 * int am_main (void)
 * {
 *     static am_input_key_handler_t key_handler;
 *
 *     am_input_key_handler_register(&key_handler, __input_key_proc, NULL);
 * }
 *
 * \endcode
 *
 * \internal
 * \par modification history
 * - 1.00 17-05-15  tee, first implementation.
 * \endinternal
 */


#ifndef __AM_INPUT_H
#define __AM_INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_input
 * \copydoc am_input.h
 * @{
 */
#include "am_common.h"
#include "am_event.h"
#include "am_input_code.h"

/**
 * \name 按键的状态
 * @{
 */

#define  AM_INPUT_KEY_STATE_PRESSED    0       /**< \brief 按键按下  */
#define  AM_INPUT_KEY_STATE_RELEASED   1       /**< \brief 按键释放  */

/** @} */

/**
 * \brief 按键回调函数类型
 *
 * \param[in] p_usr_data : 用户数据，注册时设定的用户参数
 * \param[in] key_code   : 按键编码
 * \param[in] key_state  : 按键状态，AM_INPUT_KEY_STATE_PRESSED
 */
typedef void (*am_input_key_cb_t) (void *p_arg, int key_code, int key_state);

/**
 * \brief 按键处理器
 */
typedef struct am_input_key_handler {
    am_event_handler_t    ev_handler;            /**< \brief 通用事件处理器  */
    am_input_key_cb_t     pfn_cb;                /**< \brief 回调函数  */
    void                 *p_usr_data;            /**< \brief 回调函数用户参数 */
} am_input_key_handler_t;

/**
 * \brief 上报按键事件
 *
 * \param[in] key_code   : 按键编码
 * \param[in] key_state  : 按键状态，AM_INPUT_KEY_STATE_PRESSED
 *
 * \return 标准错误号
 */
int am_input_key_report (int key_code, int key_state);

/**
 * \brief 注册一个按键处理器
 *
 * 当按键按下或释放时，均会调用  pfn_cb 指向的回调函数
 *
 * \param[in] p_handler ：  按键处理器
 * \param[in] pfn_cb    ：  按键处理回调函数
 * \param[in] p_arg     : 按键处理回调函数用户参数
 *
 * \retval   AM_OK      注册成功
 * \retval  -AM_EINVAL  参数错误
 */
int am_input_key_handler_register (am_input_key_handler_t  *p_handler,
                                   am_input_key_cb_t        pfn_cb,
                                   void                    *p_arg);

/**
 * \brief 注消一个按键处理器
 *
 * \param[in] p_handler ：  按键处理器
 *
 * \retval   AM_OK      注册成功
 * \retval  -AM_ENODEV  注册失败，按键处理器不存在（还未注册）
 * \retval  -AM_EINVAL  参数错误
 */
int am_input_key_handler_unregister (am_input_key_handler_t *p_handler);


/** @} am_if_input */

#ifdef __cplusplus
}
#endif

#endif /* __AM_INPUT_H */

/* end of file */
