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
 * \brief 可嵌套向量中断控制器(NVIC) 硬件操作接口
 *
 * \internal
 * \par Modification history
 * - 1.00 17-04-10  sdy, first implementation.
 * \endinternal
 */


#ifndef __AMHW_ARM_NVIC_H
#define __AMHW_ARM_NVIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"

/**
 * \addtogroup amhw_arm_if_nvic
 * \copydoc amhw_arm_nvic.h
 * @{
 */

/**
 * \brief  Structure type to access the Nested Vectored Interrupt Controller (NVIC).
 */
typedef struct
{
    __IO uint32_t iser[8];         /**< Interrupt Set Enable Register           */
         uint32_t reserved0[24];   /**< reserved                                */
    __IO uint32_t icer[8];         /**< Interrupt Clear Enable Register         */
         uint32_t reserved1[24];   /**< reserved                                */
    __IO uint32_t ispr[8];         /**< Interrupt Set Pending Register          */
         uint32_t reserved2[24];   /**< reserved                                */
    __IO uint32_t icpr[8];         /**< Interrupt Clear Pending Register        */
         uint32_t reserved3[24];   /**< reserved                                */
    __IO uint32_t iabr[8];         /**< Interrupt Active bit Register           */
         uint32_t reserved4[56];   /**< reserved                                */
         
         /** \brief Interrupt Priority Register union type */
         union ip {
             __IO uint8_t  ip_uint8[240];    /**< 8-Bit wide */
             __IO uint32_t ip_uint32[60];    /**< 32-Bit wide */
         }ip_t;                    /**< \brief Interrupt Priority Register */

         uint32_t reserved5[644];  /**< reserved                                */
    __O  uint32_t stir;            /**< Software Trigger Interrupt Register     */
}amhw_arm_nvic_t;

/**
 * \brief  Structure type to access the System Control Block (SCB).
 */
typedef struct
{
    __I  uint32_t cpuid;      /**< CPUID Base Register                                   */
    __IO uint32_t icsr;       /**< Interrupt Control and State Register                  */
    __IO uint32_t VTOR;       /**< Vector Table Offset Register                          */
    __IO uint32_t aircr;      /**< Application Interrupt and Reset Control Register      */
    __IO uint32_t scr;        /**< System Control Register                               */
    __IO uint32_t ccr;        /**< Configuration Control Register                        */
    
    /** \brief System Handlers Priority Registers union type */
    union shp {
        __IO uint8_t shp_uint8[12];   /**< Access as 8-bit   */
        __IO uint32_t shp_uint32[3];  /**< Access as 32-bit   */
    }shp_t;                           /**< System Handlers Priority Registers */

    __IO uint32_t shcsr;      /**< System Handler Control and State Register             */
} amhw_arm_scb_t;

/* Interrupt Priorities are WORD accessible only under ARMv6M                   */
/* The following MACROS handle generation of the register offset and byte masks */
#define __BIT_SHIFT(IRQn)  (  (((uint32_t)(IRQn)       )    &  0x03) * 8 ) /**< \brief __BIT_SHIFT */
#define __SHP_IDX(IRQn)    ( ((((uint32_t)(IRQn) & 0x0F)-4) >>    2)     ) /**< \brief __SHP_IDX */
#define __IP_IDX(IRQn)     (   ((uint32_t)(IRQn)            >>    2)     ) /**< \brief __IP_IDX */


#ifndef AMHW_ARM_NVIC
#define AMHW_ARM_NVIC  ((amhw_arm_nvic_t *)0xE000E100UL)  /**< NVIC configuration struct */
#endif

#ifndef AMHW_ARM_SCB
#define AMHW_ARM_SCB  ((amhw_arm_scb_t *)0xE000ED00UL)    /**< SCB  configuration struct */
#endif

/**
 * \brief 获取当前中断号
 * \return 中断号
 */
am_static_inline 
int amhw_inum_get (void)
{
    return (int)((AMHW_ARM_SCB->icsr - 16 ) & 0xff);
}

/**
 * \brief 使能中断
 *
 * \param[in] inum     : 中断号
 *
 * \return 无
 */
am_static_inline 
void amhw_arm_nvic_enable (int inum)
{
    AMHW_ARM_NVIC->iser[(uint32_t)((int32_t)inum) >> 5] =
            (uint32_t)(1 << ((uint32_t)((int32_t)inum) & (uint32_t)0x1F));
}

/**
 * \brief 禁能中断
 *
 * \param[in] inum     : 中断号
 *
 * \return 无
 */
am_static_inline 
void amhw_arm_nvic_disable (int inum)
{
    AMHW_ARM_NVIC->icer[((uint32_t)(inum) >> 5)] = (1 << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief 置位中断等待
 *
 * \param[in] inum     : 中断号
 *
 * \return    无
 */
am_static_inline 
void amhw_arm_nvic_pending_set (int inum)
{
    AMHW_ARM_NVIC->ispr[((uint32_t)(inum) >> 5)] = (1 << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief 清除等待中断
 *
 * \param[in] inum     : 中断号
 *
 * \return    无
 */
am_static_inline 
void amhw_arm_nvic_pending_clr (int inum)
{
    AMHW_ARM_NVIC->icpr[((uint32_t)(inum) >> 5)] = (1 << ((uint32_t)(inum) & 0x1F));
}

/**
 * \brief 设置中断优先级
 *
 * \param[in] inum      : 中断号
 * \param[in] prio      : 中断优先级
 * \param[in] prio_bits : 优先级位数.
 * 
 * \return   无
 */
am_static_inline 
void amhw_arm_nvic_m0_priority_set (int inum, uint32_t prio, uint8_t prio_bits)
{
    prio_bits &= 0x7;
    if(inum < 0) {
        AMHW_ARM_SCB->shp_t.shp_uint32[__SHP_IDX(inum)] =
            (AMHW_ARM_SCB->shp_t.shp_uint32[__SHP_IDX(inum)] & ~(0xFF << __BIT_SHIFT(inum))) |
            (((prio << (8 - prio_bits)) & 0xff) << __BIT_SHIFT(inum));
    } else {
        AMHW_ARM_NVIC->ip_t.ip_uint32[__IP_IDX(inum)] =
            (AMHW_ARM_NVIC->ip_t.ip_uint32[__IP_IDX(inum)] & ~(0xFF << __BIT_SHIFT(inum))) |
            (((prio << (8 - prio_bits)) & 0xff) << __BIT_SHIFT(inum));
    }
}

/**
 * \brief 设置中断优先级
 *
 * \param[in] inum      : 中断号
 * \param[in] prio      : 中断优先级
 * \param[in] prio_bits : 优先级位数.
 *
 * \return   无
 */
am_static_inline
void amhw_arm_nvic_m3m4_priority_set (int inum, uint32_t prio, uint8_t prio_bits)
{
    prio_bits &= 0x7;
    if(inum < 0) {
        AMHW_ARM_SCB->shp_t.shp_uint8[((uint32_t)(inum) & 0xF)-4] = ((prio << (8 - prio_bits)) & 0xff);
    } else {
        AMHW_ARM_NVIC->ip_t.ip_uint8[(uint32_t)(inum)] = ((prio << (8 - prio_bits)) & 0xff);
    }
}

/**
 * \brief 获取中断有效状态
 *
 * \param[in] inum     : 中断号
 * 
 * \retval TRUE  : 有效中断
 * \retval FALSE : 无效中断
 */
am_static_inline 
am_bool_t amhw_arm_nvic_active_state_get (int inum)
{
    return((uint8_t)((AMHW_ARM_NVIC->iabr[(uint32_t)(inum) >> 5] & (1 << ((uint32_t)(inum) & 0x1F)))?1:0));
}

/**
 * \brief 设置组中断优先级
 *
 * \param[in] group   : 组中断
 * 
 * \return   无
 */
am_static_inline 
void amhw_arm_nvic_priority_group_set (uint32_t group)
{
    uint32_t reg_value;
    uint32_t priority_group_tmp = (group & (uint32_t)0x07);    /* only values 0..7 are used          */

    reg_value  =  AMHW_ARM_SCB->aircr;                         /* read old register configuration    */
    reg_value &= ~((0xFFFFul << 16) | (7ul << 8));             /* clear bits to change               */
    reg_value  =  (reg_value              |
                  ((uint32_t)0x5FA << 16) |
                  (priority_group_tmp << 8));                  /* Insert write key and priorty group */
    AMHW_ARM_SCB->aircr =  reg_value;
}


/**
 * \brief	编码优先级寄存器值
 *
 * \param[in]	priority_group   : 优先级组
 * \param[in]   preempt_priority : 抢占优先级.
 * \param[in]   sub_priority     : 次优先级.
 * \param[in]   prio_bits        : 优先级位数.
 *
 * \return	优先级寄存器值
 *
 * \note
 */
am_static_inline
uint32_t amhw_arm_nvic_encode_priority (uint32_t priority_group,
                                        uint32_t preempt_priority,
                                        uint32_t sub_priority,
                                        uint8_t  prio_bits)
{
  uint32_t priority_group_tmp = (priority_group & 0x07);          /* only values 0..7 are used          */
  uint32_t preempt_priority_bits;
  uint32_t sub_priority_bits;

  preempt_priority_bits = ((7 - priority_group_tmp) > prio_bits) ? prio_bits : 7 - priority_group_tmp;
  sub_priority_bits     = ((priority_group_tmp + prio_bits) < 7) ? 0 : priority_group_tmp - 7 + prio_bits;

  return (
           ((preempt_priority & ((1 << (preempt_priority_bits)) - 1)) << sub_priority_bits) |
           ((sub_priority     & ((1 << (sub_priority_bits    )) - 1)))
         );
}
/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AMHW_ARM_NVIC_H */

/* end of file */
