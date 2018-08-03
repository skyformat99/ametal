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
 * \brief spi从机标准接口
 *
 * \note 可通过此接口定义一款spi从机设备
 *
 * \internal
 * \par Modification History
 * -1.00 17-09-14 fra, first implementation
 * \endinternal
 */

#ifndef __AM_SPI_SLV_H
#define __AM_SPI_SLV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "am_list.h"

/**
 * \addtogroup am_if_spi_slv
 * \copydoc am_spi_slv.h
 * @{
 */

/**
 * \name SPI从设备模式标志，用于am_spi_slv_mkdev()函数的mode参数
 * @{
 */

#define AM_SPI_SLV_CPHA        0x01  /**< \brief 时钟相位可设置             */
#define AM_SPI_SLV_CPOL        0x02  /**< \brief 时钟极性可设置             */
#define AM_SPI_SLV_CS_HIGH     0x04  /**< \brief 片选高有效                 */
#define AM_SPI_SLV_LSB_FIRST   0x08  /**< \brief 按低位先出的方式发送数据帧 */
#define AM_SPI_SLV_3WIRE       0x10  /**< \brief 3线模式，SI/SO 信号线共享  */
#define AM_SPI_SLV_LOOP        0x20  /**< \brief 回环模式                   */
#define AM_SPI_SLV_NO_CS       0x40  /**< \brief 单设备总线, 无片选         */

#define AM_SPI_SLV_MODE_0      (0 | 0)                 /**< \brief SPI从机模式0 */
#define AM_SPI_SLV_MODE_1      (0 | AM_SPI_SLV_CPHA) /**< \brief SPI从机模式1 */
#define AM_SPI_SLV_MODE_2      (AM_SPI_SLV_CPOL | 0) /**< \brief SPI从机模式2 */
#define AM_SPI_SLV_MODE_3      (AM_SPI_SLV_CPOL | AM_SPI_SLV_CPHA) /**< \brief SPI从机模式3 */

/** @} */

struct am_spi_slv_device;        /**< \brief 声明SPI从设备的结构体类型 */

/**
 * \brief SPI从设备驱动函数结构体
 */
struct am_spi_slv_drv_funcs {

    /** \brief 启动从设备  */
    int (*pfn_spi_slv_setup) (void *p_drv, struct am_spi_slv_device *p_dev);

    /** \brief 关闭SPI从设备   */
    int (*pfn_spi_slv_shutdown) (void *p_drv, struct am_spi_slv_device *p_dev);

};

/**
 * \brief SPI从设备 标准服务结构体
 */
typedef struct am_spi_slv_serv {
    struct am_spi_slv_drv_funcs   *p_funcs;    /**< \brief SPI从设备驱动函数     */
    void                          *p_drv;      /**< \brief SPI从设备驱动函数参数 */
} am_spi_slv_serv_t;


/** \brief SPI从设备 标准服务操作句柄定义 */
typedef am_spi_slv_serv_t *am_spi_slv_handle_t;


/**
 * \brief SPI 从机传输
 *
 * - SPI 从机传输写的个数总是等于读的个数。协议驱动应该总是提供 \a rx_buf 和/或
 * \a tx_buf 。在某些情况下，它们同样可能想提供数据传输的DMA地址，这样的话，底
 * 层驱动可以使用DMA，以降低数据传输的代价。
 *
 * 如果传输缓存区 tx_buf为 NULL rx_buf不为NULL， 则表示从机 将0发送到总线上，接受的存到rx_buf中
 * 如果传输缓存区 tx_buf不为 NULL rx_buf为NULL， 则表示从机 舍弃接受的数据 ，只单纯的发送数据
 *
 * - 在内存中，数据总是按CPU的的本地字节顺序存放，以线字节顺序传输 (即大端，除非
 * 设备模式或传输标志中被设置了 AM_SPI_LSB_FIRST)。例如，当\a bits_per_word 为
 * 16，缓冲区为 2N 字节长 (\a len = 2N) 并且以CPU字节顺便保存N个16-bit字。
 *
 * - 当 SPI 传输的字大小不是2的幂次倍8-bit时，这些内存中的字将包含额外的bit。在
 * 内存中，字总是向右对齐的，因此，未定义 (rx) 或未使用 (tx) 的位总是最高有效位。
 *
 */
typedef struct am_spi_slv_tansfer {
    /** \brief 指向发送缓存的指针 */
    const void *p_tx_buf;

    /** \brief 指向接收缓存的指针 */
    void *p_rx_buf;

    /** \brief 数据长度(字节数) */
    size_t  nbytes;

    /** \brief 字大小，若为0，则使用 am_spi_device_t 中设置的值 */
    uint8_t     bits_per_word;

}am_spi_slv_transfer_t;

/**
 * \brief SPI从设备回调函数类型
 */
typedef struct am_spi_slv_cb_funs{

    /** \brief 片选CS有效回调 */
    int (*pfn_cs_active)(void *p_arg);

    /** \brief 片选CS无效回调  (nbytes为本次片选有效中传输的总字节数 )*/
    int (*pfn_cs_inactive)(void *p_arg, size_t nbytes);

    /** \brief 获取 传输的信息  */
    int (*pfn_tansfer_get)(void *p_arg, am_spi_slv_transfer_t *p_trans);

} am_spi_slv_cb_funs_t;


/** \brief SPI从设备描述结构 */
typedef struct am_spi_slv_device {

     /** \brief 该设备关联的SPI标准服务handle */
     am_spi_slv_handle_t  handle;

     /** \brief 从设 数据传输包含1个或多个字，比较常见的字大小为8-bit或12-bit，在内存
      *   中，字大小应该是2的幂次方，例如，20-bit自大小应使用32-bit内存
      *   为0时，默认选择为8bit
      */
     uint8_t                bits_per_word;

     /** \brief 从设备的SPI模式标志，请参考“SPI从机模式标志”
      *
      * 该参数用于设置SPI从设备的时钟相位和极性，需要与主机设置相同
      */
     uint16_t               mode;

     /** \brief 从设备回调函数 */
     am_spi_slv_cb_funs_t *p_slv_cb_funcs;

     /** \brief 回调函数参数 */
     void                   *p_arg;

} am_spi_slv_device_t;

/**
 * \brief 设置SPI设备结构体参数
 *
 * \param[in] p_dev         : SPI从机设备描述符指针
 * \param[in] handle        : SPI标准服务操作句柄
 * \param[in] bits_per_word : 字大小，为0时使用默认的“8-bit字大小”
 * \param[in] mode          : 设备模式标志，见“SPI模式标志”
 * \param[in] p_slv_cb_funcs : SPI从设备回调函数
 * \param[in] p_arg         : SPI从设备回调函数的参数
 *
 * \return 无
 *
 * \par 范例
 * 见 am_spi_slv_setup()
 */
am_static_inline
void am_spi_slv_mkdev (am_spi_slv_device_t  *p_dev,
                       am_spi_slv_handle_t   handle,
                       uint8_t               bits_per_word,
                       uint16_t              mode,
                       am_spi_slv_cb_funs_t *p_slv_cb_funcs,
                       void                 *p_arg)
{
    p_dev->handle             = handle;
    p_dev->bits_per_word      = bits_per_word;
    p_dev->mode               = mode;
    p_dev->p_slv_cb_funcs     = p_slv_cb_funcs;
    p_dev->p_arg              = p_arg;

}


/**
 * \brief 设置SPI从机设备
 *
 * \attention 在SPI从机设备被使用前，必须先调用本函数进行设置，且如本函数返回
 *            错误，则一定不能再使用此SPI从机
 *
 * \param[in] p_dev : SPI从机设备
 *
 * \retval   AM_OK           : 设置成功
 * \retval  -AM_ENOTSUP      : 配置失败，不支持的位宽、模式等
 *
 * \par 示例
 * \code
 *  am_spi_slv_device_t spi_slv_dev;     // 从设备描述结构
 *
 * // 初始化设备描述结构
 * am_spi_slv_mkdev(&spi_slv_dev,
 *                    handle,         // SPI从机标准服务handle
 *                    8，                                 // 字大小为8-bit
 *                    AM_SPI_MODE_1,  // SPI 模式1
 *                    __g_spi_slv_cb_funs,   // 从机回调函数为__g_spi_slv_cb_funs
 *                    &spi_slv_dev);          // 从机回调函数的入口参数
 *
 * // 设置设备
 * am_spi_slv_setup(&spi_slv_dev);
 *
 * \endcode
 *
 */
am_static_inline
int am_spi_slv_setup (am_spi_slv_device_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_spi_slv_setup(p_dev->handle->p_drv, p_dev);
}



/**
 * \brief 关闭SPI从机
 *
 * \param[in]  handle : SPI标准服务操作句柄
 * \param[out] p_dev  : SPI从机设备
 *
 * \retval  AM_OK     : 设置成功
 */
am_static_inline
int am_spi_slv_shutdown (am_spi_slv_device_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_spi_slv_shutdown(p_dev->handle->p_drv, p_dev);
}

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SPI_SLV_H */

/* end of file */
