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
 * \brief 版本号标准接口
 *
 * \internal
 * \par modification history:
 * - 1.00 17-08-8  nwt, first implementation
 * \endinternal
 */

#ifndef __AM_VERSION_H
#define __AM_VERSION_H

#include <stdarg.h>
#include <stdlib.h>
#include "am_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_version
 * \copydoc am_version.h
 * @{
 */

/**
 * \brief get version information
 * \return  the string point of version information
 */
const char *am_sdk_version_get (void);

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_VERSION_H */

/* end of file */
