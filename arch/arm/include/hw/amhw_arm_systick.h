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
 * \brief CPU 系统滴答定时器(SYSTICK) 操作接口
 *
 * 1. 简单的24位递减定时器；
 * 2. 使用专用的异常向量号。
 *
 * \internal
 * \par Modification history
 * - 1.00 16-9-18  mkr, first implementation.
 * \endinternal
 */

#ifndef __AMHW_ARM_SYSTICK_H
#define __AMHW_ARM_SYSTICK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup amhw_arm_if_systick
 * \copydoc amhw_arm_systick.h
 * @{
 */

/**
 * \brief SYSTICK 寄存器块结构体
 */
typedef struct amhw_arm_systick {
    __IO uint32_t ctrl;       /**< \brief SysTick 控制和状态寄存器 */
    __IO uint32_t load;       /**< \brief SysTick 重装载值寄存器 */
    __IO uint32_t val;        /**< \brief SysTick 当前定时器值寄存器 */
    __I  uint32_t calib;      /**< \brief SysTick 校准寄存器 */
} amhw_arm_systick_t;

#ifndef AMHW_ARM_SYSTICK
#define AMHW_ARM_SYSTICK  ((amhw_arm_systick_t *)0xE000E010UL)    /**< SCB  SysTick基地址 */
#endif


/**
 * \name Systick控制寄存器的宏定义
 * @{
 */

/** \brief Systick最大值 */
#define AMHW_ARM_SYSTICK_INVAL_MAX                     (0xFFFFFFUL)

/** \brief Systick使能 */
#define AMHW_ARM_SYSTICK_CONFIG_ENABLE                 (1 << 0)

/** \brief Systick中断使能 */
#define AMHW_ARM_SYSTICK_CONFIG_TICKINT                (1 << 1)

/** \brief Systick时钟源为系统时钟 */
#define AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM          (1 << 2)

/** \brief Systick时钟源的为系统时钟的1/2 */
#define AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM_HALF     (0 << 2)

/**
 * \brief Systick时钟源为Systick时钟分频器的输出时钟
 */
#define AMHW_ARM_SYSTICK_CONFIG_CLKSRC_MAINCLK_BY_DIV  (0 << 2)

/** @} */

/**
 * \brief 使能Systick（开始向下计数）
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_arm_systick_enable (amhw_arm_systick_t *p_hw_systick)
{
    p_hw_systick->ctrl |= AMHW_ARM_SYSTICK_CONFIG_ENABLE;
}

/**
 * \brief 禁能Systick（停止向下计数）
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_arm_systick_disable (amhw_arm_systick_t *p_hw_systick)
{
    p_hw_systick->ctrl &= ~AMHW_ARM_SYSTICK_CONFIG_ENABLE;
}

/**
 * \brief 使能Systick中断
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_arm_systick_int_enable (amhw_arm_systick_t *p_hw_systick)
{
    p_hw_systick->ctrl |= AMHW_ARM_SYSTICK_CONFIG_TICKINT;
}

/**
 * \brief 禁能Systick中断
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \return 无
 */
am_static_inline
void amhw_arm_systick_int_disable (amhw_arm_systick_t *p_hw_systick)
{
    p_hw_systick->ctrl &= ~AMHW_ARM_SYSTICK_CONFIG_TICKINT;
}

/**
 * \brief 检测Systick计数值是否递减至0
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \retval AM_TRUE  : Systick计数值递减至0
 * \retval AM_FALSE : Systick计数值未递减至0
 */
am_static_inline
am_bool_t amhw_arm_systick_flag_check (amhw_arm_systick_t *p_hw_systick)
{
    return (am_bool_t)((p_hw_systick->ctrl & (1 << 16)) != 0);
}

/**
 * \brief 配置Systick
 *
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \param[in] flags        : 可以是下列单个宏值或下列多个宏的或值
 *                            - AMHW_ARM_SYSTICK_CONFIG_ENABLE
 *                            - AMHW_ARM_SYSTICK_CONFIG_TICKINT
 *                            - AMHW_ARM_SYSTICK_CONFIG_CLKSRC_SYSTEM
 *                            - AMHW_ARM_SYSTICK_CONFIG_CLKSRC_MAINCLK_BY_DIV
 * \return 无
 */
am_static_inline
void amhw_arm_systick_config (amhw_arm_systick_t *p_hw_systick, uint32_t flags)
{
    p_hw_systick->ctrl = flags;
}

/**
 * \brief 设置Systick的自动重装载值
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \param[in] value        : 自动重装载值(最大为0xFFFFFF)
 * \return 无
 */
am_static_inline
void amhw_arm_systick_reload_val_set (amhw_arm_systick_t *p_hw_systick, uint32_t value)
{
    p_hw_systick->load = value & 0xFFFFFF;
}

/**
 * \brief 获取Systick的自动重装载值
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \return 自动装载值
 */
am_static_inline
uint32_t amhw_arm_systick_reload_val_get (amhw_arm_systick_t *p_hw_systick)
{
    return ((p_hw_systick->load) & 0xFFFFFF);
}

/**
 * \brief 设置Systick当前的计数值
 *
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \param[in] value        : Systick当前的计数值
 *
 * \return 无
 *
 * \note 写任何值都将清零Systick当前的计数值，并清除控制寄存器中的COUNTFLAG标志。
 */
am_static_inline
void amhw_arm_systick_val_set (amhw_arm_systick_t *p_hw_systick, uint32_t value)
{
    p_hw_systick->val = value & 0xFFFFFF;
}

/**
 * \brief 获取Systick当前的计数值
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \return Systick当前的计数值
 */
am_static_inline
uint32_t amhw_arm_systick_val_get (amhw_arm_systick_t *p_hw_systick)
{
    return (p_hw_systick->val & 0xFFFFFF);
}

/**
 * \brief 获取Systick的校准值
 * \param[in] p_hw_systick : 指向Systick寄存器块的指针
 * \return Systick的校准值
 */
am_static_inline
uint32_t amhw_arm_systick_calib_val_get (amhw_arm_systick_t *p_hw_systick)
{
    return p_hw_systick->calib;
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /*__AMHW_ARM_SYSTICK_H */

/*end of file */
