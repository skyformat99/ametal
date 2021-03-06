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
 * \brief SPI 主机中断方式例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 将 SPI 的 MOSI 引脚和 MISO 引脚用线相连，模拟从机设备，回环测试。
 *
 * - 实验现象：
 *   1. 主机通过 MOSI 发送数据，发出的数据从 MOSI 读回；
 *   2. 调试串口打印测试结果。
 *
 * \note
 *    如需观察串口打印的调试信息，需要将 PIOA_9 引脚连接 PC 串口的 RXD。
 *
 * \par 源代码
 * \snippet demo_am116_core_std_spi_master_int.c src_am116_core_std_spi_master_int
 *
 * \internal
 * \par History
 * - 1.00 17-04-25  ari, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_am116_core_std_spi_master_int
 * \copydoc demo_am116_core_std_spi_master_int.c
 */

/** [src_am116_core_std_spi_master_int] */
#include "ametal.h"
#include "am_vdebug.h"
#include "am_zlg116.h"
#include "am_zlg116_inst_init.h"
#include "demo_std_entries.h"

/**
 * \brief 例程入口
 */
void demo_am116_core_std_spi_master_int_entry (void)
{
    AM_DBG_INFO("demo am116_core std spi master int!\r\n");

    demo_std_spi_master_entry(am_zlg116_spi1_int_inst_init(), PIOA_4);
}
/** [src_am116_core_std_spi_master_int] */

/* end of file */
