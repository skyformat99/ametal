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
 * \brief 中断延迟处理标准接口
 *
 *     实际应用中，往往希望硬件中断函数尽可能快的结束，因此，往往将中断实际需
 * 要完成的比较耗时的工作放到中断延迟中处理。
 *     中断延迟任务在添加时可以指定优先级，所有任务均被添加到任务队列中，执行
 * 一个任务时，首先从任务队列中取出一个优先级最高的任务，值得注意的是，优先级
 * 高的任务可以优先执行，但并不能打断当前正在执行的优先级较低的任务，必须等待
 * 当前任务执行完成。
 *     任务顺序执行，为了保证各个中断延迟任务也尽快完成，建议不要在中断延迟中
 * 阻塞或延时。
 *
 * 使用本服务需要包含以下头文件:
 * \code
 * #include "am_isr_defer.h"
 * \endcode
 *
 * \attention 使用该模块必须需要由具体的平台支持
 *
 * \internal
 * \par modification history:
 * - 1.00 15-09-17  tee, first implementation
 * \endinternal
 */

#ifndef __AM_ISR_DEFER_H
#define __AM_ISR_DEFER_H

/**
 * \addtogroup am_if_isr_defer
 * \copydoc am_isr_defer.h
 * @{
 */
#include "am_list.h"
#include "am_jobq.h"
#include <string.h>

/** \brief 定义中断延迟任务类型 */
typedef am_jobq_job_t am_isr_defer_job_t;

/** 
 * \brief 用于内存分配，分配后，由该指针类型获取到分配信息 
 *
 * 应用程序无需理会该类型，直接根据实际需求使用 AM_ISR_DEFER_PRIORITY_NUM_DEF() 
 * 宏定义优先级即可。
 */
struct am_isr_defer_info {
    am_jobq_queue_t       jobq_queue;
    unsigned int          priority_num;
    unsigned int         *p_bitmap_job;
    struct am_list_head   pri_heads[1]; 
};

/**
 * \brief 定义中断延迟模块使用的优先级数目
 *
 * 只需要使用该宏定义一次，job的有效优先级即为：0 ~ pri_num - 1
 *
 * \note 定义时，必须定义为全局的，存放在.c文件中。
 */
#define AM_ISR_DEFER_PRIORITY_NUM_DEF(pri_num)                      \
    struct __isr_defer_jobqinfo{                                    \
        am_jobq_queue_t       jobq_queue;                           \
        unsigned int          priority_num;                         \
        unsigned int         *p_bitmap_job;                         \
        struct am_list_head   pri_heads[pri_num];                   \
        unsigned int          bitmap_job[((pri_num) + 31) >> 5];    \
    };                                                              \
    static struct __isr_defer_jobqinfo __isr_defer_jobq_info;       \
    static struct __isr_defer_jobqinfo __isr_defer_jobq_info =      \
                 {{0}, pri_num,__isr_defer_jobq_info.bitmap_job,};  \
    struct am_isr_defer_info *gp_isr_defer_info =         \
                 (struct am_isr_defer_info *)&__isr_defer_jobq_info
            
/**
 * \brief 触发回调函数类型
 */
typedef void (*am_isr_defer_add_cb_t) (void *p_arg);
 
/**
 * \brief 中断延迟模块初始化，使用中断延迟前必须使用该函数初始化
 *
 * \param[in] pfn_cb : 添加延迟作业时的回调函数，每次使用 am_isr_defer_job_add()
 *                     添加新的延迟作业时，都会调用该函数
 * \param[in] p_arg  : 回调函数用户参数
 *
 * \return 无
 *
 * \note 通常情况下，会使用一个优先级最低的软件中断来处理中断延迟作业，这就需
 * 要在每次添加中断延迟作业时触发一次相应软件中断，触发动作就可以放在该回调函
 * 数中。特别地，若中断延迟作业放在主循环中处理，无需任何其他触发操作，则pfn_cb
 * 可以设置为NULL。
 *
 * \attention 在初始化前，务必使用 AM_ISR_DEFER_PRIORITY_NUM_DEF() 定义优先级数目
 */
void am_isr_defer_init (am_isr_defer_add_cb_t pfn_cb, void *p_arg);

/**
 * \brief 中断延迟任务处理
 *
 * 一般来讲，可以在主循环中调用该函数（主循环没有其它事务处理时），或 在一个优
 * 先级最低的软件中断中处理。
 */
int am_isr_defer_job_process (void);

/**
 * \brief 初始化一个延迟任务
 *
 * 在将一个任务加入到队列之前，确保使用该函数将一个任务初始化
 *
 * \param[in] p_job : 指向任务的指针
 * \param[in] func  : 任务对应的处理函数
 * \param[in] p_arg : 处理函数对应的用户参数
 * \param[in] pri   : 任务的优先级
 *
 * \retval AM_OK      ：初始化成功
 * \retval -AM_EINVAL : 参数错误
 *
 * \note 使用该函数前，往往先使用 am_jobq_job_t 定义一个job。再使用该函数初始化
 *       其中的各个成员。由于任务实际处理是在队列处理时，因此定义时必须定义为
 *       全局变量或静态变量，
 */
void am_isr_defer_job_init (am_isr_defer_job_t  *p_job,
                            am_pfnvoid_t         func,
                            void                *p_arg,
                            uint16_t             pri);
                            
/**
 * \brief 将一个任务添加到中断延迟工作队列中
 *
 * \param[in] p_job   : 指向任务的指针
 *
 * \retval AM_OK      ：加入成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EBUSY  : 表明当前任务已经在队列中，无法重复添加
 * \retval -AM_EPERM  : isr defer模块未正确初始化，不能使用
 */
int am_isr_defer_job_add (am_isr_defer_job_t *p_job);

/** @}  */

#ifdef __cplusplus
}
#endif  /* __cplusplus  */

#endif  /* __AM_ISR_DEFER_H */

/* end of file */

