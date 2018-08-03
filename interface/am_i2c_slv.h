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
 * \brief I2C从机标准接口
 *
 * \note 用户可调用此接口通过MCU来模拟一款I2C从设备
 *
 * \internal
 * \par Modification History
 * - 1.00 17-09-06  vir, first implementation.
 * \endinternal
 */

#ifndef __AM_I2C_SLV_H
#define __AM_I2C_SLV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * @addtogroup am_if_i2c_slv
 * @copydoc am_i2c_slv.h
 * @{
 */

/**
 * \name I2C从机设备  属性标志，用于 am_i2c_slv_device 的 dev_flags 成员
 * @{
 */
#define AM_I2C_SLV_ADDR_7BIT          0x0000u            /**< \brief 7-bits 设备地址(默认) */
#define AM_I2C_SLV_ADDR_10BIT         0x0001u            /**< \brief 10-bits 设备地址 */

#define AM_I2C_SLV_GEN_CALL_NACK      0X0000u             /**< \brief 不响应广播地址 (默认) */
#define AM_I2C_SLV_GEN_CALL_ACK       0X0002u             /**< \brief 响应广播地址  */

/* 以下用于含有子地址的设备 */
#define AM_I2C_SLV_SUBADDR_1BYTE      0X0000u            /**< \brief 子地址宽度1字节(默认) */
#define AM_I2C_SLV_SUBADDR_2BYTE      0X0004u            /**< \brief 子地址宽度2字节 */


#define AM_I2C_SLV_SUBADDR_SELF_INCREASING       0X0000u   /**< \brief 支持子地址自增  (默认)*/
#define AM_I2C_SLV_SUBADDR_NSELF_INCREASING      0X0100u   /**< \brief 不支持子地址自增   */
/** @} */


/**
 * \name  回调函数的返回值类型
 *
 * \ note 要看具体硬件是否支持控制从机响应
 * @{
 */
#define AM_I2C_SLV_ACK          0       /**< \brief 从机响应  */
#define AM_I2C_SLV_NACK         1       /**< \brief 从机不响应 */

/** @} */



struct am_i2c_slv_device;

/**
 * \brief I2C从机驱动函数体
 */
struct am_i2c_slv_drv_funcs {
    /** \brief 使能I2C从机设备开始工作 */
    int (*pfn_i2c_slv_setup) (void *p_drv, struct am_i2c_slv_device *p_i2c_slv_dev);

    /** \brief 关闭I2C从设备 */
    int (*pfn_i2c_slv_shutdown) (void *p_drv, struct am_i2c_slv_device *p_i2c_slv_dev);

    /** \brief 获取还可生成的从机设备个数 */
    int (*pfn_i2c_slv_num_get) (void *p_drv);
};

/**
 * \brief I2C从机标准服务结构体
 */

typedef struct am_i2c_slv_serv {
    struct am_i2c_slv_drv_funcs   *p_funcs;   /**< \brief I2C从驱动函数         */
    void                          *p_drv;     /**< \brief I2C从驱动函数入口参数 */
} am_i2c_slv_serv_t;

/** \brief I2C从机标准服务操作句柄定义 */
typedef am_i2c_slv_serv_t *am_i2c_slv_handle_t;

/**
 * \brief 回调函数 用于分析主机向从机操作的状态
 *
 * \note 返回值参考 ：“回调函数的返回值类型”
 */
typedef struct am_i2c_slv_cb_funcs {

    /** \brief 从机地址匹配时回调函数，当 is_rx 为AM_TRUE时表示 从机接受数据完成 ,为 AM_FALSE时表示从机发送数据完成   */
    int (*pfn_addr_match)(void *p_arg, am_bool_t is_rx);

    /** \brief 获取一个发送字节回调函数 */
    int (*pfn_txbyte_get)(void *p_arg, uint8_t *p_byte);

    /** \brief 提交一个接收到的字节回调函数  */
    int (*pfn_rxbyte_put)(void *p_arg, uint8_t byte);

    /** \brief 停止传输回调函数 */
    void (*pfn_tran_stop)(void *p_arg);

    /** \brief 广播回调函数   */
    int (*pfn_gen_call)(void *p_arg, uint8_t byte);

}am_i2c_slv_cb_funcs_t;


/**
 * \brief I2C从机设备结构体  (推荐使用 am_i2c_slv_mkdev() 设置本数据结构)
 */
typedef struct am_i2c_slv_device {
    am_i2c_slv_handle_t    handle;              /**< \brief 与从设备关联的I2C服务标准handle    */
    am_i2c_slv_cb_funcs_t *p_cb_funs;           /**< \brief 回调函数,用户不需要的回调函数赋值为NULL即可 */
    uint16_t               dev_addr;            /**< \brief 本设备的地址 */
    uint16_t               dev_flags;           /**< \brief 设备的特性，见“I2C从设备属性标志” */
    void                  *p_arg;               /**< \brief 回调函数参数 */
}am_i2c_slv_device_t;

/**
 * \brief I2C本从机设备 描述结构体参数设置
 *
 * \param[in] p_dev                 : 指向从机设备描述结构体的指针
 * \param[in] handle                : 与从设备关联的I2C标准服务操作句柄
 * \param[in] p_cb_funs             : 回调函数
 * \param[in] dev_addr              : 从机设备地址
 * \param[in] dev_flags             : 从机设备特性，见“I2C从设备属性标志”
 * \param[in] p_arg                 : 回调函数参数
 *
 * \return 无
 *
 * \par 范例
 * \code
 * #include "am_i2c_slv.h"
 *
 *   am_i2c_slv_device_t  i2c_slv_dev;
 *
 *   am_i2c_slv_mkdev(  &i2c_slv_dev,
 *                      i2c_slv_handle,
 *                      &__g__cb_funs,
 *                      0x55,
 *                      AM_I2C_SLV_ADDR_7BIT
 *                      &i2c_slv_dev);
 * \endcode
 */
am_static_inline
void am_i2c_slv_mkdev (am_i2c_slv_device_t    *p_dev,
                       am_i2c_slv_handle_t     handle,
                       am_i2c_slv_cb_funcs_t  *p_cb_funs,
                       uint16_t                dev_addr,
                       uint16_t                dev_flags,
                       void                   *p_arg)

{
    p_dev->handle           = handle;
    p_dev->dev_addr         = dev_addr;
    p_dev->dev_flags        = dev_flags;
    p_dev->p_cb_funs        = p_cb_funs;
    p_dev->p_arg            = p_arg;
}

/**
 * \brief 开启I2C从机设备
 *
 * \param[in] p_dev  : I2C从机设备
 *
 * \retval  AM_OK      :开启成功
 * \retval  AM_EINVAL  :参数错误
 * \retval  AM_EAGAIN  :无可用的从机个数
 *
 * \note  调用am_i2c_slv_mkdev函数生成一个从机设备之后，必须调用此函数 开启从机
 *
 */
am_static_inline
int am_i2c_slv_setup (am_i2c_slv_device_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_i2c_slv_setup(p_dev->handle->p_drv, p_dev);
}

/**
 * \brief 关闭I2C从机设备
 *
 * \param[in] p_dev  : I2C从机设备
 *
 * \retval  AM_OK      : 关闭成功
 *
 * \note 关闭之后若需重新开启则 调用 am_i2c_slv_start 即可
 */
am_static_inline
int am_i2c_slv_shutdown ( am_i2c_slv_device_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_i2c_slv_shutdown(p_dev->handle->p_drv, p_dev);
}

/**
 * \brief 获取可用的从机设备个数
 *
 * \param[in] handle   : I2C从机标准服务操作句柄
 *
 * \retval  返回值为可用的从机个数
 *          返回 0 ：无可用从机个数 ， 不能再调用 am_i2c_slv_setup 函数
 *          返回 1 ：可用从机个数为 1，可再调用am_i2c_slv_setup 函数一次，来将新生成的设备开启
 *          ...
 *          返回 n : 可用从机个数为 n,可再调用am_i2c_slv_setup 函数n次,来将新生成的设备开启 .
 */
am_static_inline
int am_i2c_slv_num_get (am_i2c_slv_handle_t handle)
{
    return handle->p_funcs->pfn_i2c_slv_num_get(handle->p_drv);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_I2C_SLV_H */

/* end of file */
