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
 * \brief 时间标准服务
 *
 * 使用本服务需要包含头文件 am_time.h
 *
 * \par 使用示例
 * \code
 * \endcode
 *
 * \internal
 * \par modification history
 * - 1.00 13-09-06  zen, first implementation
 * \endinternal
 */

#ifndef __AM_TIME_H
#define __AM_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ametal.h"
#include "time.h"

/**
 * \addtogroup am_if_time
 * \copydoc am_time.h
 * @{
 */

/** \brief 日历时间，自1970 年 1 月 1 日 0 时 0 分 0 秒 以来的秒数 */
typedef time_t am_time_t;

/** \brief 精确日历时间，用秒和纳秒表示的日历时间 */
typedef struct am_timespec {
    am_time_t       tv_sec;   /**< \brief 秒计数   */
    unsigned long   tv_nsec;  /**< \brief 纳秒计数 */
} am_timespec_t;

/**
 * \brief 细分时间, 本结构体与 <time.h> 中的 struct tm 相似, 可使用 <time.h> 中
 * 的辅助函数进行运算 (使用强制转换 (struct tm *)xx )
 */
typedef struct am_tm {
    int tm_sec;     /**< \brief seconds after the minute     - [0, 59]  */
    int tm_min;     /**< \brief minutes after the hour       - [0, 59]  */
    int tm_hour;    /**< \brief hours after midnight         - [0, 23]  */
    int tm_mday;    /**< \brief day of the month             - [1, 31]  */
    int tm_mon;     /**< \brief months since January         - [0, 11]  */
    int tm_year;    /**< \brief years since 1900                        */
    int tm_wday;    /**< \brief days since Sunday            - [0, 6]   */
    int tm_yday;    /**< \brief days since January 1         - [0, 365] */
    int tm_isdst;   /**< \brief Daylight Saving Time flag               */
} am_tm_t;

/**
 * \brief 获取当前日历时间 (与 <time.h> 中 time() 行为相同)
 *
 * \note 可使用 am_time_to_tm() 将日历时间转换为细分时间
 *
 * \param[out] p_time   若不为NULL, 则返回日历时间
 *
 * \return 当前日历时间
 *
 * \par 示例
 * \code
 * #include "am_time.h"
 *
 * am_time_t time;      // 日历时间
 * am_tm_t   tm;        // 细分时间
 *
 * time = am_time(NULL);                            // 得到日历时间
 * printf("%s\n", ctime(const time_t *)&time);      // 打印日历时间
 *
 * am_time_to_tm(time, &tm);                        // 将日历时间转换为细分时间
 * printf("%s\n", asctime(const struct tm *)&tm);   // 打印细分时间
 * \endcode
 */
am_time_t am_time(am_time_t *p_time);

/**
 * \brief 获取当前日历时间 (秒、纳秒表示)
 *
 * 相比 am_time(), am_tm_get(), 本函数能多获取一个纳秒值。
 *
 * \note 可使用 am_tm_get() 直接获取当前时间的细分表示形式
 *
 * \param[out] p_tv  时间值
 *
 * \retval AM_OK    成功
 * \retval -AM_EINVAL  参数错误
 *
 * \par 示例
 * \code
 * #include "am_time.h"
 *
 * am_timespec_t timespec;      // 日历时间
 *
 * am_timespec_get(&timespec);                            // 得到日历时间
 * printf("%s", ctime((const time_t *)&timespec.tv_sec)); // 打印日历时间
 * printf(" %d ms \n", timespec.tv_nsec / 1000000);       // 打印毫秒
 * \endcode
 * \sa am_time() am_timespec_set()
 */
int am_timespec_get(am_timespec_t *p_tv);

/**
 * \brief 设置当前日历时间
 *
 * \note 可使用 am_tm_set() 以细分表示形式来设置当前时间
 *
 * \param[in] p_tv 要设置的时间
 *
 * \retval AM_OK    成功
 * \retval -AM_EINVAL  参数错误
 *
 * \par 示例
 * \code
 * #include "am_time.h"
 *
 * am_tm_t       tm;            // 细分时间
 * am_timespec_t timespec;      // 日历时间
 *
 * tm.tm_sec    = 6;
 * tm.tm_min    = 6;
 * tm.tm_hour   = 6;
 * tm.tm_mday   = 6;
 * tm.tm_mon    = 6 - 1;        // 实际月份要减1
 * tm.tm_year   = 2013 - 1900;  // 距1900的年数
 * tm.tm_isdst  = -1;           // 夏令时不可用
 *
 * am_tm_to_time(&tm, &timespec.tv_sec);    // 转换为日历时间
 * timespec.tv_nsec = 0;                    // 纳秒部分置0
 *
 * am_timespec_set(&timespec);              // 设置日历时间
 *
 * \endcode

 * \sa am_time() am_timespec_Get()
 */
int am_timespec_set(am_timespec_t *p_tv);

/**
 * \brief 获取当前时间 (细分形式表示)
 *
 * \note 可使用 am_tm_get() 直接获取当前时间的细分表示形式
 *
 * \param[out] p_tm  时间值
 *
 * \retval AM_OK    成功
 * \retval -AM_EINVAL  参数错误
 *
 * \par 示例
 * \code
 * #include "am_time.h"
 * am_tm_t tm;          // 细分时间
 *
 * am_tm_get(&tm);      // 得到当前时间
 * printf("%s", asctime((const struct tm *)&tm)); // 打印时间
 *
 * printf("%d-%d-%d %d:%d:%d wday:%d yday:%d\n",
 *        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
 *        tm.tm_hour, tm.tm_min, tm.tm_sec,
 *        tm.tm_wday, tm.tm_yday);              // 打印时间
 *
 * \endcode
 * \sa am_time() am_tm_set()
 */
int am_tm_get (am_tm_t *p_tm);

/**
 * \brief 设置当前时间(细分形式表示)
 *
 * \param[in] p_tm 要设置的时间
 *
 * \retval AM_OK    成功
 * \retval -AM_EINVAL  参数错误
 *
 * \par 示例
 * \code
 * #include "am_time.h"
 *
 * am_tm_t       tm;            // 细分时间
 *
 * tm.tm_sec    = 6;
 * tm.tm_min    = 6;
 * tm.tm_hour   = 6;
 * tm.tm_mday   = 6;
 * tm.tm_mon    = 6 - 1;        // 实际月份要减1
 * tm.tm_year   = 2013 - 1900;  // 距1900的年数
 * tm.tm_isdst  = -1;           // 夏令时不可用
 *
 * am_tm_set(&tm);              // 设置日历时间
 *
 * \endcode
 *
 * \sa am_time() am_timespec_Get()
 */
int am_tm_set (am_tm_t *p_tm);

/**
 * \brief 将细分时间形式转换为日历时间形式
 *
 * \param[in,out] p_tm    细分时间, 成员 tm_wday, tm_yday 在调用后被更新;
 *                        如夏令时有效, tm_isdst 为正, 如无效则为0, 若信息不可用
 *                        则为负
 * \param[out]    p_time  日历时间
 *
 * \retval AM_OK       成功
 * \retval -AM_EINVAL  参数错误
 * \retval -EBADF      细分时间错误, 细分时间的有效值请参考 am_tm_t
 */
int am_tm_to_time (am_tm_t *p_tm, am_time_t *p_time);

/**
 * \brief 将日历时间形式转换为细分时间形式
 *
 * \attention 本函数不可重入
 *
 * \param[in]   p_time  日历时间
 * \param[out]  p_tm    细分时间
 *
 * \retval AM_OK       成功
 * \retval -AM_EINVAL  参数错误
 */
int am_time_to_tm (am_time_t *p_time, am_tm_t *p_tm);

/**
 * \brief 系统时间初始化
 *
 * \param[in] rtc_handle        : 依赖的RTC实时时钟句柄
 * \param[in] update_sysclk_ns  : 使用系统时钟更新的时间间隔（单位：纳秒）
 * \param[in] update_rtc_s      : 使用RTC更新的时间间隔（单位：纳秒）
 *
 * \retval AM_OK       成功
 * \retval -AM_EINVAL  参数错误
 */
int am_time_init (void           *rtc_handle,
                  unsigned int    update_sysclk_ns,
                  unsigned int    update_rtc_s);

/**
 * \brief 根据RTC更新系统时间
 */
int am_time_update (void);

/** @} am_if_time */

#ifdef __cplusplus
}
#endif

#endif /* __AM_TIME_H */

/* end of file */

