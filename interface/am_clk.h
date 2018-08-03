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
 * \brief CLK 标准接口
 *
 * \internal
 * \par Modification History
 * - 1.00 15-12-01  LNK, first implementation.
 * \endinternal
 */

#ifndef __AM_CLK_H
#define __AM_CLK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h" 

/**
 * \addtogroup am_if_clk
 * \copydoc am_clk.h
 * @{
 */

/** \brief 时钟ID数据类型*/
typedef int am_clk_id_t;

/**
 * \brief 使能时钟
 *
 * \param[in]   clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \retval      AM_OK   成功
 * \retval     -AM_ENXIO   时钟频率ID不存在
 * \retval     -AM_EIO     使能失败
 */
int am_clk_enable (am_clk_id_t clk_id);

/**
 * \brief 禁能时钟
 *
 * \param[in]   clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \retval      AM_OK     成功
 * \retval     -AM_ENXIO  时钟频率ID不存在
 * \retval     -AM_EIO       禁能失败
 */
int am_clk_disable (am_clk_id_t clk_id);

/**
 * \brief 获取时钟频率
 *
 * \param[in]   clk_id  时钟 ID (由平台定义), 参见 \ref grp_clk_id
 *
 * \retval      0       时钟频率为0，或获取时钟频率失败
 * \retval      正值           时钟频率
 */
int am_clk_rate_get (am_clk_id_t clk_id);


/** 
 * @}  
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_CLK_H */

/* end of file */
