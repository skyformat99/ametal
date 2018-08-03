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
 * \brief     USBD标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 16-9-27  bob, first implementation.
 * \endinternal
 */

#ifndef __AM_USB_DCI_H__
#define __AM_USB_DCI_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "am_usbd_config.h"
#include "am_usbd.h"

/**
 * \addtogroup am_if_usb_dci
 * \copydoc am_usb_dci.h
 * @{
 */

/** \brief Macro to define controller handle */
#define am_usbd_controller_handle_t am_usbd_handle_t

/** \brief Available notify types for device notification */
typedef enum am_usbd_notification
{
    AM_USBD_NOTIFY_BUS_RESET = 0x10U, /**< \brief Reset signal detected */
    AM_USBD_NOTIFY_SUSPEND,           /**< \brief Suspend signal detected */
    AM_USBD_NOTIFY_RESUME,            /**< \brief Resume signal detected */
    AM_USBD_NOTIFY_ERROR,             /**< \brief Errors happened in bus */
    AM_USBD_NOTIFY_DETACH,            /**< \brief Device disconnected from a host */
    AM_USBD_NOTIFY_ATTACH,            /**< \brief Device connected to a host */
} am_usbd_notification_t;

/** \brief Device notification message structure */
typedef struct am_usbd_callback_message
{
    uint8_t *p_buffer; /**< \brief Transferred buffer */
    uint32_t length; /**< \brief Transferred data length */
    uint8_t code;    /**< \brief Notification code */
    uint8_t is_setup; /**< \brief Is in a setup phase */
} am_usbd_callback_message_t;

/** \brief Control type for controller */
typedef enum am_usbd_control_type
{
    AM_USBD_CONTROL_RUN = 0U,          /**< \brief Enable the device functionality */
    AM_USBD_CONTROL_STOP,              /**< \brief Disable the device functionality */
    AM_USBD_CONTROL_ENDPOINT_INIT,      /**< \brief Initialize a specified endpoint */
    AM_USBD_CONTROL_ENDPOINT_DEINIT,    /**< \brief De-initialize a specified endpoint */
    AM_USBD_CONTROL_ENDPOINT_STALL,     /**< \brief Stall a specified endpoint */
    AM_USBD_CONTROL_ENDPOINT_UNSTALL,   /**< \brief Unstall a specified endpoint */
    AM_USBD_CONTROL_GET_DEVICE_STATUS,   /**< \brief Get device status */
    AM_USBD_CONTROL_GET_ENDPOINT_STATUS, /**< \brief Get endpoint status */
    AM_USBD_CONTROL_SET_DEVICE_ADDRESS,  /**< \brief Set device address */
    AM_USBD_CONTROL_GET_SYNCH_FRAME,     /**< \brief Get current frame */
    AM_USBD_CONTROL_RESUME,            /**< \brief Drive controller to generate a resume signal in USB bus */
    AM_USBD_CONTROL_SET_DEFAULT_STATUS,  /**< \brief Set controller to default status */
    AM_USBD_CONTROL_GET_SPEED,          /**< \brief Get current speed */
    AM_USBD_CONTROL_GET_OTG_STATUS,      /**< \brief Get OTG status */
    AM_USBD_CONTROL_SET_OTG_STATUS,      /**< \brief Set OTG status */
    AM_USBD_CONTROL_SET_TEST_MODE,       /**< \brief Drive xCHI into test mode */
} am_usbd_control_type_t;

/** \brief USB device controller initialization function typedef */
typedef am_usb_status_t (*pfn_usbd_controller_init_t)(am_usbd_controller_handle_t controller_handle);

/** \brief USB device controller de-initialization function typedef */
typedef am_usb_status_t (*pfn_usbd_controller_deinit_t)(am_usbd_controller_handle_t controller_handle);

/** \brief USB device controller send data function typedef */
typedef am_usb_status_t (*pfn_usbd_controller_send_t)(am_usbd_controller_handle_t controller_handle,
                                                     uint8_t endpoint_address,
                                                     uint8_t *p_buffer,
                                                     uint32_t length);

/** \brief USB device controller receive data function typedef */
typedef am_usb_status_t (*pfn_usbd_controller_recv_t)(am_usbd_controller_handle_t controller_handle,
                                                      uint8_t endpoint_address,
                                                      uint8_t *p_buffer,
                                                      uint32_t length);

/** \brief USB device controller cancel transfer function in a specified endpoint typedef */
typedef am_usb_status_t (*pfn_usbd_controller_cancel_t)(am_usbd_controller_handle_t controller_handle,
                                                       uint8_t endpoint_address);

/** \brief USB device controller control function typedef */
typedef am_usb_status_t (*pfn_usbd_controller_control_t)(am_usbd_controller_handle_t controller_handle,
                                                      am_usbd_control_type_t command,
                                                        void *p_param);

/** \brief USB device controller interface structure */
typedef struct am_usbd_controller_interface
{
    pfn_usbd_controller_init_t    device_init;    /**< \brief Controller initialization */
    pfn_usbd_controller_deinit_t  device_deinit;  /**< \brief Controller de-initialization */
    pfn_usbd_controller_send_t    device_send;    /**< \brief Controller send data */
    pfn_usbd_controller_recv_t    device_recv;    /**< \brief Controller receive data */
    pfn_usbd_controller_cancel_t  device_cancel;  /**< \brief Controller cancel transfer */
    pfn_usbd_controller_control_t device_control; /**< \brief Controller control */
} am_usbd_controller_interface_t;

/** \brief USB device status structure */
typedef struct am_usbd_dev
{
    am_usbd_controller_handle_t controller_handle;                       /**< \brief Controller handle */
    const am_usbd_controller_interface_t *p_controller_interface; /**< \brief Controller interface handle */
#if USB_DEVICE_CONFIG_USE_TASK
    AM_usb_osa_msgq_handle notification_queue; /**< \brief Message queue */
#endif
    pfn_usbd_callback_t pfn_device_callback; /**< \brief Device callback function pointer */
    am_usbd_endpoint_callback_struct_t
        endpoint_callback[USB_DEVICE_CONFIG_ENDPOINTS << 1U]; /**< \brief Endpoint callback function structure */
    uint8_t device_address;                                   /**< \brief Current device address */
    uint8_t controller_id;                                    /**< \brief Controller ID */
    uint8_t state;                                           /**< \brief Current device state */
    uint8_t is_reset_ting;                                     /**< \brief Is doing device reset or not */
} am_usbd_dev_t;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_USB_DCI_H__ */
