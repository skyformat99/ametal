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
 * \brief 按键输入事件（实现了标准接口文件 am_input.h）
 *
 * \internal
 * \par modification history:
 * - 1.00 17-05-12  tee, first implementation.
 * \endinternal
 */
#include "am_input.h"
#include "am_event.h"
#include "am_vdebug.h"
#include "am_delay.h"
#include <string.h>
#include "am_event_category_input.h"

/*******************************************************************************
  Local type defines
*******************************************************************************/

/** \brief 按键数据 */
typedef struct am_event_input_key_data {
    int     key_code;      /**< \brief  按键类型码 */
    int     key_state;     /**< \brief  按键状态, DOWN : 0, UP : 非0*/
} am_event_input_key_data_t;

/*******************************************************************************
  Global variables
*******************************************************************************/

static am_event_type_t       __g_event_input_key;

/*******************************************************************************
  Local functions
*******************************************************************************/

static void __event_key_input_handle (am_event_type_t *p_evt_type,
                                      void            *p_evt_data,
                                      void            *p_hdl_data)
{
    am_input_key_handler_t    *p_hdlr = (am_input_key_handler_t *)p_hdl_data;
    am_event_input_key_data_t *p_data = (am_event_input_key_data_t *)p_evt_data;

    p_hdlr->pfn_cb(p_hdlr->p_usr_data, p_data->key_code, p_data->key_state);
}

/*******************************************************************************
  Public variables
*******************************************************************************/

void am_event_input_key_init (void)
{
    am_event_init(&__g_event_input_key);
    am_event_category_intput_event_register(&__g_event_input_key);
}

/*******************************************************************************
  standard interface drivers
*******************************************************************************/

int am_input_key_report (int key_code, int key_state)
{
    am_event_input_key_data_t  data;

    (void)memset(&data, 0, sizeof(data));

    data.key_code   = key_code;
    data.key_state  = key_state;

    am_event_raise(&__g_event_input_key, &data, 0);

    return AM_OK;
}

/******************************************************************************/
int am_input_key_handler_register (am_input_key_handler_t  *p_handler,
                                   am_input_key_cb_t        pfn_cb,
                                   void                    *p_arg)
{
    if ((NULL == p_handler) || (NULL == pfn_cb)) {
        return -AM_EINVAL;
    }

    p_handler->pfn_cb      = pfn_cb;
    p_handler->p_usr_data  = p_arg;

    am_event_handler_init(&p_handler->ev_handler,
                          __event_key_input_handle,
                          (void *)p_handler,          /* private date */
                          0);

    am_event_handler_register(&__g_event_input_key,
                              &p_handler->ev_handler);

    return AM_OK;
}

/******************************************************************************/
int am_input_key_handler_unregister (am_input_key_handler_t *p_handler)
{
    if (NULL == p_handler) {
        return -AM_EINVAL;
    }

    return am_event_handler_unregister(&__g_event_input_key,
                                       &p_handler->ev_handler);
}

/* end of file */
