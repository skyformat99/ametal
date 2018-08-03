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
 * \brief SPI标准接口
 *
 * \internal
 * \par Modification history
 * - 1.01 15-08-17  tee, modified some interface. 
 * - 1.00 14-11-01  jon, first implementation.
 * \endinternal
 */

#ifndef __AM_SPI_H
#define __AM_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "am_list.h"

/**
 * \addtogroup am_if_spi
 * \copydoc am_spi.h
 * @{
 */

/**
 * \name SPI模式标志，用于am_spi_mkdev()函数的mode参数
 * @{
 */

#define AM_SPI_CPHA        0x01  /**< \brief 时钟相位可设置             */
#define AM_SPI_CPOL        0x02  /**< \brief 时钟极性可设置             */
#define AM_SPI_CS_HIGH     0x04  /**< \brief 片选高有效                 */
#define AM_SPI_LSB_FIRST   0x08  /**< \brief 按低位先出的方式发送数据帧 */
#define AM_SPI_3WIRE       0x10  /**< \brief 3线模式，SI/SO 信号线共享  */
#define AM_SPI_LOOP        0x20  /**< \brief 回环模式                   */
#define AM_SPI_NO_CS       0x40  /**< \brief 单设备总线, 无片选         */
#define AM_SPI_READY       0x80  /**< \brief READY信号,从机拉低此信号暂停传输 */

#define AM_SPI_MODE_0      (0 | 0)                     /**< \brief SPI模式0 */
#define AM_SPI_MODE_1      (0 | AM_SPI_CPHA)           /**< \brief SPI模式1 */
#define AM_SPI_MODE_2      (AM_SPI_CPOL | 0)           /**< \brief SPI模式2 */
#define AM_SPI_MODE_3      (AM_SPI_CPOL | AM_SPI_CPHA) /**< \brief SPI模式3 */

/** @} */

/**
 * \name SPI传输特殊特殊设置标志，用于am_spi_mktrans()函数的flags参数
 * @{
 */

/** \brief SPI在读取过程中MOSI引脚输出高电平（默认为低电平）    */
#define AM_SPI_READ_MOSI_HIGH    0x01

/** @} */

/** 
 * \brief SPI控制器信息结构体
 */
typedef struct am_spi_info {
    uint32_t  min_speed;     /**< \brief 最小工作频率  */
    uint32_t  max_speed;     /**< \brief 最大工作频率  */ 
    uint32_t  features;      /**< \brief SPI控制器特性,支持的见“SPI配置标志” */
} am_spi_info_t;

struct am_spi_device;        /**< \brief 声明SPI从机设备的结构体类型 */
struct am_spi_transfer;      /**< \brief 声明SPI传输的结构体类型     */
struct am_spi_message;       /**< \brief 声明SPI消息的结构体类型     */

/**
 * \brief SPI驱动函数结构体
 */
struct am_spi_drv_funcs {

    /** \brief 获取SPI控制器信息 */
    int (*pfn_spi_info_get) (void *p_drv, am_spi_info_t *p_info);
    
    /** \brief 设置SPI从机设备   */
    int (*pfn_spi_setup) (void *p_drv, struct am_spi_device *p_dev);
    
    /** \brief 启动SPI消息传输，完成后调用回调函数  */
    int (*pfn_spi_msg_start)(void                   *p_drv,
                             struct am_spi_device   *p_dev,
                             struct am_spi_message  *p_msg);
};

/**
 * \brief SPI 标准服务结构体
 */
typedef struct am_spi_serv {
    struct am_spi_drv_funcs   *p_funcs;    /**< \brief SPI  SPI驱动函数     */
    void                      *p_drv;      /**< \brief SPI  SPI驱动函数参数 */
} am_spi_serv_t;

/** \brief SPI 标准服务操作句柄定义 */
typedef am_spi_serv_t *am_spi_handle_t;

/**
 * \brief SPI 传输
 *
 * - SPI 传输写的个数总是等于读的个数。协议驱动应该总是提供 \a rx_buf 和/或
 * \a tx_buf 。在某些情况下，它们同样可能想提供数据传输的DMA地址，这样的话，底
 * 层驱动可以使用DMA，以降低数据传输的代价。
 *
 * - 如果传输缓冲区 \a tx_buf 为NULL，则在填充 \a rx_buf 时，0将会被移出到总线上。
 * 如果接收缓冲区 \a rx_buf 为NULL，则从总线上移入的数据将会被丢弃。只有 \a len
 * 个字节会被移出或移入。尝试移出部分字是错误的操作，例如，移出3个字节而字大小
 * 是 16 或 20 bit，前者每个字使用2个字节，而后者使用4个字节。
 *
 * - 在内存中，数据总是按CPU的的本地字节顺序存放，以线字节顺序传输 (即大端，除非
 * 设备模式或传输标志中被设置了 AM_SPI_LSB_FIRST)。例如，当\a bits_per_word 为
 * 16，缓冲区为 2N 字节长 (\a len = 2N) 并且以CPU字节顺便保存N个16-bit字。
 *
 * - 当 SPI 传输的字大小不是2的幂次倍8-bit时，这些内存中的字将包含额外的bit。在
 * 内存中，字总是向右对齐的，因此，未定义 (rx) 或未使用 (tx) 的位总是最高有效位。
 *
 * - 所有SPI传输开始时，片选变得有效，通常它一直保持有效，直到消息的最后一个传输
 * 完成。驱动可以使用 \a cs_change 来影响片选:
 *  -# 如果该传输不是消息的最后一个，则该标志被用来在消息的中间让片选变为无效。
 *     以这种方式来翻转片选，可能需要终止一个芯片命令，让单个 am_spi_message_t
 *     执行所有芯片事务组。
 *  -# 如果该传输是消息的最后一个，则片选保持有效直到下一个传输。在多设备SPI总线
 *     上，若没有要发送给其它设备的消息被阻塞，这便是提高性能的一个技巧；
 *     发送给另一个设备的消息开始执行后，便会使当前设备片选无效。但是，在其它某
 *     些情况下，该标志可以被用来保证正确性。某些设备需要协议事务由一系列的
 *     am_spi_message_t 组成，其中一条消息的内容由前一条消息的结果决定，且整个
 *     事务由片选变为无效而结束。
 */
typedef struct am_spi_transfer {
    const void *p_txbuf;         /**< \brief 指向发送数据缓存的指针   */
    void       *p_rxbuf;         /**< \brief 指向接收数据缓存的指针   */
    size_t      nbytes;          /**< \brief 数据长度(字节数)         */

    /** \brief 字大小，若为0，则使用 am_spi_device_t 中设置的值 */
    uint8_t     bits_per_word;   

    /** \brief 本传输完成后，是否影响片选: 1-影响 0-不影响     */
    uint8_t     cs_change;      

    /** \brief 在本传输之后，在改变片选状态之前延时的微秒数    */
    uint16_t    delay_usecs;

    /**
     * \brief 为本次传输选择一个与 am_spi_device_t 中的默认设置不同的速度，
     * 0 表示使用默认值
     */
    uint32_t    speed_hz;

    /**
     * \brief 本次传输过程的一些特殊设置
     *
     * 比如在读取过程MOSI引脚的状态
     */
    uint32_t     flags;

    /** \brief 链表结点，便于将消息中所有transfer组成一条链 */
    struct am_list_head trans_node;

} am_spi_transfer_t;

/**
 * \brief SPI 消息
 *
 * 1个SPI消息被用来执行一个原子的数据传输序列，每个传输用 am_spi_transfer 表示。
 * 该序列是原子的，意味着任何其它SPI消息不可以使用SPI总线，直到该序列执行完成。
 * 在所有系统中，消息是被排队的，发送给某个 am_spi_device_t 的消息总是以FIFO的
 * 顺序被处理。
 *
 * \attention
 * 向底层提交 am_spi_message_t 的代码负责管理它自己的内存。清零所有的你不需要明
 * 确设置的域，推荐使用 am_spi_msg_init() 来初始化 am_spi_message_t 。
 * 在提交消息以及它的传输之后，不能操作这些内存,直到消息的传输完成回调函数被调用
 * 回调函数。
 * 回调函数调用后，表明消息处理完毕，可以通过 p_msg->status 来判断消息完成的状态
 *  \a AM_OK    消息成功处理完成
 *  \a -AM_EIO  传输出现错误
 *
 * 无论消息的状态如何，都可以从 p_msg->actual_length 来查看成功完成的字节数。
 */
typedef struct am_spi_message {
    struct am_list_head transfers;          /**< \brief 组成消息的传输段 */

    struct am_spi_device *p_spi_dev;        /**< \brief 发送消息的SPI从设备 */

    am_pfnvoid_t    pfn_complete;   /**< \brief 传输完成回调函数 */
    void           *p_arg;          /**< \brief 传递给 pfn_complete 的参数 */
    size_t          actual_length;  /**< \brief 所有成功发送的数据字节总数 */
    int             status;         /**< \brief 消息的状态 */

    void           *ctlrdata[2];    /**< \brief 控制器使用，应用不应操作 */
} am_spi_message_t;

/** \brief SPI从设备描述结构 */
typedef struct am_spi_device {

     am_spi_handle_t  handle;       /**< \brief 该设备关联的SPI标准服务handle */
    
    /**
     * \brief 数据传输包含1个或多个字，比较常见的字大小为8-bit或12-bit，在内存
     * 中，字大小应该是2的幂次方，例如，20-bit自大小应使用32-bit内存。本设置可以
     * 被设备驱动改变，或者设置为0表示使用默认的“字大小为8-bit”。
     *“传输”的 am_spi_transfer_t.bits_per_word 可以重载此配置。
     */
    uint8_t          bits_per_word;

    /** \brief 从设备的SPI模式标志，请参考“SPI模式标志”*/
    uint16_t         mode;

    /**
     * \brief 设备支持的最高速度
     *
     * “传输”的 am_spi_transfer_t.speed_hz 可重载此设置。
     */
    uint32_t         max_speed_hz;

    /**
     * \brief 片选引脚编号
     *
     * 当 am_spi_device.pfunc_cs 为NULL时，才使用此设置。默认低电平有效；若
     * am_spi_device.mode 中 AM_SPI_CS_HIGH 标志被设置，则高电平有效。
     */
    int              cs_pin;

    /**
     * \brief 自定义片选控制函数
     *
     * 使用此函数控制设备的片选: 参数 \a state 为1时片选有效，为0时片选无效。
     * 若此配置为NULL时，将使用 am_spi_device.cs_pin 作为片选引脚，且驱动可能
     * 重新设置一个合适的片选控制函数。
     *
     * \param p_dev
     */
    void      (*pfunc_cs)(struct am_spi_device *p_dev, int state);

} am_spi_device_t;

/**
 * \brief 设置SPI设备结构体参数
 *
 * \param[in] p_dev         : SPI从机设备描述符指针
 * \param[in] handle        : SPI标准服务操作句柄
 * \param[in] bits_per_word : 字大小，为0时使用默认的“8-bit字大小”
 * \param[in] mode          : 设备模式标志，见“SPI模式标志”
 * \param[in] max_speed_hz  : 设备支持的最大速度
 * \param[in] cs_pin        : 片选引脚编号(\a pfunc_cs 为NULL时，本参数有效)
 * \param[in] pfunc_cs      : 片选控制函数(本参数不为 NULL 时，cs_pin 参数无效)
 *
 * \return 无
 *
 * \par 范例
 * 见 am_spi_setup()
 */
am_static_inline
void am_spi_mkdev (am_spi_device_t *p_dev,
                   am_spi_handle_t  handle,
                   uint8_t          bits_per_word,
                   uint16_t         mode,
                   uint32_t         max_speed_hz,
                   int              cs_pin,
                   void           (*pfunc_cs)(am_spi_device_t *, int ))
{
    p_dev->handle        = handle;
    p_dev->bits_per_word = bits_per_word;
    p_dev->mode          = mode;
    p_dev->max_speed_hz  = max_speed_hz;
    p_dev->cs_pin        = cs_pin;
    p_dev->pfunc_cs      = pfunc_cs;
}

/**
 * \brief SPI传输结构体参数设置
 *
 * \param[in] p_trans       : 指向SPI传输结构体的指针
 * \param[in] p_txbuf       : 指向发送数据缓存的指针,仅接收时应该置为NULL
 * \param[in] p_rxbuf       : 指向接收数据缓存的指针,仅发送时应该置为NULL
 * \param[in] nbytes        : 当前传输数据长度，字节数（16位数据时，字节数应该
 *                             是2 * N（数据个数））
 * \param[in] cs_change     : 传输结束是否影响片选，0-不影响，1-影响
 * \param[in] bits_per_word : 位宽，为0时使用设备中默认的位宽
 * \param[in] delay_usecs   : 传输结束后的延时，单位：us
 * \param[in] speed_hz      : 传输速率，为0时使用设备中默认的最大传输速率
 * \param[in] flags         : SPI传输控制标志位，AM_SPI_TRANS_*的宏值或者
 *                            多个AM_SPI_TRANS_*宏的 或（OR）值
 *
 * \return 无
 *
 * \note 如果发送和接收数据缓冲区都有效，务必确保它们的数据长度是一致的。
 *       关于缓冲区的类型和数据传输字节数：
 *        - 1  ~ 8 位宽时，uint8_t 类型，nbytes = 数据传输的个数
 *        - 9  ~ 16位宽时，uint16_t类型，nbytes = 2 * 数据传输的个数
 *        - 17 ~ 31位宽时，uint32_t类型，nbytes = 4 * 数据传输的个数
 */
am_static_inline
void am_spi_mktrans (am_spi_transfer_t *p_trans,
                     const void        *p_txbuf,
                     void              *p_rxbuf,
                     uint32_t           nbytes,
                     uint8_t            cs_change,
                     uint8_t            bits_per_word,
                     uint16_t           delay_usecs,
                     uint32_t           speed_hz,
                     uint32_t           flags)
{
    p_trans->p_txbuf        = p_txbuf;
    p_trans->p_rxbuf        = p_rxbuf;
    p_trans->nbytes         = nbytes;
    p_trans->cs_change      = cs_change;
    p_trans->bits_per_word  = bits_per_word;
    p_trans->delay_usecs    = delay_usecs;
    p_trans->speed_hz       = speed_hz;
    p_trans->flags          = flags;
}

/**
 * \brief 初始化消息
 *
 * \param[in] p_msg        : SPI消息描述符指针
 * \param[in] pfn_complete : 传输完成回调函数
 * \param[in] p_arg        : 传递给回调函数的参数
 *
 * \return 无
 */
am_static_inline
void am_spi_msg_init (am_spi_message_t  *p_msg,
                      am_pfnvoid_t       pfn_complete,
                      void              *p_arg)
{
    AM_INIT_LIST_HEAD(&p_msg->transfers);

    p_msg->p_spi_dev     = NULL;
    p_msg->actual_length = 0;
    p_msg->pfn_complete  = pfn_complete;
    p_msg->p_arg         = p_arg;
    p_msg->status        = -AM_ENOTCONN;
    p_msg->ctlrdata[0]   = NULL;
    p_msg->ctlrdata[1]   = NULL;
}

/**
 * \brief 将传输添加到消息末尾
 *
 * \param[in] p_msg   : 指向消息的指针
 * \param[in] p_trans : 待加入消息的传输
 *
 * \return 无
 */
am_static_inline
void am_spi_trans_add_tail (am_spi_message_t  *p_msg,
                            am_spi_transfer_t *p_trans)
{
    am_list_add_tail(&p_trans->trans_node, &p_msg->transfers);
}

/**
 * \brief 将传输从消息中删除
 * \param[in] p_trans : 待加入删除的传输
 * \return 无
 * \note 删除前，务必确保该传输确实已经加入了消息中
 */
am_static_inline
void am_spi_trans_del (am_spi_transfer_t *p_trans)
{
    am_list_del(&p_trans->trans_node);
}

/** 
 * \brief 获取SPI控制器的信息
 * 
 * \param[in]  handle : SPI标准服务操作句柄
 * \param[out] p_info : 用于获取SPI控制器信息的指针
 *
 * \retval  AM_OK     : 获取信息成功
 * \retval -AM_EINVAL : 获取信息失败, 参数错误
 */
am_static_inline
int am_spi_info_get (am_spi_handle_t handle, am_spi_info_t *p_info)
{
    return handle->p_funcs->pfn_spi_info_get(handle->p_drv,p_info);
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
 * am_spi_device_t spi_dev;     // 设备描述结构
 *
 * // 初始化设备描述结构
 * am_spi_mkdev(&spi_dev,
 *              handle,         // SPI标准服务handle
 *              8，             // 字大小为8-bit
 *              AM_SPI_MODE_0,  // SPI 模式0
 *              500000,         // 支持的最大速度 500000 Hz
 *              PIO0_3,         // 片选引脚为 PIO0_3
 *              NULL);          // 无自定义的片选控制函数
 *
 * // 设置设备
 * am_spi_setup(&spi_dev);
 *
 * \endcode
 */
am_static_inline
int am_spi_setup (am_spi_device_t *p_dev)
{
    return p_dev->handle->p_funcs->pfn_spi_setup(p_dev->handle->p_drv, p_dev);
}

/**
 * \brief SPI传输函数
 *
 * \param[in] p_dev   : SPI从机设备
 * \param[in] p_msg   : 传输的消息
 *
 * \retval  AM_OK     : 传输正常进行处理
 * \retval -AM_EINVAL : 输入参数错误
 *  传输过程以及结果状态，通过传输回调函数获取状态位
 *    \li  AM_OK  : 传输完成
 *    \li -AM_EIO : 传输错误
 */
am_static_inline
int am_spi_msg_start (am_spi_device_t   *p_dev,
                      am_spi_message_t  *p_msg)
{
    return p_dev->handle->p_funcs->pfn_spi_msg_start(p_dev->handle->p_drv, 
                                                     p_dev, 
                                                     p_msg);
}

/**
 * \brief 先写后读
 *
 * 本函数先进行SPI写操作，再进行读操作。
 * 在实际应用中，写缓冲区可以为地址，读缓冲区为要从该地址读取的数据。
 *
 * \param[in]  p_dev   : SPI从机设备
 * \param[in]  p_txbuf : 数据发送缓冲区
 * \param[in]  n_tx    : 要发送的数据字节个数
 * \param[out] p_rxbuf : 数据接收缓冲区
 * \param[in]  n_rx    : 要接收的数据字节个数
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
int am_spi_write_then_read (am_spi_device_t *p_dev,
                            const uint8_t   *p_txbuf,
                            size_t           n_tx,
                            uint8_t         *p_rxbuf,
                            size_t           n_rx);

/**
 * \brief 执行两次写操作
 *
 * 本函数连续执行两次写操作，依次发送数据缓冲区0和缓冲区1中的数据。
 * 在实际应用中，缓冲区0可以为地址，缓冲区1为要写入该地址的数据。
 *
 * \param[in] p_dev    : SPI从机设备
 * \param[in] p_txbuf0 : 数据发送缓冲区0
 * \param[in] n_tx0    : 缓冲区0数据个数
 * \param[in] p_txbuf1 : 数据发送缓冲区1
 * \param[in] n_tx1    : 缓冲区1数据个数
 *
 * \retval AM_OK      : 消息处理成功
 * \retval -AM_EINVAL : 参数错误
 * \retval -AM_EIO    : 传输出错
 */
int am_spi_write_then_write (am_spi_device_t *p_dev,
                             const uint8_t   *p_txbuf0,
                             size_t           n_tx0,
                             const uint8_t   *p_txbuf1,
                             size_t           n_tx1);

/** 
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_SPI_H */

/*end of file */
