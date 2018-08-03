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
 * \brief 任务队列
 *
 *      可以将多个待处理的任务放到一个队列中，在合适的时机再开始处理这些任务，
 *  常用于中断事件的处理中。通常情况下，中断函数应该尽可能快的结束，更不应该
 *  出现阻塞式语句（如延时、查询等待等）。此时，就可以将中断中要处理的事情先
 *  存放到任务队列中，然后在非中断环境下，再调用任务队列处理函数，处理其中的
 *  任务。
 *      特别地，向任务队列中添加任务时，可以指定优先级，优先级高的任务将会优先
 *  处理（调用任务队列处理函数时）。值得注意的是，这仅仅只是优先处理优先级高的
 *  任务，并不能打断优先级低的任务。
 *
 * 使用本服务需要包含以下头文件:
 * \code
 * #include "am_jobq.h"
 * \endcode
 *
 * \internal
 * \par modification history:
 * - 1.00 15-09-17  tee, first implementation
 * \endinternal
 */

#ifndef __AM_JOBQ_H
#define __AM_JOBQ_H

/**
 * \addtogroup am_if_jobq
 * \copydoc am_jobq.h
 * @{
 */
#include "am_list.h"
#include <string.h>


/**
 * \brief 定义任务队列结构体
 */
typedef struct am_jobq_queue {
    
    /** \brief 每个优先级对应一个链表头    */
    struct am_list_head  *p_heads;
    
    /** \brief 任务队列的优先级数目，最大优先级数目为1024   */
    unsigned int          pri_num;
    
     /** \brief 每位表示一个组，为 1 表示该组内有任务       */
    unsigned int          bitmap_grp;
    
    /** \brief 一个组内有32个任务，位为 1 表示该优先级有任务 */
    unsigned int         *p_bitmap_job;
 
    /** \brief 任务队列的一些状态标志      */
    uint32_t              flags;
} am_jobq_queue_t;

/**
 * \brief 定义标准任务队列的标准服务句柄，该句柄可由初始化函数获得 
 */
typedef struct am_jobq_queue *am_jobq_handle_t;

/** 
 * \brief 定义任务结构体，应用程序不应直接操作结构体成员
 *
 * 使用该类型定义一个job后，应调用am_jobq_mkjob()函数设置其中的各个成员
 */
typedef struct am_jobq_job {
    am_pfnvoid_t        func;   /**< \brief 该任务对应的处理函数 */
    void               *p_arg;  /**< \brief 处理函数对应的参数   */
    uint16_t            pri;    /**< \brief 优先级               */
    uint16_t            flags;  /**< \brief 一些标志             */
    struct am_list_head node;   /**< \brief 链表节点             */
} am_jobq_job_t;

/**
 * \brief 初始化一个任务队列
 *
 * \param[in] p_jobq_queue : 指向任务队列的指针
 * \param[in] pri_num      : 任务队列使用的优先级数目
 *                             - 实际任务的有效优先级为：0 ~ priority_num - 1
                               - 值越大，优先级越低，0为最高优先级
 * \param[in] p_heads      : 内存分配，大小必须为： priority_num 
 * \param[in] p_bitmap_job : 内存分配，大小必须为： priority_num / 32 向上取整，
 *                           即：(priority_num + 31) / 32
 *
 * \return 任务队列的操作句柄，如果初始化失败，则值为NULL
 *
 * \par 范例
 * \code
 *
 *  #define MY_QJOB_NUM_PRI  64    // 优先级数目64
 *
 *  am_jobq_queue_t      myqjob_queue;
 *  struct am_list_head  myqjob_head[MY_QJOB_NUM_PRI];
 *  unsigned int         myqjob_pri_active[(MY_QJOB_NUM_PRI + 31) / 32];
 *
 *  am_jobq_queue_init(&myqjob_queue,
 *                     MY_QJOB_NUM_PRI,
 *                     myqjob_head,
 *                     myqjob_pri_active);
 * \endcode
 *
 * \note 由于该函数需要内存分配，如果不熟悉，则不建议直接使用该函数。可以使用更
 *       为简单的宏同样得到一个handle。即使用 AM_JOBQ_QUEUE_DECL()定义任务队列。
 *       再使用 AM_JOBQ_QUEUE_INIT() 初始化任务队列。
 *
 * \sa AM_JOBQ_QUEUE_DECL(), AM_JOBQ_QUEUE_INIT()
 */
am_jobq_handle_t am_jobq_queue_init (am_jobq_queue_t     *p_jobq_queue,
                                     unsigned int         pri_num,
                                     struct am_list_head *p_heads,
                                     unsigned int        *p_bitmap_job);

/**
 * \brief 初始化一个任务
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
void am_jobq_job_init (am_jobq_job_t  *p_job,
                       am_pfnvoid_t    func,
                       void           *p_arg,
                       uint16_t        pri);
 
/**
 * \brief 将一个任务添加到队列中
 *
 * \param[in] handle  : 任务队列的标准服务句柄
 * \param[in] p_job   : 指向任务的指针
 *
 * \retval AM_OK      ：加入成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EBUSY  : 表明当前任务已经在队列中，无法重复添加
 */
int am_jobq_post (am_jobq_handle_t handle, am_jobq_job_t *p_job);

/**
 * \brief 处理一次任务队列，所有任务处理完毕后返回
 *
 * 一个任务被处理后，自动退出队列，可以被再次加入。否则，
 * 任务还未被处理时，无法再次加入队列中
 *
 * \param[in] handle  : 任务队列的标准服务句柄
 *
 * \retval AM_OK      ：队列中所有任务处理完成
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EBUSY  : 表明当前任务队列正在处理中
 */
int am_jobq_process (am_jobq_handle_t handle);

/**
 * \brief 定义任务队列实例，并指定任务使用的最高优先级
 *
 * \param[in] queue_name   : 队列名
 * \param[in] priority_num : 任务队列使用的优先级数目
 *                             - 实际任务的有效优先级为：0 ~ priority_num - 1;
 *                             - 值越大，优先级越低，0为最高优先级。
 *
 * \note 该定义作为内存分配，由于队列需要保持在初始化时和处理时均有效，因此需要
 *       定义为全局的，如果只在模板内使用，则建议使用 AM_JOBQ_QUEUE_DECL_STATIC()
 *       定义为静态的。
 */
#define AM_JOBQ_QUEUE_DECL(queue_name, priority_num)                        \
            struct __jobqinfo_##queue_name {                                \
                am_jobq_queue_t      jobq_queue;                            \
                struct am_list_head  pri_heads[priority_num];               \
                unsigned int         bitmap_job[(priority_num + 31) >> 5];  \
            } queue_name;

/**
 * \brief 定义任务队列实例（静态）
 *
 * \param[in] queue_name   : 队列名
 * \param[in] priority_num : 任务队列使用的优先级数目
 *                             - 实际任务的有效优先级为：0 ~ priority_num - 1;
 *                             - 值越大，优先级越低，0为最高优先级。
 */
#define AM_JOBQ_QUEUE_DECL_STATIC(queue_name, priority_num)            \
            static AM_JOBQ_QUEUE_DECL(queue_name, priority_num)
/**
 * \brief 初始化任务队列
 *
 * \param[in] queue_name : 任务队列名称，由AM_JOBQ_QUEUE_DECL_STATIC()或
 *                          AM_JOBQ_QUEUE_DECL()定义的任务队列。
 *
 * \return 任务队列的操作句柄，如果初始化失败，则值为NULL
 */
#define AM_JOBQ_QUEUE_INIT(queue_name)                                         \
            am_jobq_queue_init(                                                \
             &(queue_name.jobq_queue),                                         \
               sizeof(queue_name.pri_heads) / sizeof(queue_name.pri_heads[0]), \
               queue_name.pri_heads,                                           \
               queue_name.bitmap_job)

/** @}  */

#ifdef __cplusplus
}
#endif  /* __cplusplus  */

#endif  /* __AM_JOBQ_H */

/* end of file */

