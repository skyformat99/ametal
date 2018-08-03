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
 * \brief  NVRAM 接口例程
 *
 *  测试使用的存储段需在 __g_nvram_segs[] 列表中定义（am_nvram_cfg.c 文件中）
 *
 * - 实验现象：
 *   1. 调试串口打印出测试结果。
 *
 * \par 源代码
 * \snippet demo_std_nvram.c src_std_nvram
 *
 * \internal
 * \par Modification history
 * - 1.00  17-11-13  pea, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_nvram
 * \copydoc demo_std_nvram.c
 */

/** [src_std_nvram] */
#include "ametal.h"
#include "am_nvram.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_ep24cxx.h"

#define __BUF_SIZE 16 /**< \brief 缓冲区大小 */

/**
 * \brief 例程入口
 */
void demo_std_nvram_entry (char *p_nvram_name, int32_t nvram_unit, int32_t test_lenth)
{
    uint8_t  wr_buf[__BUF_SIZE] = {0}; /* 写数据缓存定义 */
    uint8_t  rd_buf[__BUF_SIZE] = {0}; /* 读数据缓存定义 */
    uint16_t i;
    int      ret;

    if (__BUF_SIZE < test_lenth) {
        test_lenth = __BUF_SIZE;
    }

    /* 填充发送缓冲区 */
    for (i = 0;i < test_lenth; i++) {
        wr_buf[i] = i;
    }

    /* 写数据 */
    ret = am_nvram_set(p_nvram_name,
                       nvram_unit,
                      &wr_buf[0],
                       0,
                       test_lenth);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_ep24cxx_write error(id: %d).\r\n", ret);
        return;
    }
    am_mdelay(5);

    /* 读数据 */
    ret = am_nvram_get(p_nvram_name,
                       nvram_unit,
                      &rd_buf[0],
                       0,
                       test_lenth);

    if (ret != AM_OK) {
        AM_DBG_INFO("am_ep24cxx_read error(id: %d).\r\n", ret);
        return;
    }

    /* 校验写入和读取的数据是否一致 */
    for (i = 0; i < test_lenth; i++) {
        AM_DBG_INFO("Read EEPROM the %2dth data is 0x%02x\r\n", i ,rd_buf[i]);

        /* 校验失败 */
        if(wr_buf[i] != rd_buf[i]) {
            AM_DBG_INFO("verify failed at index %d.\r\n", i);
            break;
        }
    }

    if (test_lenth == i) {
        AM_DBG_INFO("verify success!\r\n");
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_nvram] */

/* end of file */
