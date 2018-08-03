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
 * \brief 延时标准接口
 * 
 * 包括毫秒级别延时和微秒级别延时
 * 
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DELAY_H
#define __AM_DELAY_H


#ifdef __cplusplus
extern "C" {
#endif

#include "am_types.h"

/**
 * \addtogroup am_if_delay
 * \copydoc am_delay.h
 * @{
 */
     
/** 
 * \brief 毫秒级别延时
 * \param[in] nms : 延时的毫秒数
 * \return 无
 */   
void am_mdelay(uint32_t nms);
    
/** 
 * \brief 微秒级别延时
 * \param[in] nus : 延时的微秒数
 * \return 无
 */    
void am_udelay(uint32_t nus);
 
/**
 * @} 
 */
 
#ifdef __cplusplus
}
#endif

#endif /* __AM_DELAY_H */

/* end of file */
