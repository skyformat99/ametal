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
 * \brief I2C 从机例程，通过标准接口实现
 *
 * - 操作步骤：
 *   1. 连接对应引脚。
 *
 * \par 源代码
 * \snippet demo_std_i2c_slave.c src_std_i2c_slave
 *
 * \internal
 * \par Modification history
 * - 1.00  17-09-06  vir, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_i2c_slave
 * \copydoc demo_std_i2c_slave.c
 */

/** [src_std_i2c_slave] */
#include "ametal.h"
#include "am_i2c_slv.h"

/** \breif 寄存器数量 */
#define __REG_SIZE  20

static uint8_t __g_reg_buf[__REG_SIZE];

static uint8_t __g_subaddr;

/**
 * \brief 地址匹配回调函数
 */
static uint8_t __g_count = 0;
static int __addr_matching ( void * p_arg, am_bool_t is_rx)
{

    /* do some thing */
    __g_count++;
    return AM_OK;
}

/**
 * \brief 获取发送一个字节回调
 */
static int __txbyte_get (void *p_arg, uint8_t *p_byte)
{
    *p_byte = __g_reg_buf[__g_subaddr];
    __g_subaddr++;
    return AM_OK;
}

/**
 * \brief 接受一个字节回调
 */
static int __rxbyte_put(void *p_arg, uint8_t byte)
{
    __g_reg_buf[__g_subaddr] = byte;
    __g_subaddr++;
    return AM_OK;
}

/**
 * \brief 停止传输回调
 */
static void  __tran_stop(void *p_arg)
{
    __g_subaddr = 0;
}

static am_i2c_slv_cb_funcs_t __g_i2c_slv_cb_funs = {
    __addr_matching,
    __txbyte_get,
    __rxbyte_put,
    __tran_stop,
};

/**
 * \brief 例程入口
 */
void demo_std_i2c_slave_entry (am_i2c_slv_handle_t handle, uint16_t dev_addr)
{
    int                 i;
    am_i2c_slv_device_t i2c_slv_dev;

    am_i2c_slv_mkdev(&i2c_slv_dev,
                      handle,
                     &__g_i2c_slv_cb_funs,
                      dev_addr,
                      AM_I2C_SLV_ADDR_7BIT,
                      NULL);

    /* 开始运行从机设备 */
    am_i2c_slv_setup (&i2c_slv_dev);

    /**
     * \brief 初始化从机缓存区
     */
    for ( i = 0; i < __REG_SIZE; i++) {
        __g_reg_buf[i] = 5+i;
    }

    AM_FOREVER {
        ; /* VOID */
    }
}
/** [src_std_i2c_slave] */

/* end of file */
