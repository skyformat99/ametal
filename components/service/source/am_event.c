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
 * \brief Event reporting framework.
 *
 * \internal
 * \par modification history:
 * - 1.01 15-01-05 orz, move event configuration to aw_event_cfg.c.
 * - 1.00 14-06-05 orz, first implementation.
 * \endinternal
 */
#include "ametal.h"
#include "am_event.h"
#include "am_int.h"

/*******************************************************************************
  Local functions
*******************************************************************************/

/* go through an event handler list and call each handler functions */
static void __event_handler_process (am_event_handler_t **pp_head,
                                     am_event_type_t     *p_event,
                                     void                *p_evt_data)
{
    am_event_handler_t *p_next, *p_handler;

    for (p_handler = *pp_head; NULL != p_handler; p_handler = p_next) {

        p_next = p_handler->p_next;   /* handlers may delete themselves */

        if (NULL != p_handler->pfn_proc) {
            p_handler->pfn_proc(p_event, p_evt_data, p_handler->p_data);
        }

        if (p_handler->flags & AM_EVENT_HANDLER_FLAG_AUTO_UNREG) {
            /* the handler request to delete it's self */
            p_handler->p_next = NULL; /* this handler point to NULL */
            *pp_head          = p_next; /* previous handler pointer point to next */
        } else {
             pp_head = &p_handler->p_next; /* now this handler is 'previous' */
        }
    }
}

/******************************************************************************/
/* raise an event immediately. */
static int __event_raise (am_event_type_t *p_event, void *p_evt_data, int cat_only)
{
    int key = am_int_cpu_lock();

    /* process category handler for this event first */
    __event_handler_process(&p_event->p_category->p_handler, p_event, p_evt_data);

    if (!cat_only) {

        /* process event handler */
        __event_handler_process(&p_event->p_handler, p_event, p_evt_data);
    }

    am_int_cpu_unlock(key);

    return AM_OK;;
}

/******************************************************************************/
static int __event_handler_delete (am_event_handler_t **pp_head,
                                   am_event_handler_t  *p_handler)
{
    int                 ret  = -AM_ENODEV;
    am_event_handler_t *prev = container_of(pp_head, am_event_handler_t, p_next);
    am_event_handler_t *hdlr;

    int key = am_int_cpu_lock();

    for (hdlr = *pp_head; NULL != hdlr; prev = hdlr, hdlr = hdlr->p_next) {
        if (hdlr == p_handler) {
            prev->p_next      = p_handler->p_next;
            p_handler->p_next = NULL;
            ret               = AM_OK;
            break;
        }
    }

    am_int_cpu_unlock(key);

    return ret;
}

/******************************************************************************/
static int __event_type_delete (am_event_type_t **pp_head,
                                am_event_type_t  *p_evt_type)
{
    int              ret  = -AM_EINVAL;
    am_event_type_t *prev = container_of(pp_head, am_event_type_t, p_next);
    am_event_type_t *hdlr;

    int key = am_int_cpu_lock();

    for (hdlr = *pp_head; NULL != hdlr; prev = hdlr, hdlr = hdlr->p_next) {
        if (hdlr == p_evt_type) {
            prev->p_next       = p_evt_type->p_next;
            p_evt_type->p_next = NULL;
            ret                = AM_OK;
            break;
        }
    }

    am_int_cpu_unlock(key);

    return ret;
}

/*******************************************************************************
  Public functions
*******************************************************************************/

int am_event_raise (am_event_type_t *p_event, void *p_evt_data, int flags)
{

    int cat_only = flags & AM_EVENT_PROC_FLAG_CAT_ONLY;

    if (p_event == NULL) {
        return -AM_EINVAL;
    }

    return __event_raise(p_event, p_evt_data, cat_only);
}

/******************************************************************************/
int am_event_handler_register (am_event_type_t     *p_event,
                               am_event_handler_t  *p_handler)
{
    int key;

    if ((p_event == NULL) || (p_handler == NULL)) {
        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();

    /* add the handler to event handler list */
    p_handler->p_next  = p_event->p_handler;
    p_event->p_handler = p_handler;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
int am_event_handler_unregister (am_event_type_t     *p_event,
                                 am_event_handler_t  *p_handler)
{
    if ((p_event == NULL) || (p_handler == NULL)) {
        return -AM_EINVAL;
    }

    return __event_handler_delete(&p_event->p_handler, p_handler);
}

/******************************************************************************/
int am_event_category_handler_register (am_event_category_t *p_category,
                                         am_event_handler_t  *p_handler)
{
    int key;

    if ((p_category == NULL) || (p_handler == NULL)) {
        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();

    /* add the handler to event category handler list */
    p_handler->p_next     = p_category->p_handler;
    p_category->p_handler = p_handler;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
int am_event_category_handler_unregister (am_event_category_t *p_category,
                                          am_event_handler_t  *p_handler)
{
    return __event_handler_delete(&p_category->p_handler, p_handler);
}

/******************************************************************************/
int am_event_category_event_register (am_event_category_t  *p_category,
                                      am_event_type_t      *p_event)
{
    int key;

    if ((p_category == NULL) || (p_event == NULL)) {
        return -AM_EINVAL;
    }

    key = am_int_cpu_lock();

    p_event->p_category = p_category;

    /* add the handler to event category handler list */
    p_event->p_next     = p_category->p_event;
    p_category->p_event = p_event;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
int am_event_category_event_unregister (am_event_category_t *p_category,
                                        am_event_type_t     *p_event)
{
    if ((p_category == NULL) || (p_event == NULL)) {
        return -AM_EINVAL;
    }

    if (p_event->p_category != p_category) {
        return -AM_EINVAL;
    }

    return __event_type_delete(&p_category->p_event, p_event);
}

/* end of file */
