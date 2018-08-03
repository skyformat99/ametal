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
 * \brief 数码管显示，60s 计时器应用
 * 
 * 本应用需要使用到两位数码管，指定的数码管显示器必须至少包含两位数码管
 *
 * - 操作步骤
 *   1. 调用入口函数，传入指定的数码管显示器编号
 *
 * - 实验现象：
 *   1. 可以看到数值 0 ~ 59 的秒计数器；
 *   2. 数组小于30时，个位闪烁，大于30时十位闪烁。
 *
 * \par 源代码
 * \snippet demo_std_digitron_60s_counting.c src_std_digitron_60s_counting
 *
 * \internal
 * \par Modification history
 * - 1.00 15-07-21  tee, first implementation
 * \endinternal
 */

/**
 * \addtogroup demo_if_std_digitron_60s_counting
 * \copydoc demo_std_digitron_60s_counting.c
 */

/** [src_std_digitron_60s_counting] */
#include "ametal.h"
#include "am_delay.h"
#include "am_vdebug.h"
#include "am_digitron_disp.h"

/**
 * \brief 例程入口
 */
void demo_std_digitron_60s_counting_entry (int32_t id)
{
    int i   = 0;
    int num = 0;

    /* 初始化，设置8段ASCII解码 */
    am_digitron_disp_decode_set(id, am_digitron_seg8_ascii_decode);

    am_digitron_disp_char_at(id, 0, '0');        /* 十位数码管显示字符0 */
    am_digitron_disp_char_at(id, 1, '0');        /* 个位数码管显示字符0 */

    /* 初始数值小于30，个位闪烁 */
    am_digitron_disp_blink_set(id, 1, AM_TRUE);

    while(1) {
       for (i = 0; i < 60; i++) {
             am_mdelay(1000);

             /* 数值加1， 0 ~ 59循环 */
             num = (num + 1) % 60;

             /* 更新个位和十位显示 */
             am_digitron_disp_char_at(id, 0, '0' + num / 10 );
             am_digitron_disp_char_at(id, 1, '0' + num % 10);

             if (num < 30) {
                 /* 小于30，个位闪烁，十位不闪烁 */
                 am_digitron_disp_blink_set(id, 0, AM_FALSE);
                 am_digitron_disp_blink_set(id, 1, AM_TRUE);
             } else {

                 /* 大于30，十位闪烁，个位不闪烁 */
                 am_digitron_disp_blink_set(id, 0, AM_TRUE);
                 am_digitron_disp_blink_set(id, 1, AM_FALSE);
            }
        }
    }
}
/** [src_std_digitron_60s_counting] */

/* end of file */
