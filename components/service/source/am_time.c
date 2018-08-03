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
 * \brief system time manager library implementation
 *
 * \internal
 * \par modification history:
 * - 1.00 13-09-04  zen, first implementation
 * \endinternal
 */
#include "ametal.h"
#include "am_time.h"
#include "am_softimer.h"
#include <string.h>
#include "am_rtc.h"
#include "time.h"
#include "am_int.h"
#include "am_vdebug.h"

/******************************************************************************/

/* real time clock */
struct __real_clock {
    am_softimer_t     timer;
    am_timespec_t     time;

    unsigned long     ns_add;
    am_rtc_handle_t   rtc_handle;
};

/* real time clock instance */
struct __real_clock   __g_real_clock;

/******************************************************************************/
static void __clock_tick (void *arg)
{
    struct __real_clock *p_clock = (struct __real_clock *)arg;

    int key;

    key = am_int_cpu_lock();

    p_clock->time.tv_nsec += p_clock->ns_add;
    if (p_clock->time.tv_nsec >= 1000000000) {
        p_clock->time.tv_nsec -= 1000000000;
        p_clock->time.tv_sec  += 1;
    }
    am_int_cpu_unlock(key);
}

/******************************************************************************/
static int __timespec_get (am_timespec_t *p_timespec)
{
    struct __real_clock *p_clock = &__g_real_clock;

    int key;

    key = am_int_cpu_lock();

    p_timespec->tv_sec  = p_clock->time.tv_sec;
    p_timespec->tv_nsec = p_clock->time.tv_nsec;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
static int __timespec_set (am_timespec_t *p_timespec)
{
    struct __real_clock *p_clock = &__g_real_clock;

    int key;

    key = am_int_cpu_lock();

    p_clock->time.tv_nsec = p_timespec->tv_nsec;
    p_clock->time.tv_sec  = p_timespec->tv_sec;

    am_int_cpu_unlock(key);

    return AM_OK;
}

/******************************************************************************/
am_time_t am_time (am_time_t *p_time)
{
    am_timespec_t timespec;

    /* get time from real time clock */
    __timespec_get(&timespec);

    if (p_time) {
        *p_time = timespec.tv_sec;
    }

    return timespec.tv_sec;
}

/******************************************************************************/
int am_timespec_get (am_timespec_t *p_timespec)
{
    if (p_timespec == NULL) {
        return -AM_EINVAL;
    }

    /* get time from real time clock */
    return __timespec_get(p_timespec);
}

/******************************************************************************/
int am_timespec_set (am_timespec_t *p_timespec)
{
    struct __real_clock *p_clock = &__g_real_clock;
    am_tm_t tm;

    if (p_timespec == NULL) {
        return -AM_EINVAL;
    }

    /* set time to real time clock */
    __timespec_set(p_timespec);

    /* set time to RTC */
    am_time_to_tm(&p_timespec->tv_sec, &tm);
    am_rtc_time_set(p_clock->rtc_handle, &tm);

    return AM_OK;
}

/******************************************************************************/
int am_tm_set (am_tm_t *p_tm)
{
    struct __real_clock *p_clock = &__g_real_clock;
    am_timespec_t time;

    if (p_tm == NULL) {
        return -AM_EINVAL;
    }

    /* make time */
    time.tv_sec  = mktime((struct tm *)p_tm);
    time.tv_nsec = 0;
    if (time.tv_sec == (am_time_t)-1) {
        return -AM_EBADF;
    }

    /* set time to real time clock */
    (void)__timespec_set(&time);

    /* set time to RTC */
    if (am_rtc_time_set(p_clock->rtc_handle, p_tm) != AM_OK) {
        AM_ERRF(("[am_time]:Save time to RTC failed!\n"));
    }

    return AM_OK;
}

/******************************************************************************/
int am_tm_get (am_tm_t *p_tm)
{
    am_timespec_t time;

    if (p_tm == NULL) {
        return -AM_EINVAL;
    }

    /* get time from real time clock */
    (void)__timespec_get(&time);

    /* transfer to pm type */
    return am_time_to_tm(&time.tv_sec, p_tm);
}

/******************************************************************************/
int am_tm_to_time (am_tm_t *p_tm, am_time_t *p_time)
{
    if ((p_tm == NULL) ||
        (p_time == NULL)) {

        return -AM_EINVAL;
    }

    if ((*p_time = mktime((struct tm *)p_tm)) == (am_time_t)-1) {
        return -AM_EBADF;
    }
    return AM_OK;
}

/******************************************************************************/
int am_time_to_tm (am_time_t *p_time, am_tm_t *p_tm)
{
    struct tm *p_tm_tmp = NULL;

    if ((p_tm == NULL) ||
        (p_time == NULL)) {

        return -AM_EINVAL;
    }

    p_tm_tmp = localtime((const time_t *)p_time);
    if (p_tm_tmp) {
        memcpy(p_tm, p_tm_tmp, sizeof(*p_tm));
    }

    return AM_OK;
}

/******************************************************************************/
int am_time_update (void)
{
    am_tm_t              tm;
    struct __real_clock *p_clock = &__g_real_clock;

    if (am_rtc_time_get(p_clock->rtc_handle, &tm) == AM_OK) {
        am_timespec_t timespec;
        am_tm_to_time(&tm, &timespec.tv_sec);
        timespec.tv_nsec = 0;

        AM_DBG_INFO("Update the time!\n");

        /* set time */
        __timespec_set(&timespec);

    } else {
        AM_DBG_INFO("[am_time]:Restore time from RTC failed!\n");
        return AM_ERROR;
    }

    return AM_OK;
}

/******************************************************************************/
int am_time_init (void           *rtc_handle,
                  unsigned int    update_sysclk_ns,
                  unsigned int    update_rtc_s)
{
    struct __real_clock *p_clock = &__g_real_clock;
    am_tm_t              tm;
    int                  ticks;


    if ((update_sysclk_ns == 0) ||
        (update_rtc_s     == 0) ||
        (rtc_handle       == NULL)) {

        return -AM_EINVAL;
    }

    memset(p_clock, 0, sizeof(*p_clock));

    /* todo: calculate the timer ticks, now, the unit is fixed to ms */
    ticks = update_sysclk_ns / 1000000;

    if (ticks == 0) {
        ticks = 1;
    }

    /* calculate the ns increase every tick */
    p_clock->ns_add = ticks * 1000000;

    /* restore time from RTC */
    p_clock->rtc_handle = rtc_handle;

    if (am_rtc_time_get(rtc_handle, &tm) == AM_OK) {

        am_timespec_t timespec;

        if (am_tm_to_time(&tm, &timespec.tv_sec) != AM_OK) {

            /* 时间有误，标准时间要求，时间必须在1970年之后，设置时间为：1970年1月1日  */
            tm.tm_year = 1970 - 1900;
            tm.tm_mon  = 0;
            tm.tm_mday = 1;

            am_rtc_time_set(rtc_handle, &tm);
        }

        am_tm_to_time(&tm, &timespec.tv_sec);
        timespec.tv_nsec = 0;

        AM_DBG_INFO("current time: %d-%02d-%02d %02d:%02d:%02d\n",
                    tm.tm_year+1900,
                    tm.tm_mon+1,
                    tm.tm_mday,
                    tm.tm_hour,
                    tm.tm_min,
                    tm.tm_sec);

        /* set time */
        __timespec_set(&timespec);

    } else {
        AM_DBG_INFO("[am_time]:Restore time from RTC failed!\n");
    }

    /* TODO: set C library's time */

    /* start up the timer */
    am_softimer_init(&p_clock->timer, __clock_tick, p_clock);
    am_softimer_start(&p_clock->timer, ticks);

    return AM_OK;
}

/* end of file */

