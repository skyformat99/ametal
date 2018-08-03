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
 * \brief NVRAM(非易失性存储器)标准接口
 *
 * \par 简单示例
 * \code
 *  #include "am_nvram.h"
 *
 *  char ip[4] = {1, 1, 1, 1};
 *  char data[4];
 *
 *  am_nvram_set("ip", 0, &ip[0],   0, 4);    // 设置非易失性数据"ip"
 *  am_nvram_get("ip", 0, &data[0], 0, 4);    // 读取非易失性数据"ip"
 * \endcode
 *
 * // 更多内容待添加。。。
 *
 * \internal
 * \par modification history:
 * - 1.00 15-09-12  tee,first implementation
 * \endinternal
 */

#ifndef __AM_NVRAM_H
#define __AM_NVRAM_H

#ifdef __cplusplus
extern "C" {
#endif   /* __cplusplus  */

/**
 * \addtogroup am_if_nvram
 * \copydoc am_nvram.h
 * @{
 */
   
#include "ametal.h"

/** 
 * \brief 存储段类型定义
 *
 *      该结构体主要用于描述一个存储段，每个存储段都必须处于某个存储器设备中，该存
 *  储器设备使用 p_dev_name 和 dev_unit 确定。系统使用的所有存储段都应直接定义于
 *  \sa am_nvram_cfg() 文件中。
 *
 *      假定应用需要4个存储段：分别保存两个IP地址（4字节×2），温度上限值（4字节），
 *  系统参数（50字节）。系统中可以作为NVRAM的设备为 FM24C02，则可以定义如下存储
 *  段列表：
 *  const am_nvram_segment_t g_nvram_segs[] = {
 *      {"ip",         0,  0,  4,   "fm24c02"},
 *      {"ip",         1,  4,  4,   "fm24c02"},
 *      {"temp_limit", 0,  8,  4,   "fm24c02"},
 *      {"system",     0,  12, 50,  "fm24c02"},
 *      {"test",       0,  62, 178, "fm24c02"},
 *      {NULL,         0,   0,   0,      NULL}  // 空存储段，必须保留
 * };
 */
typedef struct am_nvram_segment {
    char       *p_name;       /**< \brief 存储段名字                    */
    int         unit;         /**< \brief 存储段单元号                  */
    uint32_t    seg_addr;     /**< \brief 存储段在存储器件中的起始地址  */
    uint32_t    seg_size;     /**< \brief 存储段的大小                  */
    const char *p_dev_name;   /**< \brief 存储段所处的存储器设备名      */
} am_nvram_segment_t;

/**
 * \brief NVRAM 驱动函数结构体
 */
struct am_nvram_drv_funcs {
    
    /** \brief 指向 NVRAM 数据获取函数*/
    int (*pfn_nvram_get) (void       *p_drv,
                          int         offset,
                          uint8_t    *p_buf,
                          int         len);
 
    /** \brief 指向 NVRAM 数据设置函数 */
    int (*pfn_nvram_set) (void          *p_drv,
                          int            offset,
                          uint8_t       *p_buf,
                          int            len);
};

/**
 * \brief NVRAM存储器设备类型定义
 */
typedef struct am_nvram_dev {
    const char                      *p_dev_name; /**< \brief 设备名           */
    const struct am_nvram_drv_funcs *p_funcs;    /**< \brief 对应的驱动函数   */
    void                            *p_drv;      /**< \brief 驱动函数参数     */
    struct am_nvram_dev             *p_next;     /**< \brief 指向下一个设备   */
} am_nvram_dev_t;

/**
 * \brief NVRAM 服务初始化
 * \param[in] p_seglist : 系统存储段列表
 * \return AM_OK，初始化成功；其它值，初始化失败。  
 * \note 该函数一般在系统启动时调用，应用程序无需直接使用该函数
 */
int am_nvram_init (const am_nvram_segment_t *p_seglist);

/**
 * \brief 向系统中注册一个NVRAM存储器设备
 * \param[in] p_dev : 待注册的设备
 * \return AM_OK，注册成功；其它值，注册失败。
 * \note 该函数一般由存储器件的驱动调用，应用程序无需直接使用该函数
 */
int am_nvram_dev_register (am_nvram_dev_t *p_dev);

/**
 * \brief 取消一个已注册的NVRAM存储器设备
 * \param[in] p_dev : 待取消的已经注册的 NVRAM存储器设备
 * \return AM_OK，取消注册成功；其它值，取消注册失败。
 * \note 该函数一般由存储器件的驱动调用，应用程序无需直接使用该函数
 */
int am_nvram_dev_unregister (am_nvram_dev_t *p_dev);

/**
 * \brief 读取一条非易失性存储信息
 *
 * 本函数从非易失性存储设备中读取非易失性信息并保存到调用者提供的缓冲区中。任何
 * 需要使用NVRAM服务的模块都可以使用本函数。
 *
 * 调用者使用\a p_name 和\a unit 来标识一条非易失性信息。
 *
 * 拷贝的起始位置由\a offset 指定，拷贝数据总量由\a len 指定。如果指定的大小超出
 * 了该非易失性信息的大小，则其行为未定义。
 *
 * \param[in] p_name  非易失性存储信息的名字
 * \param[in] unit    非易失性存储信息的单元号
 * \param[in] p_buf   写入数据缓冲区
 * \param[in] offset  在非易失性存储信息中的偏移
 * \param[in] len     要存储的长度
 *
 * \retval AM_OK      成功
 * \retval -AM_EINVAL  \a p_name 或 \a p_buf 为NULL
 * \retval -AM_EIO    指定的非易失性存储信息不存在或操作失败
 *
 * \par 示例
 * \code
 *  #include "aw_nvram.h"
 *
 *  char ip[4];
 *  aw_nvram_get("ip", 0, &ip[0], 0, 4);    // 读取非易失性数据"ip"
 * \endcode
 *
 * \sa aw_nvram_set()
 */
int am_nvram_get (char *p_name, int unit, uint8_t *p_buf, int offset, int len);

/**
 * \brief 写入一条非易失性存储信息
 *
 * 本函数从调用者提供的缓冲区中读取信息，并将其存储到非易失性存储设备中。任何
 * 需要使用NVRAM服务的模块都可以使用本函数。
 *
 * 调用者使用\a p_name 和\a unit 来标识一条非易失性信息。
 *
 * 存储的起始位置由\a offset 指定，存储数据总量由\a len 指定。如果指定的大小超出
 * 了该非易失性信息的大小，则其行为未定义。
 *
 * \param[in] p_name  非易失性存储信息的名字
 * \param[in] unit    非易失性存储信息的单元号
 * \param[in] p_buf   写入数据缓冲区
 * \param[in] offset  在非易失性存储信息中的偏移
 * \param[in] len     要存储的长度
 *
 * \retval AM_OK      成功
 * \retval -AM_EINVAL  \a p_name 或 \a p_buf 为NULL
 * \retval -AM_EIO    指定的非易失性存储信息不存在或操作失败
 *
 * \par 示例
 * \code
 *  #include "am_nvram.h"
 *
 *  char ip[4] = {1, 1, 1, 1};
 *  am_nvram_set("ip", 0, &ip[0], 0, 4);    // 写入非易失性数据"ip"
 * \endcode
 *
 * \sa am_nvram_get()
 */
int am_nvram_set (char *p_name, int unit, uint8_t *p_buf, int offset, int len);

/**
 * \brief 获取NVRAM存储段的容量大小
 *
 * \param[in]  p_name  非易失性存储信息的名字
 * \param[in]  unit    非易失性存储信息的单元号
 * \param[out] p_size  存储段的容量大小
 *
 * \retval AM_OK      成功
 * \retval -AM_EINVAL  \a p_name 或 \a p_buf 为NULL
 * \retval -AM_EIO    指定的非易失性存储信息不存在或操作失败
 *
 * \par 示例
 * \code
 *  #include "am_nvram.h"
 *  size_t size = 0;
 *  am_nvram_set("ip", 0, &size);    // 写入非易失性数据"ip"
 * \endcode
 *
 * \sa am_nvram_get()
 */
int am_nvram_size_get (char *p_name, int unit, size_t *p_size);

/** @} am_if_nvram */

#ifdef __cplusplus
}
#endif   /* __cplusplus  */

#endif   /* __AM_NVRAM_H */

/* end of file */
