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
 * \brief 蜂鸣器操作接口
 *
 * 使用本服务需要包含以下头文件:
 * \code
 * #include "am_buzzer.h"
 * \endcode
 *
 * \par 使用示例
 * \code
 * #include "am_buzzer.h"
 *
 * am_buzzer_on();      // 蜂鸣器鸣叫
 * am_buzzer_off();     // 关闭蜂鸣器鸣叫
 * am_buzzer_beep(100); // 蜂鸣器鸣叫100毫秒
 *
 * \endcode
 *
 * \internal
 * \par Modification history
 * - 1.01 14-12-09  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_BUZZER_H
#define __AM_BUZZER_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "am_common.h"
    
/**
 * \addtogroup am_if_buzzer
 * \copydoc am_buzzer.h
 * @{
 */

/**
 * \brief 打开蜂鸣器（开始鸣叫）
 *
 * \retval  AM_OK     : 打开成功
 * \retval -AM_ENODEV : 不存在蜂鸣器设备
 */ 
int am_buzzer_on(void);

/**
 * \brief 关闭蜂鸣器
 *
 * \retval  AM_OK     : 关闭成功
 * \retval -AM_ENODEV : 不存在蜂鸣器设备
 */ 
int am_buzzer_off(void);

/**
 * \brief 蜂鸣器鸣叫指定时间（时间单位：毫秒），鸣叫结束后返回
 *
 * \param[in] ms : 鸣叫时间
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_ENODEV : 不存在蜂鸣器设备
 */ 
int am_buzzer_beep(uint32_t ms);

/**
 * \brief 蜂鸣器鸣叫指定时间（时间单位：毫秒），函数会立即返回
 *
 * \param[in] ms : 鸣叫时间
 *
 * \retval  AM_OK     : 操作成功
 * \retval -AM_ENODEV : 不存在蜂鸣器设备
 */
int am_buzzer_beep_async (uint32_t ms);
 
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_BUZZER_H */

/* end of file */
