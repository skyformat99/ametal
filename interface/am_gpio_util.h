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
 * \brief GPIO标准接口的辅助工具宏
 *
 * 该文件只被 \sa am_gpio.h 包含，而且不能被修改 该文件定义了引脚通用的功能和
 * 模式。
 *
 * \internal
 * \par Modification History
 * - 1.00 15-02-03  hbt, first implementation.
 * \endinternal
 */

#ifndef __AM_GPIO_UTIL_H
#define __AM_GPIO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * \addtogroup am_if_gpio_util
 * \copydoc am_gpio_util.h
 * @{
 */

/**
 * \name bit[2:0] 通用功能码位域
 * @{
 */

#define AM_GPIO_FUNCBITS_START          0       /**< \brief 功能码起始位   */
#define AM_GPIO_FUNCBITS_LEN            3       /**< \brief 功能码长度     */

#define AM_GPIO_FUNC_INVALID_VAL        0x0     /**< \brief 无效功能       */
#define AM_GPIO_INPUT_VAL               0x1     /**< \brief 输入功能       */
#define AM_GPIO_OUTPUT_VAL              0x2     /**< \brief 输出功能       */
#define AM_GPIO_OUTPUT_INIT_HIGH_VAL    0x3     /**< \brief 初始化为高电平 */
#define AM_GPIO_OUTPUT_INIT_LOW_VAL     0x4     /**< \brief 初始化为低电平 */
 
/** \brief 置功能码 */
#define AM_GPIO_COM_FUNC_CODE(value)    AM_SBF((value), AM_GPIO_FUNCBITS_START)

/** \brief 获取功能码 */
#define AM_GPIO_COM_FUNC_GET(data)      AM_BITS_GET((data), AM_GPIO_FUNCBITS_START, \
                                                     AM_GPIO_FUNCBITS_LEN)

/** \brief 无效功能 */
#define AM_GPIO_COM_FUNC_INVALID        AM_GPIO_COM_FUNC_CODE(AM_GPIO_FUNC_INVALID_VAL)

/** @} */

/** 
 * \name bit[5:3] 通用模式码位域
 * @{
 */

#define AM_GPIO_MODEBITS_START          3       /**< \brief 模式码起始位 */
#define AM_GPIO_MODEBITS_LEN            3       /**< \brief 模式码长度   */

#define AM_GPIO_MODE_INVALID_VAL        0x0     /**< \brief 无效模式     */
#define AM_GPIO_PULL_UP_VAL             0x1     /**< \brief 上拉模式     */
#define AM_GPIO_PULL_DOWN_VAL           0x2     /**< \brief 下拉模式     */
#define AM_GPIO_FLOAT_VAL               0x3     /**< \brief 浮空模式     */
#define AM_GPIO_OPEN_DRAIN_VAL          0x4     /**< \brief 开漏模式     */
#define AM_GPIO_PUSH_PULL_VAL           0x5     /**< \brief 推挽模式     */

/** \brief 置模式码 */
#define AM_GPIO_COM_MODE_CODE(value)    AM_SBF((value), AM_GPIO_MODEBITS_START)

/** \brief 获取模式码 */
#define AM_GPIO_COM_MODE_GET(data)      AM_BITS_GET((data), AM_GPIO_MODEBITS_START, \
                                                    AM_GPIO_MODEBITS_LEN)

/** \brief 无效模式 */
#define AM_GPIO_COM_MODE_INVALID        AM_GPIO_COM_MODE_CODE(AM_GPIO_MODE_INVALID_VAL)

/** @} */


/**
 * \name 平台定义
 *
 * bit[11:6] 保留
 * bit[31:12] 用于平台定义
 *
 * @{
 */

#define AM_GPIO_PLFM_BITS_START          12     /**< \brief 平台定义起始位 */
#define AM_GPIO_PLFM_BITS_LEN            20     /**< \brief 平台定义长度   */

/** @} */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* __AM_GPIO_UTIL_H */

/* end of file */
