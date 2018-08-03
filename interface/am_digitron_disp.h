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
 * \brief 支持多实例的段式数码管驱动接口
 *
 * 使用本服务需要包含头文件 am_digitron_disp.h
 *
 * \par 使用示例
 * \code
 * #include "am_digitron_disp.h"
 *
 * // 初始化，设置8段ASCII解码
 * am_digitron_disp_decode_set(0, am_digitron_seg8_ascii_decode);
 *
 * am_digitron_disp_blink_set(0, 0, AM_TRUE);  // 设置第0个数码管闪烁
 * am_digitron_disp_at(0, 0, 0x54);            // 第0个数码管显示自定义形状0x54
 * am_digitron_disp_char_at(0, 1, 'A');        // 第1个数码管显示字符'A'
 * am_digitron_disp_clr(0);                    // 清除所有数码管显示（黑屏）
 * am_digitron_disp_str(0, 0, 4, "0123");      // 数码管显示字符串"0123"
 * \endcode
 *
 * \internal
 * \par modification history
 * - 1.00 17-05-18  tee, first implementation.
 * \endinternal
 */

#ifndef __AM_DIGITRON_DISP_H
#define __AM_DIGITRON_DISP_H

#include "am_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \addtogroup am_if_digitron_disp
 * \copydoc am_digitron_disp.h
 * @{
 */

/**
 * \brief 通用的8段数码管ASCII字符解码函数
 * \param[in] ascii_char ： 要解码的ASCII字符
 * \return ASCII字符对应的数码管字形数据, 如果无法解码，返回空白码（0x00）。
 *
 * \note
 * 该函数一般作为8段数码管的默认解码函数，如果用户需要使用自定义的解码函数对字符
 * 进行解码，可以使用 am_digitron_disp_set_decode() 设置自定义的解码函数。
 *
 * 该函数支持的ASCII字符有：
 *  |  A  |  S  |  C  |  I  |  I  |
 *  |-----|-----|-----|-----|-----|
 *  | '0' | '1' | '2' | '3' | '4' |
 *  | '5' | '6' | '7' | '8' | '9' |
 *  | 'A' | 'B' | 'C' | 'D' | 'E' |
 *  | 'F' | 'H' | 'I' | 'J' | 'L' |
 *  | 'N' | 'O' | 'P' | 'R' | 'S' |
 *  | 'a' | 'b' | 'c' | 'd' | 'e' |
 *  | 'f' | 'h' | 'i' | 'j' | 'l' |
 *  | 'n' | 'o' | 'p' | 'r' | 's' |
 *  | '-' | '.' | ' ' |  -  |  -  |
 *
 */
uint16_t am_digitron_seg8_ascii_decode (uint16_t ascii_char);

/**
 * \brief 设置数码管显示驱动的段码解码接口
 *
 * \param[in] id         : 显示器编号
 * \param[in] pfn_decode : 解码函数
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_ENODEV : 显示器编号有误，无此显示器
 */
int am_digitron_disp_decode_set (int        id,
                                 uint16_t (*pfn_decode) (uint16_t ch));

/**
 * \brief 设置数码管显示闪烁属性
 *
 * \param[in] id    : 显示器编号
 * \param[in] index : 数码管索引号
 * \param[in] blink : 闪烁属性，TURE：闪烁，FALSE：禁止闪烁。
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_ENODEV : 显示器编号有误，无此显示器
 */
int am_digitron_disp_blink_set (int id, int index, am_bool_t blink);

/**
 * \brief 直接向数码管写入段码显示数据。本函数一般用于显示自定义的特殊符号或形状
 *
 * \param[in] id    : 显示器编号
 * \param[in] index : 数码管索引号
 * \param[in] seg   : 显示的“段”码（高电平有效，位值为1时，相应的段点亮）
 *
 * \retval AM_OK      : 写入成功
 * \retval -AM_ENODEV : 显示器编号有误，无此显示器
 */
int am_digitron_disp_at (int id, int index, uint16_t seg);

/**
 * \brief 在指定编号的数码管上显示ASCII字符
 *
 * \param[in] id    : 显示器编号
 * \param[in] index : 数码管索引号
 * \param[in] ch    : 要显示的ASCII字符
 *
 * \retval AM_OK      : 设置成功
 * \retval -AM_ENODEV : 显示器编号有误，无此显示器
 *
 * \note 能显示的字符由解码函数确定
 */
int am_digitron_disp_char_at (int id, int index, const char ch);

/**
 * \brief 在数码管上显示ASCII字符串
 *
 * \param[in] id    : 显示器编号
 * \param[in] index : 数码管索引号
 * \param[in] len   : 显示长度
 * \param[in] p_str : 要显示的ASCII字符串
 *
 * \retval AM_OK      : 显示成功
 * \retval -AM_ENODEV : 显示器编号有误，无此显示器
 *
 * \note 能显示的字符由解码函数确定
 */
int am_digitron_disp_str (int id, int index, int len, const char *p_str);

/**
 * \brief 清除所有数码管显示
 *
 * \param[in] id  : 显示器编号
 *
 * \retval AM_OK      : 清屏成功
 * \retval -AM_ENODEV : 显示器编号有误，无此显示器
 */
int am_digitron_disp_clr (int id);

/**
 * \brief 使能数码管显示，开启扫描（默认状态是开启的）
 *
 * \param[in] id  : 显示器编号
 *
 * \retval AM_OK      : 使能成功
 * \retval -AM_ENODEV : 显示器编号有误，无此显示器
 */
int am_digitron_disp_enable (int id);

/**
 * \brief 禁能数码管显示,停止扫描
 *
 * \param[in] id  : 显示器编号
 *
 * \retval AM_OK      : 禁能成功
 * \retval -AM_ENODEV : 显示器编号有误，无此显示器
 */
int am_digitron_disp_disable (int id);

/** @} am_if_digitron_disp */

#ifdef __cplusplus
}
#endif

#endif /* __AM_DIGITRON_DISP_H */

/* end of file */
