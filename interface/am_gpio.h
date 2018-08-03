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
 * \brief GPIO标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-01-01  hbt, first implementation.
 * \endinternal
 */

#ifndef __AM_GPIO_H
#define __AM_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h" 
#include "am_gpio_util.h"

/**
 * \addtogroup am_if_gpio
 * \copydoc am_gpio.h
 * @{
 */

/**
 * \name GPIO 引脚功能
 * \anchor grp_am_gpio_pin_function
 * @{
 */

/** \brief GPIO 引脚输入 */
#define AM_GPIO_INPUT                AM_GPIO_COM_FUNC_CODE(AM_GPIO_INPUT_VAL)

/** \brief GPIO 引脚输出高电平 */
#define AM_GPIO_OUTPUT_INIT_HIGH     AM_GPIO_COM_FUNC_CODE(AM_GPIO_OUTPUT_INIT_HIGH_VAL)

/** \brief GPIO 引脚输出低电平 */
#define AM_GPIO_OUTPUT_INIT_LOW      AM_GPIO_COM_FUNC_CODE(AM_GPIO_OUTPUT_INIT_LOW_VAL)

/** @} */


/**
 * \name GPIO 引脚模式
 * \anchor grp_am_gpio_pin_mode
 * @{
 */

/** \brief 上拉模式 */
#define AM_GPIO_PULLUP               AM_GPIO_COM_MODE_CODE(AM_GPIO_PULL_UP_VAL)

/** \brief 下拉模式 */
#define AM_GPIO_PULLDOWN             AM_GPIO_COM_MODE_CODE(AM_GPIO_PULL_DOWN_VAL)

/** \brief 浮动模式 */
#define AM_GPIO_FLOAT                AM_GPIO_COM_MODE_CODE(AM_GPIO_FLOAT_VAL)

/** \brief 开漏模式 */
#define AM_GPIO_OPEN_DRAIN           AM_GPIO_COM_MODE_CODE(AM_GPIO_OPEN_DRAIN_VAL)

/** \brief 推挽模式 */
#define AM_GPIO_PUSH_PULL            AM_GPIO_COM_MODE_CODE(AM_GPIO_PUSH_PULL_VAL)

/** @} */


/**
 * \name GPIO 引脚触发功能
 * \anchor grp_am_gpio_pin_trigger_function
 * @{
 */

/** \brief 关闭触发 */
#define AM_GPIO_TRIGGER_OFF          0

/** \brief 高电平触发 */
#define AM_GPIO_TRIGGER_HIGH         1

/** \brief 低电平触发 */
#define AM_GPIO_TRIGGER_LOW          2

/** \brief 上升沿触发 */
#define AM_GPIO_TRIGGER_RISE         3

/** \brief 下降沿触发 */
#define AM_GPIO_TRIGGER_FALL         4

/** \brief 双边沿触发 */
#define AM_GPIO_TRIGGER_BOTH_EDGES   5

/** @} */

/**
 * \name GPIO 引脚电平
 * \anchor grp_am_gpio_pin_level
 * @{
 */
 
/** \brief 低电平 */
#define AM_GPIO_LEVEL_LOW            0

/** \brief 高电平 */
#define AM_GPIO_LEVEL_HIGH           1

/** @} */


/**
 * \brief 配置GPIO引脚功能
 *
 * 通过参数 \a flags 配置引脚功能 \a flags 的格式为:
 * (功能 | 模式 | 平台), 
 * - 功能：
 *   - \ref grp_am_gpio_pin_function
 * - 模式：
 *   - \ref grp_am_gpio_pin_mode
 * - 平台：
 *   - xxx_pin.h 
 *
 * 如果配置引脚的功能和模式与平台的配置冲突，功能域和模式域的优先级最高，而
 * 忽略平台的相关配置。
 * 
 * \param[in] pin      : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] flags    : 配置参数，参见
 *                       \ref grp_am_gpio_pin_function 和
 *                       \ref grp_am_gpio_pin_mode 和 xxx_pin.h
 * 
 * \retval  AM_OK      : 配置成功
 * \retval -AM_ENOTSUP : 不支持该功能
 */
int am_gpio_pin_cfg(int pin, uint32_t flags);

/**
 * \brief 获取GPIO引脚状态
 * \param[in] pin : 引脚编号，值为 PIO* (#PIO0_0)
 * \return          引脚状态
 */
int am_gpio_get(int pin);

/**
 * \brief 设置引脚输出状态
 *
 * \param[in] pin   : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] value : 引脚状态，参见
 *                    \ref grp_am_gpio_pin_level
 *
 * \retval  AM_OK   : 操作成功
 */
int am_gpio_set(int pin, int value);

/**
 * \brief 翻转GPIO引脚输出状态
 * \param[in] pin : 引脚编号，值为 PIO* (#PIO0_0)
 * \retval  AM_OK : 操作成功
 */
int am_gpio_toggle(int pin);

/**
 * \brief 配置GPIO引脚触发功能
 * 
 * \param[in] pin  : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] flag : 配置参数，参见
 *                   \ref grp_am_gpio_pin_trigger_function
 * \note:使用该接口，应选调用am_gpio_pin_cfg设置gpio的引脚为输入并且选择合适的引脚模式
 * 
 * \retval AM_OK   : 配置成功
 */
int am_gpio_trigger_cfg(int pin, uint32_t flag);

/**
 * \brief 连接回调函数到引脚
 *
 * \param[in] pin          : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] pfn_callback : 回调函数指针
 * \param[in] p_arg        : 回调函数的入口参数
 *
 * \retval  AM_OK          : 操作成功
 */
int am_gpio_trigger_connect(int           pin,
                            am_pfnvoid_t  pfn_callback,
                            void         *p_arg);

/**
 * \brief 删除连接到引脚的回调函数
 *
 * \param[in] pin          : 引脚编号，值为 PIO* (#PIO0_0)
 * \param[in] pfn_callback : 回调函数指针
 * \param[in] p_arg        : 回调函数的入口参数
 *
 * \retval  AM_OK          : 操作成功
 */
int am_gpio_trigger_disconnect(int           pin,
                               am_pfnvoid_t  pfn_callback,
                               void         *p_arg);

/**
 * \brief 使能引脚触发中断
 * \param[in] pin : 引脚编号，值为 PIO* (#PIO0_0)
 * \retval  AM_OK : 操作成功
 */
int am_gpio_trigger_on(int pin);

/**
 * \brief 禁能引脚触发中断
 * \param[in] pin : 引脚编号，值为 PIO* (#PIO0_0)
 * \retval  AM_OK : 操作成功
 */
int am_gpio_trigger_off(int pin);


/** 
 * @}  
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_GPIO_H */

/* end of file */
