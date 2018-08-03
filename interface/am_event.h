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
 * \brief 事件报告框架
 *
 * 三级概念：
 * 1. event category ：其对应多个event type 和多个 event handler
 * 2. event type     ：其对应多个event handler
 * 3. event handler  ：其对应一个事件处理函数
 *
 * 事件触发：
 * 事件触发的最小单位是 event type，事件触发时：
 * 1. 触发该 event type 所属 event category 中的所有 event handler 执行一遍。
 * 2. 如果事件标志不含 AM_EVENT_PROC_FLAG_CAT_ONLY，则该 event type中所有的
 *    event handler 也会执行一遍。
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12 tee, first implementation.
 * \endinternal
 */

#ifndef __AM_EVENT_H /* { */
#define __AM_EVENT_H

/**
 * \addtogroup am_if_event
 * \copydoc am_event.h
 * @{
 */

#include "am_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name 事件处理器标志，用于am_event_handler_t的  flags 成员，多个标志可使用或连接
 * @{
 */

/** \brief 事件处理后，该事件处理器将自动注销   */
#define AM_EVENT_HANDLER_FLAG_AUTO_UNREG     (1u << 0)

/** @} */

/**
 * \name 事件标志，多个标志可使用或连接
 * @{
 */

/* \brief 该事件只会触发 其所属 event category 中的 event handler  */
#define AM_EVENT_PROC_FLAG_CAT_ONLY         (1u << 0)

/** @} */

/** \breif event_category 结构体类型声明 */
struct am_event_category;
typedef struct am_event_category am_event_category_t;

/** \breif event_type 结构体类型声明 */
struct am_event_type;
typedef struct am_event_type am_event_type_t;

/** \breif event_handler 结构体类型声明 */
struct am_event_handler;
typedef struct am_event_handler am_event_handler_t;

/**
 * \brief 事件处理函数类型定义
 *
 * \param[in]  p_evt_type : 指向事件
 * \param[in]  p_evt_data : 事件的数据
 * \param[in]  p_hdl_data ： 事件处理器的私有数据
 *
 * \return 无
 */
typedef void am_event_function_t (am_event_type_t   *p_evt_type,
                                  void              *p_evt_data,
                                  void              *p_hdl_data);

/**
 * \brief 事件处理器结构体定义
 */
struct am_event_handler {
    uint16_t                 flags;     /**< \brief 标志：AM_EVENT_HANDLER_FLAG_*  */
    am_event_function_t     *pfn_proc;  /**< \brief 处理函数  */
    void                    *p_data;    /**< \brief 事件处理器私有数据      */
    struct am_event_handler *p_next;    /**< \brief 指向下一个事件处理器  */
};

/** \brief 初始化事件处理器 */
am_static_inline
void am_event_handler_init (am_event_handler_t  *p_handler,
                            am_event_function_t *pfn_proc,
                            void                *p_data,
                            uint16_t             flags)
{
    p_handler->flags    = flags;
    p_handler->pfn_proc = pfn_proc;
    p_handler->p_data   = p_data;
    p_handler->p_next   = NULL;
}


/** \brief 事件类型结构定义 */
struct am_event_type {
    am_event_handler_t  *p_handler;   /**< \brief 该事件的所有处理器（链表） */
    am_event_category_t *p_category;  /**< \brief 该事件所属的事件种类  */
    am_event_type_t     *p_next;      /**< \brief 指向下一个事件  */
};


/** \brief 初始化事件  */
am_static_inline
void am_event_init (am_event_type_t *p_event)
{
    p_event->p_category = NULL;
    p_event->p_handler  = NULL;
    p_event->p_next     = NULL;
}

/** \brief 注册事件处理器到事件中 */
int am_event_handler_register (am_event_type_t     *p_event,
                               am_event_handler_t  *p_handler);

/** \brief 从事件中注销事件处理器 */
int am_event_handler_unregister (am_event_type_t     *p_event,
                                 am_event_handler_t  *p_handler);

/**
 * \brief 触发一个事件
 *
 * 会触发该 event type 所属 event category 中的所有 event handler 执行一遍。
 * 如果事件标志不含 AM_EVENT_PROC_FLAG_CAT_ONLY，则该 event type中所有的
 * event handler 也会执行一遍。
 *
 * \param p_event     ： 事件
 * \param p_evt_data  ： 事件数据
 * \param flags       ： 处理标志, 0 或 AM_EVENT_PROC_FLAG_*
 *
 * \return 标准错误号
 */
int am_event_raise (am_event_type_t *p_event, void *p_evt_data, int flags);

/**
 * \brief 事件种类结构体定义，其包含多个事件或事件处理器
 *
 * 该种类下的任意事件发生时，都将触发该种类下的所有事件处理器
 */
struct am_event_category {
    am_event_type_t    *p_event;    /**< \brief 事件链表  */
    am_event_handler_t *p_handler;  /**< \brief 事件处理器链表 */
};


/** \brief 初始化事件事件种类 */
am_static_inline
void am_event_category_init (am_event_category_t *p_category)
{
    p_category->p_event   = NULL;
    p_category->p_handler = NULL;
}

/** \brief 注册事件处理器到事件种类中 */
int am_event_category_handler_register (am_event_category_t *p_category,
                                        am_event_handler_t  *p_handler);

/** \brief 从事件种类中注销事件处理器 */
int am_event_category_handler_unregister (am_event_category_t *p_category,
                                          am_event_handler_t  *p_handler);

/** \brief 注册事件到事件种类中  */
int am_event_category_event_register (am_event_category_t *p_category,
                                      am_event_type_t     *p_event);

/** \brief 从事件种类中注销事件类型  */
int am_event_category_event_unregister (am_event_category_t *p_category,
                                        am_event_type_t     *p_event);

#ifdef __cplusplus
}
#endif

/** @} grp_am_if_event */

#endif /* } __AM_EVENT_H */

/* end of file */
