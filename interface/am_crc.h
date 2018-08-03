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
 * \brief 循环冗余校验(CRC)标准接口
 *
 * \internal
 * \par modification history
 * - 1.00 15-01-19  tee, first implementation
 * \endinternal
 */

#ifndef __AM_CRC_H
#define __AM_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_crc
 * \copydoc am_crc.h
 * @{
 */
#include "am_common.h"

/**
 * \brief CRC 模型
 */
typedef struct am_crc_pattern {
    uint8_t    width;     /**< \brief CRC宽度(如：CRC5 的宽度为 5)            */
    uint32_t   poly;      /**< \brief CRC生成多项式                           */
    uint32_t   initvalue; /**< \brief CRC初始值                               */
    am_bool_t  refin;     /**< \brief 如果输入数据需要位反转，则该值为TRUE    */
    am_bool_t  refout;    /**< \brief 如果输出结果值需要位反转，则该值为TRUE  */
    uint32_t   xorout;    /**< \brief 输出异或值                              */
} am_crc_pattern_t;


/**
 * \brief CRC驱动函数结构体
 */
struct am_crc_drv_funcs {

    /** \brief 根据CRC模型初始化CRC，为CRC计算做好准备 */
    int (*pfn_crc_init) (void *p_cookie, am_crc_pattern_t *p_pattern);

    /** \brief CRC计算                                 */
    int (*pfn_crc_cal) (void *p_cookie,const uint8_t *p_data, uint32_t nbytes);

    /** \brief 获取CRC计算结果                         */
    int (*pfn_crc_final) (void *p_cookie, uint32_t *p_value);
};

/** 
 * \brief CRC服务
 */
typedef struct am_crc_serv {

    /** \brief CRC驱动函数结构体指针    */
    struct am_crc_drv_funcs *p_funcs;
    
    /** \brief 用于驱动函数的第一个参数 */
    void                    *p_drv;     
} am_crc_serv_t;

/** \brief CRC标准服务操作句柄类型定义  */
typedef am_crc_serv_t *am_crc_handle_t;


/** 
 * \brief 根据CRC模型初始化CRC，为CRC计算做好准备
 * 
 * \param[in] handle    : CRC标准服务操作句柄
 * \param[in] p_pattern : 指向CRC模型信息结构体指针
 *
 * \retval  AM_OK       : 初始化成功
 * \retval -AM_EINVAL   : 初始化失败, 参数错误
 * \retval -AM_ENOTSUP  : 初始化失败, CRC模型不支持
 */
am_static_inline
int am_crc_init (am_crc_handle_t handle, am_crc_pattern_t *p_pattern)
{
    return handle->p_funcs->pfn_crc_init(handle->p_drv, p_pattern);
}

/** 
 * \brief CRC计算(数据处理)
 * 
 * \param[in] handle  : CRC标准服务操作句柄
 * \param[in] p_data  : 指向待计算数据缓冲区
 * \param[in] nbytes  : 待计算数据的个数
 *
 * \retval  AM_OK     : CRC计算成功
 * \retval -AM_EINVAL : CRC计算失败, 参数错误
 */
am_static_inline
int am_crc_cal (am_crc_handle_t handle, const uint8_t *p_data, uint32_t nbytes)
{
    return handle->p_funcs->pfn_crc_cal(handle->p_drv, p_data, nbytes);
}

/** 
 * \brief 获取CRC计算结果值
 *
 * \param[in]  handle  : CRC标准服务操作句柄
 * \param[out] p_value : 用于获取CRC计算结果值
 *
 * \retval  AM_OK      : 获取CRC计算结果值成功
 * \retval -AM_EINVAL  : 获取CRC计算结果值失败, 参数错误
 */
am_static_inline
int am_crc_final (am_crc_handle_t handle, uint32_t *p_value) 
{
    return handle->p_funcs->pfn_crc_final(handle->p_drv, p_value);
}

/** 
 * @}  
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_CRC_H */

/* end of file */
