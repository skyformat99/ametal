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
 * \brief 含有子地址的I2C从机标准接口
 *
 *
 * \internal
 * \par Modification History
 * - 1.00 17-09-19  vir, first implementation.
 * \endinternal
 */

#ifndef __AM_I2C_SLV_SUBADDR_H
#define __AM_I2C_SLV_SUBADDR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "am_common.h"
#include "am_i2c_slv.h"

/**
 * @addtogroup am_if_i2c_slv_subaddr
 * @copydoc am_i2c_slv_subaddr.h
 * @{
 */

struct am_i2c_slv_subaddr_device;

/**
 * \brief 含有子地址设备的回调函数 ，用于分析
 */
typedef struct am_i2c_slv_subaddr_cb_funcs {

    /** \brief 从机地址匹配时回调函数 */
    void (*pfn_addr_match)( void       *p_arg,
                            am_bool_t   is_rx);

    /**
     * \brief 传输完成回调函数
     *         is_rx:   为AM_TRUE时表示 从机接受数据完成 , 为 AM_FALSE时表示从机发送数据完成
     *         subaddr： 主机给从机发送的子地址 ，即相对于 缓存区的索引号 (与寄存器位宽无关 )
     *                  eg: 从机缓存区为 ：int16_t buf[5]，subaddr=2时 ，即主机向从机的buf[2]开始操作
     *         size： 主机向从机 读或写了多少字节
     *
     */
    void (*pfn_tran_finish)(void        *p_arg,
                            am_bool_t    is_rx,
                            uint32_t     subaddr,
                            uint32_t     size);

    /** \brief 广播回调函数   */
    int (*pfn_gen_call)(void *p_arg, uint8_t byte);

}am_i2c_slv_subaddr_cb_funcs_t;

/**
 * \name 从设备寄存器空间大小
 * \anchor grp_i2c_slv_reg_mode
 * @{
 */

#define AM_I2C_SLV_REG_8_BIT           0X01u            /**< \brief 每个子地址对应的寄存器空间大小为8位 ( 默认) */
#define AM_I2C_SLV_REG_16_BIT          0X02u            /**< \brief 每个子地址对应的寄存器空间大小为16位  */
#define AM_I2C_SLV_REG_32_BIT          0X04u            /**< \brief 每个子地址对应的寄存器空间大小为32位  */
#define AM_I2C_SLV_REG_IRREGULAR       0X08u            /**< \brief 子地址对应的寄存器空间大小不规则  */

/** @} */

/**
 * \brief 含有子地址的寄存器信息结构体
 *
 * \note 以下注释的 1 表示 ：每个子地址对应的寄存器内存空间 不相同 或者
 *                     某个子地址对应的寄存器空间 大小不为1、2、4字节 (即不是8位、16位、32位)
 *
 *              2 表示 ： 每个子地址对应的寄存器空间大小都为1字节 或2字节 或4字节 (即8位、16位、32位)
 *                     默认所有寄存器是连续的
 *
 * \par 范例
 * \code
 * #include "am_i2c_slv_subaddr.h"
 *
 *   //定义 1 类 的从机设备寄存器
 *    reg_mode = AM_I2C_SLV_REG_IRREGULAR
 *
 *   static uint8_t   reg1[5];    //子地址为0
 *   static uint16_t  reg2[10];   //子地址为1
 *   static uint32_t  reg3[1];    //子地址 位2
 *   .....
 *   static uint32_t  reg3_wr[1] = {0XFFFF0000}; //寄存器3的读写保护标志
 *
 *   am_i2c_slv_subaddr_reg_info_t   i2c_slv_reg[] =
 *   {
 *      { sizeof(reg1), reg1, NULL },
 *      { sizeof(reg2), reg2, NULL },
 *      { sizeof(reg3), reg3, reg3_wr},
 *      ....
 *   }
 *
 *   //定义2 类的从机设备寄存器
 *   reg_mode = AM_I2C_SLV_REG_8_BIT   或
 *   reg_mode = AM_I2C_SLV_REG_16_BIT  或
 *   reg_mode = AM_I2C_SLV_REG_32_BIT
 *
 *   static uint16_t  reg[20];  //子地址就是数组的索引号
 *  am_i2c_slv_subaddr_reg_info_t i2c_slv_reg[1] =
 *  {
 *      { sizeof(reg), reg,  NULL },
 *  }
 *
 * \endcode
 *
 */
typedef struct am_i2c_slv_subaddr_reg_info {

    /**
     *  \brief 1 某个子地址对应寄存器空间的长度(字节)
     *         2 从设备所有寄存器的总长度 (字节)
     */
    uint32_t  reg_len;

    /**
     *  \brief 1 某个子地址 对应寄存器空间的首地址
     *         2 从设备寄存器的首地址
     */
    uint8_t  *p_reg_addr;

    /**
     * \brief  1 某个子地址 对应寄存器空间的的读写保护标志(比特位值为  0：可读可写    1：只可读，不可写)
     *            若不需要赋值为NULL，即默认可读可写
     *         2 从设备寄存器的读写保护标志
     */
    uint8_t  *p_reg_wr;
}am_i2c_slv_subaddr_reg_info_t;



/**
 * \brief 含有子地址的I2C从机设备结构体
 */
typedef struct am_i2c_slv_subaddr_device {
    uint32_t   subaddr ;               /**< \brief 主机发给给从机的子地址 */
    uint32_t   off_byte ;              /**< \brief 主机要求操作的数据段相对于设备寄存器缓存首地址的偏移  */
    am_bool_t  is_subaddr;             /**< \brief 判断当前接受的是否是子地址 */
    am_bool_t  is_rx;                  /**< \brief 判断当前从机是发送还是接受数据  */
    uint8_t    reg_bit_width;          /**< \brief 从机寄存器位宽 ,是8位、16位或者32位 */
    uint8_t    subaddr_count_flag;     /**< \brief 接受子地址个数标识  */
    uint32_t    data_count;            /**< \brief 接受或发送字节统计 (除去子地址的字节数)*/
    uint16_t   subaddr_dev_flags;      /**< \brief 子地址设备标识  */
    am_bool_t  reg_mode;               /**< \brief 一个子地址对应内存空间大小的模式 (8位,16位,32位或者不规则) */
    uint32_t   reg_num;                /**< \brief 从机寄存器个数 */

    am_i2c_slv_subaddr_reg_info_t     *reg_info;            /**< \brief 整个设备寄存器内存的首地址 */
    am_i2c_slv_subaddr_reg_info_t     *cur_reg_info;        /**< \brief 当前处理子地址对应的信息 */


    am_i2c_slv_subaddr_cb_funcs_t *p_cb_funs;    /**< \brief 含有子地址的回调函数  */
    void                          *p_arg;        /**< \brief 回调函数参数  */

    am_i2c_slv_device_t            i2c_slv_dev;  /**< \brief I2C从机设备 */
}am_i2c_slv_subaddr_device_t;

/**
 * \brief 含有子地址的设备参数设置
 *
 * \param[in] p_dev                 : 指向含有子地址从机设备描述结构体的指针
 * \param[in] handle                : 与从设备关联的I2C标准服务操作句柄
 * \param[in] p_cb_funs             : 用于用户分析的回调函数
 * \param[in] p_arg                 : 回调函数参数
 * \param[in] dev_addr              : 从机设备地址
 * \param[in] dev_flags             : 从机设备特性，如：设备地址、子地址宽度 等， 见“I2C从设备属性标志”，在am_i2c_slv.h中
 * \param[in] p_reg_info            : 从设备寄存器的 信息 ，
 * \param[in] reg_num               : 从设备寄存器的 个数 ，规则的寄存器空间大小可不用这个参数，赋值为1即可
 * \param[in] reg_mode              : 从设备寄存器的模式  ，值参考 ：\ref grp_i2c_slv_reg_mode
 *
 * \return 无
 *
 * \par 范例
 * \code
 * #include "am_i2c_slv_subaddr.h"
 *
 *   static uint8_t   reg1[5];    //子地址为0
 *   static uint16_t  reg2[10];   //子地址为1
 *   static uint32_t  reg3[1];    //子地址 位2
 *
 *   static uint32_t  reg3_wr[1] = {0XFFFF0000}; //寄存器3的读写保护标志 ，高16位只读不可写
 *
 *   am_i2c_slv_subaddr_reg_info_t   i2c_slv_reg[3] =
 *   {
 *      { sizeof(reg1), reg1, NULL },
 *      { sizeof(reg2), reg2, NULL },
 *      { sizeof(reg3), reg3, reg3_wr},
 *   }
 *
 *   am_i2c_slv_subaddr_device_t  i2c_slv_dev;
 *   uint32_t reg_num = sizeof(i2c_slv_reg) / sizeof(am_i2c_slv_subaddr_reg_info_t );
 *
 *   am_i2c_slv_subaddr_mkdev( &i2c_slv_dev,
 *                              i2c_slv_handle,
 *                             &__g__cb_funs,
 *                             &i2c_slv_dev
 *                              0x55,
 *                              AM_I2C_SLV_ADDR_7BIT|AM_I2C_SLV_SUBADDR_1BYTE,
 *                              &i2c_slv_reg[0],
 *                              reg_num,
 *                              AM_I2C_SLV_REG_IRREGULAR);
 * \endcode
 *
 * \note 生成 含有子地址的设备时，必须调用  am_i2c_slv_setup 函数 开启从机运行
 *       同时可调用 am_i2c_slv_shutdown 关闭该设备
 */
void am_i2c_slv_subaddr_mkdev( am_i2c_slv_subaddr_device_t   *p_dev,
                               am_i2c_slv_handle_t            handle,
                               am_i2c_slv_subaddr_cb_funcs_t *p_cb_funs,
                               void                          *p_arg,
                               uint16_t                       dev_addr,
                               uint16_t                       dev_flags,
                               am_i2c_slv_subaddr_reg_info_t *p_reg_info,
                               uint32_t                       reg_num,
                               uint8_t                        reg_mode);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_I2C_SLV_SUBADDR_H */

/* end of file */
