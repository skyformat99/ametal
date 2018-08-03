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
 * \brief 输入事件种类
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "am_event.h"
#include "am_input.h"
#include "am_event_category_input.h"

/*******************************************************************************
  Global variables
*******************************************************************************/

static am_event_category_t   __g_category_input;
static am_event_handler_t    __g_handler_cat_input;

/*******************************************************************************
  Local functions
*******************************************************************************/

static void __input_event_manager (am_event_type_t *p_evt_type,
                                   void            *p_evt_data,
                                   void            *p_usr_data)
{
    /*
     * 执行event_type的handler之前, 先执行该category的handler
     * input category的handler暂时不处理任何事务, 预留给未来使用
     */
}

/*******************************************************************************
  Public variables
*******************************************************************************/

void am_event_category_input_init (void)
{
    /* 初始化一个事件类别 : Input类别 */
    am_event_category_init(&__g_category_input);

    /* 为Input类别初始化一个handler */
    am_event_handler_init(&__g_handler_cat_input,
                           __input_event_manager,
                           "cat_input",
                           0);                                 /* 无特殊标志   */

    /* 把handler与Input类别绑定 */
    am_event_category_handler_register(&__g_category_input,
                                       &__g_handler_cat_input);
}

/******************************************************************************/
int am_event_category_intput_event_register (am_event_type_t *p_evt_type)
{
    return am_event_category_event_register(&__g_category_input, p_evt_type);
}

/******************************************************************************/
int am_event_category_intput_event_unregister (am_event_type_t *p_evt_type)
{
    return am_event_category_event_unregister(&__g_category_input, p_evt_type);
}

/******************************************************************************/
int am_event_category_intput_handler_register (am_event_handler_t *p_evt_handler)
{
    return am_event_category_handler_register(&__g_category_input, p_evt_handler);
}

/******************************************************************************/
int am_event_category_intput_handler_unregister (am_event_handler_t *p_evt_handler)
{
    return am_event_category_handler_unregister(&__g_category_input, p_evt_handler);
}

/* end of file */
