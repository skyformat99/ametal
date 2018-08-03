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
 * \brief I2C标准接口
 *
 * \par 简单示例
 * \code
 * #include "am_i2c.h"
 *
 * am_i2c_device_t dev;                     // 设备描述
 * uint8_t         read_buf[16];            // 读缓存区
 * uint8_t         write_buf[16];           // 写缓存区
 *
 * // 生成从机设备
 * am_i2c_mkdev(&dev,
 *              i2c_handle,
 *              0x50,
 *              AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
 *
 * // 写入数据到从器件子地址0开始的16个字节
 * am_i2c_write(&dev, 0, &write_buf[0], 16);
 *
 * // 从器件子地址0开始读16个字节
 * am_i2c_read(&dev, 0, &read_buf[0], 16);
 * \endcode
 *
 * \internal
 * \par Modification History
 * - 1.03 15-12-08  cyl, delete the am_i2c_connect.
 * - 1.02 15-10-27  tee, add the concept of message.
 * - 1.01 15-08-19  tee, modified some interface.
 * - 1.00 14-11-01  jon, first implementation.
 * \endinternal
 */

#ifndef __AM_I2C_H
#define __AM_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"

/**
 * @addtogroup am_if_i2c
 * @copydoc am_i2c.h
 * @{
 */

/**
 * \name I2C 传输控制标识，用于am_i2c_transfer的flags成员
 * @{ 
 * 定义I2C传输的地址类型、读写类型、特殊控制等标志
 *
 * 这些标志相“或”之后的结果用于给结构体成员
 * <code>am_i2c_transfer::flags</code>赋值，未显式给出的标志将使用默认值。
 * 例如，下面的这些赋值将得到不同的控制效果：
 * \code
 * am_i2c_transfer_t trans;
 * trans.flags = 0;             // 7-bit从机地址(默认)、写操作(默认)
 * trans.flags = AM_I2C_M_WR;   // 7-bit从机地址(默认)、写操作
 * trans.flags = AM_I2C_M_RD;   // 7-bit从机地址(默认)、读操作
 * trans.flags = AM_I2C_M_10BIT | AM_I2C_M_RD;   // 10-bit从机地址、读操作
 *
 * trans.flags = AM_I2C_M_7BIT |
 *               AM_I2C_M_WR |
 *               AM_I2C_M_IGNORE_NAK; // 7-bit从机地址、写操作、忽略无应答错误
 * \endcode
 *
 * \sa struct am_i2c_transfer
 */

#define AM_I2C_M_7BIT          0x0000u    /**< \brief 7-bits 设备地址  */
#define AM_I2C_M_10BIT         0x0001u    /**< \brief 10-bits 设备地址 */
#define AM_I2C_M_WR            0x0000u    /**< \brief 写操作           */
#define AM_I2C_M_RD            0x0002u    /**< \brief 读操作           */
#define AM_I2C_M_NOSTART       0x0004u    /**< \brief 无需重新启动     */
#define AM_I2C_M_REV_DIR_ADDR  0x0008u    /**< \brief 读写标志位反转   */
#define AM_I2C_M_RECV_LEN      0x0010u    /**< \brief 暂不支持         */

/** \brief 忽略所有无应答错误 (包括从机地址无应答和数据无应答) */
#define AM_I2C_M_IGNORE_NAK    0x0020u

/** \brief 在读操作中接收数据时不发送应答    */
#define AM_I2C_M_NO_RD_ACK     0x0040u

/** \brief I2C传输控制标志掩码 */
#define AM_I2C_M_MASK          0x00FFu

/** @} */


/**
 * \name I2C 从设备属性标志，用于 am_i2c_device 的 dev_flags 成员
 * @{ 
 * 定义I2C从机地址类型、器件内子地址类型和器件内子地址宽度等标志
 *
 * 例如，下面的这些赋值将定义不同的从设备属性：
 * \code
 * am_i2c_device_t dev;
 * dev.flags = 0;                     // 7-bit 从机地址、无子地址
 * dev.flags = AM_I2C_SUBADDR_1BYTE;  // 7-bit 从机地址、1字节子地址
 * dev.flags = AM_I2C_SUBADDR_2BYTE;  // 7-bit 从机地址、2字节子地址、
 *                                    // 子地址高位地址先传输
 *
 * dev.flags = AM_I2C_SUBADDR_2BYTE |
 *             AM_I2C_SUBADDR_LSB_FIRST; // 7-bit 从机地址、2字节子地址、
 *                                       // 子地址低位字节先传输
 *
 * \endcode
 *
 * \sa struct am_i2c_device
 */

/** \brief 7位地址模式(默认模式) */
#define AM_I2C_ADDR_7BIT         AM_I2C_M_7BIT

/** \brief 10位地址模式 */
#define AM_I2C_ADDR_10BIT        AM_I2C_M_10BIT

/** \brief 忽略设备的无应答 */
#define AM_I2C_IGNORE_NAK        AM_I2C_M_IGNORE_NAK

/** \brief 器件内子地址高位字节先传输(默认)*/
#define AM_I2C_SUBADDR_MSB_FIRST 0x0000u

/** \brief 器件内子地址低位字节先传输 */
#define AM_I2C_SUBADDR_LSB_FIRST 0x0100u

#define AM_I2C_SUBADDR_NONE      0x0000u  /**< \brief 无子地址(默认)  */
#define AM_I2C_SUBADDR_1BYTE     0x1000u  /**< \brief 子地址宽度1字节 */
#define AM_I2C_SUBADDR_2BYTE     0x2000u  /**< \brief 子地址宽度2字节 */

/** \brief 从标志控制字中取得子地址宽度 */
#define AM_I2C_SUBADDR_LEN_GET(flags)  (((flags) & 0xF000) >> 12)

/** @} */

struct am_i2c_message;

/**
 * \brief I2C驱动函数结构体
 */
struct am_i2c_drv_funcs {
    
    /** \brief 开始消息处理 */
    int (*pfn_i2c_msg_start) (void *p_drv, struct am_i2c_message *p_trans);
    
};

/**
 * \brief I2C标准服务结构体 
 */
typedef struct am_i2c_serv {
    struct am_i2c_drv_funcs   *p_funcs;   /**< \brief I2C驱动函数         */
    void                      *p_drv;     /**< \brief I2C驱动函数入口参数 */
} am_i2c_serv_t;

/** \brief I2C标准服务操作句柄定义 */
typedef am_i2c_serv_t *am_i2c_handle_t;

/**
 * \brief I2C从设备描述结构体 (推荐使用 am_i2c_mkdev() 设置本数据结构) 
 */
typedef struct am_i2c_device {
    am_i2c_handle_t handle;    /**< \brief 与从设备关联的I2C服务标准handle    */
    uint16_t        dev_addr;  /**< \brief 从机设备地址，7位或10位            */ 
    uint16_t        dev_flags; /**< \brief 从机设备特性，见“I2C从设备属性标志”*/
} am_i2c_device_t;

/**
 * \brief I2C 传输结构体 (推荐使用 am_i2c_mktrans() 设置本数据结构) 
 */ 
typedef struct am_i2c_transfer {
    uint16_t    addr;         /**< \brief I2C设备地址                       */
    uint16_t    flags;        /**< \brief I2C 传输标志 见“I2C传输控制标志”  */
    uint8_t    *p_buf;        /**< \brief 数据缓冲区                        */
    uint32_t    nbytes;       /**< \brief 数据个数                          */
} am_i2c_transfer_t;

/** 
 * \brief I2C 消息 (推荐使用 am_i2c_mkmsg() 设置本数据结构) 
 */
typedef struct am_i2c_message {
    am_i2c_transfer_t  *p_transfers;  /**< \brief 组成消息的传输              */
    uint16_t            trans_num;    /**< \brief 请求处理的传输个数          */
    uint16_t            done_num;     /**< \brief 成功处理的传输个数          */
    am_pfnvoid_t        pfn_complete; /**< \brief 传输完成回调函数            */
    void               *p_arg;        /**< \brief 传递给 pfn_complete 的参数  */
    int                 status;       /**< \brief 消息的状态                  */
    void               *ctlrdata[2];  /**< \brief 控制器使用                  */
} am_i2c_message_t;


/**
 * \brief I2C从机设备描述结构体参数设置
 *
 * \param[in] p_dev     : 指向从机设备描述结构体的指针
 * \param[in] handle    : 与从设备关联的I2C标准服务操作句柄
 * \param[in] dev_addr  : 从机设备地址
 * \param[in] dev_flags : 从机设备特性，见“I2C从设备属性标志”
 *
 * \return 无
 *
 * \par 范例
 * \code
 * #include "am_i2c.h"
 *
 * am_i2c_device_t cat1025; // CAT1025从设备描述结构
 *
 * // 初始化CAT1025设备描述结构：位于I2C0控制器，设备地址0x50，
 * // 寄存器地址为1字节宽
 * am_i2c_mkdev(&cat1025,
 *              i2c_handle,
 *              0x50,
 *              AM_I2C_ADDR_7BIT | AM_I2C_SUBADDR_1BYTE);
 * \endcode
 */
am_static_inline
void am_i2c_mkdev (am_i2c_device_t *p_dev,
                   am_i2c_handle_t  handle,
                   uint16_t         dev_addr,
                   uint16_t         dev_flags)

{
    p_dev->handle     = handle;
    p_dev->dev_addr   = dev_addr;
    p_dev->dev_flags  = dev_flags;
}

/**
 * \brief I2C传输结构体信息参数设置
 *
 * \param[in] p_trans : 指向传输结构体的指针
 * \param[in] addr    : 操作的从机设备地址
 * \param[in] flags   : 传输过程中的控制标识位，AM_I2C_M_*的宏值或者多个
 *                      AM_I2C_M_*宏的或（OR）值 (#AM_I2C_M_RD) 
 * \param[in] p_buf   : 发送或者接收数据缓冲区
 * \param[in] nbytes  : 传输的数据个数
 *
 * \retval  AM_OK     : 传输结构体参数设置完成
 * \retval -AM_EINVAL : 参数错误
 */
am_static_inline
void am_i2c_mktrans (am_i2c_transfer_t *p_trans, 
                     uint16_t           addr,  
                     uint16_t           flags, 
                     uint8_t           *p_buf, 
                     uint32_t           nbytes)
{
    p_trans->addr   = addr;
    p_trans->flags  = flags;
    p_trans->p_buf  = p_buf; 
    p_trans->nbytes = nbytes;
}

/**
 * \brief 设置I2C消息结构体参数
 *
 * \param[in] p_msg          : I2C消息描述符指针
 * \param[in] p_transfers    : 组成消息的“I2C传输”
 * \param[in] trans_num      : “I2C传输”的个数
 * \param[in] pfn_complete   : 消息处理完成回调函数
 * \param[in] p_arg          : 传递给回调函数的参数
 *
 * \par 范例
 * \code
 * #include "am_i2c.h"
 *
 * am_i2c_message_t  msg;       // 消息描述结构体
 * am_i2c_transfer_t trans[2];  // 传输描述结构
 * uint8_t data0[16]            // 数据缓冲区 0
 * uint8_t data1[16];           // 数据缓冲区 1
 *
 * // 设置传输描述结构体
 * am_i2c_mktrans(&trans[0],    // 传输描述结构 0
 *                0x50,         // 要传输的目标器件(从机)地址
 *                AM_I2C_M_7BIT | AM_I2C_M_WR，// 7-bit从机地址、写操作
 *                1,            // 数据长度，1字节
 *                &data0[0]);   // 数据缓冲区 0
 *
 * am_i2c_mktrans(&trans[1],    // 传输描述结构 1
 *                0x50,         // 要传输的目标器件(从机)地址
 *                AM_I2C_M_7BIT | AM_I2C_M_RD，// 7-bit从机地址、读操作
 *                16,           // 数据长度，16字节
 *                &data1[0]);   // 数据缓冲区1
 *
 * // 设置消息描述结构体
 * am_i2c_mkmsg(&msg,           // 消息描述结构
 *              &trans[0],      // 组成消息的传输
 *              2,              // 传输个数
 *              my_callback,    // 传输完成回调函数
 *              my_arg);        // 传输完成回调函数的参数
 *
 * \endcode
 */
am_static_inline
void am_i2c_mkmsg (am_i2c_message_t  *p_msg,
                   am_i2c_transfer_t *p_transfers,
                   uint16_t           trans_num,
                   am_pfnvoid_t       pfn_complete,
                   void              *p_arg)
{
    p_msg->p_transfers  = p_transfers;
    p_msg->trans_num    = trans_num;
    p_msg->done_num     = 0;
    p_msg->pfn_complete = pfn_complete;
    p_msg->p_arg        = p_arg;
    p_msg->status       = -AM_ENOTCONN;
}

/**
 * \brief 开始处理一个消息
 *
 *     以异步的方式处理消息，若当前I2C控制器空闲，则该消息会得到立即执行，
 * 若处理器不空闲，则会将该消息加入一个队列中排队等待，I2C控制器将顺序处理
 * 队列中的消息。消息的处理状态和结果反映在\a p_msg->status。
 * 消息处理结束(成功、超时或出错)时，将会调用\a p_msg->pfn_complete 并传递
 * 参数\a p_msg->p_arg。
 *
 * \param[in]     handle : I2C标准服务操作句柄
 * \param[in,out] p_msg  : 要处理的消息
 *
 * 函数的返回值如下：
 * \retval  AM_OK      : 消息排队成功，等待处理
 * \retval  -AM_EINVAL : 参数错误
 *
 * 成功处理的传输个数反映在\a p_msg->done_num
 *
 * 消息的处理状态和结果反映在\a p_msg->status ：
 *
 *      \li  -AM_ENOTCONN    消息尚未排队
 *      \li  -AM_EISCONN     消息正在排队处理
 *      \li  -AM_EINPROGRESS 消息正在被处理
 * 下面为消息处理完毕的结果 (可通过\a p_msg->done_num 定位出错的传输)
 *      \li  AM_OK           所有传输成功处理
 *      \li  -AM_ENOTSUP     某次传输的控制标志不支持
 *      \li  -AM_ENODEV      某次传输的目标地址无应答
 *      \li  -AM_ENOENT      某次传输发送的数据无应答
 *      \li  -AM_EIO         传输消息时发生总线错误、总线仲裁失败过多等错误
 *      \li  -AM_ECANCELED   因控制器出错或处理队列满，消息被取消，可稍后再试
 *
 * \par 示例
 * \code
 *  #include "am_i2c.h"
 *
 * bool_t g_trans_complete = FALSE;
 *
 * void eeprom_callback (void *p_arg) 
 * {
 *      g_trans_complete = TRUE;
 *
 * }
 *
 * int main ()
 * {
 *     am_i2c_message_t  msg;
 *     am_i2c_transfer_t trans[2];
 *     uint8_t           subaddr;
 *     uint8_t           data[EEPROM_PAGE_SIZE];
 *     int               i;
 *     am_i2c_handle_t   i2c_handle = NULL;
 *
 *     // 写EEPROM子地址0x00开始的一个页 (时序请参考数据手册)
 *
 *     // 第1个传输发送子地址
 *     subaddr = 0x00;
 *     am_i2c_mktrans(&trans[0],
 *                    EEPROM_ADDR,                 // 目标器件地址
 *                    AM_I2C_M_7BIT | AM_I2C_M_WR, // 7-bit器件地址, 写操作
 *                    &subaddr,                    // 子地址
 *                    1);                          // 子地址长度，1byte
 *     
 *     // 第2个传输发送数据
 *     for (i = 0; i < EEPROM_PAGE_SIZE; i++) {
 *         data[i] = i;
 *     }
 *     am_i2c_mktrans(&trans[1],
 *                    EEPROM_ADDR,                 // 目标器件地址 (将被忽略)
 *     
 *                    // 写操作, 不发送起始信号及从机地址
 *                    AM_I2C_M_WR | AM_I2C_M_NOSTART,
 *     
 *                    &data[0],                    // 数据缓冲区
 *                    EEPROM_PAGE_SIZE);           // 数据个数
 *     
 *     // 将传输组成消息
 *     am_i2c_mkmsg(&msg,
 *                  &trans[0],
 *                  2,
 *                  eeprom_callback,
 *                  &sem_sync);
 *     
 *     // 开始处理此消息，使用i2c_handle之前，应根据实际平台获取首先获取一个i2c_handle
 *     if (am_i2c_msg_start(i2c_handle, &msg) == AM_OK) {
 *          
 *          while (g_trans_complete == FALSE);
 *         
 *           if (msg.status != AM_OK) {
 *               // 传输出现错误
 *           }
 *     }
 *     
 *     while(1) {
 *
 *     }
 * }
 * 
 * \endcode
 */
am_static_inline
int am_i2c_msg_start (am_i2c_handle_t handle, am_i2c_message_t *p_msg)
{
    return handle->p_funcs->pfn_i2c_msg_start(handle->p_drv, p_msg);
}

/**
 * \brief I2C写数据
 *
 * \param[in] p_dev    : 指向从机设备信息的结构体的指针
 * \param[in] sub_addr : 从机设备子地址
 * \param[in] p_buf    : 指向发送/接收数据缓存
 * \param[in] nbytes   : 数据缓存长度
 *
 * \return  AM_OK : 写数据完成
 */
int am_i2c_write(am_i2c_device_t *p_dev,
                 uint32_t         sub_addr,
                 const uint8_t   *p_buf, 
                 uint32_t         nbytes);
/**
 * \brief I2C读数据
 *
 * \param[in] p_dev    : 指向从机设备信息的结构体的指针
 * \param[in] sub_addr : 从机设备子地址
 * \param[in] p_buf    : 指向发送/接收数据缓存
 * \param[in] nbytes   : 数据缓存长度
 *
 * \return  AM_OK : 读数据完成 
 */
int am_i2c_read(am_i2c_device_t *p_dev,
                uint32_t         sub_addr,
                uint8_t         *p_buf, 
                uint32_t         nbytes);
/** 
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_I2C_H */

/* end of file */
