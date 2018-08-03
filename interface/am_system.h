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
 * \brief 系统相关服务
 *
 * 只要包含了 ametal.h 就可以使用本服务。
 *
 * \par 简单示例:计算程序执行时间
 * \code
 *  #include "ametal.h"
 *
 *  int main(){
 *      unsigned int tick;
 *
 *      tick = am_sys_tick_get();
 *      //  do something
 *      tick = am_sys_tick_get() - tick;
 *      am_kprintf("tick used=%d\n , time=%d ms",
 *                  tick,
 *                  am_ticks_to_ms(tick));
 *      return 0;
 *  }
 * \endcode
 *
 * //更多内容待添加。。。
 *
 * \internal
 * \par modification history:
 * - 1.00 13-02-27  zen, first implementation
 * \endinternal
 */

#ifndef __AM_SYSTEM_H
#define __AM_SYSTEM_H

#include "am_types.h"

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus	*/

/**
 * \addtogroup am_if_system
 * \copydoc am_system.h
 * @{
 */

/**
 * \brief 系统模块初始化（一般在系统启动时调用）
 * \param[in] clkrate : 运行频率，必须按照该频率调用  am_system_module_tick()函数。
 * \retval AM_OK     : 模块初始化成功，可以正常使用
 */
int am_system_module_init (unsigned long clkrate);

/**
 * \brief 一般在某一定时器中断中周期性的调用该函数
 */
int am_system_module_tick (void);

/**
 * \brief 取得系统时钟节拍频率
 *
 * \return 系统时钟节拍频率, 类型为 unsigned long
 */
unsigned long am_sys_clkrate_get (void);

/**
 * \brief 取得系统当前的节拍计数值,
 *
 * 系统启动后，节拍计数值从0开始，每过1个tick增加1 。系统节拍的频率可调用
 * am_sys_clkrate_get() 获取。
 *
 * \return 系统当前的节拍计数值, 类型为 unsigned int
 *
 * \par 示例:计算程序执行时间
 * \code
 *  #include "ametal.h"
 *
 *  int main(){
 *      unsigned int tick;
 *
 *      tick = am_sys_tick_get();
 *      //  do something
 *      tick = am_sys_tick_get() - tick;
 *      am_kprintf("tick used=%d\n , time=%d ms",
 *                  tick,
 *                  am_ticks_to_ms(tick));
 *      return 0;
 *  }
 * \endcode
 *
 * // 更多内容待添加。。。
 */
am_tick_t am_sys_tick_get (void);

/**
 * \brief 计算两个时间tick的差值
 *
 *  不需要考虑反转的问题: 负数的表示机制能自动避免反转问题，如0xfffffff即表示无符号的最大值(32bit)，
 *  也表示有符号数的-1.
 * \return 系统当前时间戳值(时间戳定时器的计数值)
 */
static am_inline am_tick_t am_sys_tick_diff(am_tick_t __t0, am_tick_t __t1)
{
    return __t1 - __t0;
}

/**
 * \brief 转换毫秒时间为系统时钟节拍个数
 * \param ms 毫秒时间
 * \return 毫秒时间对应的节拍个数
 * \note 毫秒数不足一个节拍的将补齐一个节拍。如一个节拍为5毫秒，则1~5毫秒都将
 *      转换为1个节拍。
 * \par 示例
 * \code
 * #include "apollo.h"
 *
 * am_task_delay(am_ms_to_ticks(500));    // 延时500ms
 * \endcode
 */
am_tick_t am_ms_to_ticks (unsigned int ms);

/**
 * \brief 转换系统时钟节拍个数为毫秒时间
 * \param ticks 节拍个数
 * \return  系统时钟节拍数对应的毫秒时间
 *
 * \par 示例:计算程序执行时间
 * \code
 *  #include "apollo.h"
 *
 *  int main(){
 *      unsigned int tick;
 *
 *      tick = am_sys_tick_get();
 *      //  do something
 *      tick = am_sys_tick_get() - tick;
 *      am_kprintf("tick used=%d\n , time=%d ms",
 *                  tick,
 *                  am_ticks_to_ms(tick));
 *      return 0;
 *  }
 * \endcode
 */
unsigned int am_ticks_to_ms (am_tick_t ticks);

 
/**
 * \brief 获取当前处理器核ID
 *
 * \return 处理器核ID
 */
uint32_t am_core_id_self(void);

/**
 * \brief 获取处理器本地地址对应的全局地址 (物理地址, DMA等总线主机外设使用此地址)
 *
 * \return 全局地址
 */
void *am_global_addr_get(void *p_addr);

/** @} am_if_system */

#ifdef __cplusplus
}
#endif  /* __cplusplus  */

#endif    /* __AM_SYSTEM_H    */

/* end of file */
