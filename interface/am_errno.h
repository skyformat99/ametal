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
 * \brief 错误号相关定义头文件
 *
 * \internal
 * \par Modification history
 * - 1.00 14-11-01  zen, first implementation.
 * \endinternal
 */

#ifndef __AM_ERRNO_H
#define __AM_ERRNO_H
 
#ifdef __cplusplus
extern "C" {
#endif


/**
 * \addtogroup am_if_errno
 * \copydoc am_errno.h
 * @{
 */
 
/**
 * \name 错误编码数据类型
 * @{
 */
typedef int error_t;    /* 兼容POSIX错误类型 */
typedef int am_err_t;   /* Apollo错误类型 */
/** @} */

/**
 * \name POSIX错误值
 * @{
 */
#define AM_EPERM           1       /**< \brief 操作不允许          */
#define AM_ENOENT          2       /**< \brief 文件或目录不存在    */
#define AM_ESRCH           3       /**< \brief 进程不存在          */
#define AM_EINTR           4       /**< \brief 调用被中断          */
#define AM_EIO             5       /**< \brief I/O 错误            */
#define AM_ENXIO           6       /**< \brief 设备或地址不存在    */
#define AM_E2BIG           7       /**< \brief 参数列表太长        */
#define AM_ENOEXEC         8       /**< \brief 可执行文件格式错误  */
#define AM_EBADF           9       /**< \brief 文件描述符损坏      */
#define AM_ECHILD          10      /**< \brief 没有子进程          */
#define AM_EAGAIN          11      /**< \brief 资源不可用，需重试  */
#define AM_ENOMEM          12      /**< \brief 空间（内存）不足    */
#define AM_EACCES          13      /**< \brief 权限不够            */
#define AM_EFAULT          14      /**< \brief 地址错误            */
#define AM_ENOTEMPTY       15      /**< \brief 目录非空            */
#define AM_EBUSY           16      /**< \brief 设备或资源忙        */
#define AM_EEXIST          17      /**< \brief 文件已经存在        */
#define AM_EXDEV           18      /**< \brief 跨设备连接          */
#define AM_ENODEV          19      /**< \brief 设备不存在          */
#define AM_ENOTDIR         20      /**< \brief 不是目录            */
#define AM_EISDIR          21      /**< \brief 是目录              */
#define AM_EINVAL          22      /**< \brief 无效参数            */
#define AM_ENFILE          23      /**< \brief 系统打开文件太多，描述符不够用 */
#define AM_EMFILE          24      /**< \brief 打开的文件太多      */
#define AM_ENOTTY          25      /**< \brief 不合适的I/O控制操作 */
#define AM_ENAMETOOLONG    26      /**< \brief 文件名太长          */
#define AM_EFBIG           27      /**< \brief 文件太大            */
#define AM_ENOSPC          28      /**< \brief 设备剩余空间不足    */
#define AM_ESPIPE          29      /**< \brief 无效的搜索（Invalid seek） */
#define AM_EROFS           30      /**< \brief 文件系统只读        */
#define AM_EMLINK          31      /**< \brief 链接太多            */
#define AM_EPIPE           32      /**< \brief 损坏的管道          */
#define AM_EDEADLK         33      /**< \brief 资源可能死锁        */
#define AM_ENOLCK          34      /**< \brief 无可用（空闲）的锁  */
#define AM_ENOTSUP         35      /**< \brief 不支持              */
#define AM_EMSGSIZE        36      /**< \brief 消息太大            */

/** @} */

/**
 * \name ANSI错误值
 * @{
 */

#define AM_EDOM            37      /**< \brief 数学函数参数超出范围 */
#define AM_ERANGE          38      /**< \brief 数学函数计算结果太大 */
#define AM_EILSEQ          39      /**< \brief 非法的字节顺序       */
 
/** @} */

/**
 * \name 网络参数错误相关
 * @{
 */

#define AM_EDESTADDRREQ    40      /**< \brief 需要目标地址         */
#define AM_EPROTOTYPE      41      /**< \brief socket协议类型错误   */
#define AM_ENOPROTOOPT     42      /**< \brief 协议不可用           */
#define AM_EPROTONOSUPPORT 43      /**< \brief 协议不支持           */
#define AM_ESOCKTNOSUPPORT 44      /**< \brief Socket类型不支持     */
#define AM_EOPNOTSUPP      45      /**< \brief socket不支持该操作   */
#define AM_EPFNOSUPPORT    46      /**< \brief 协议族不支持         */
#define AM_EAFNOSUPPORT    47      /**< \brief 地址簇不支持         */
#define AM_EADDRINUSE      48      /**< \brief 地址已经被占用       */
#define AM_EADDRNOTAVAIL   49      /**< \brief 地址不可用           */
#define AM_ENOTSOCK        50      /**< \brief 被操作对象不是socket */

/** @} */

/**
 * \name 可选择实现的错误值
 * @{
 */

#define AM_ENETUNREACH     51        /**< \brief 网络不可达         */
#define AM_ENETRESET       52        /**< \brief 网络中断了连接     */
#define AM_ECONNABORTED    53        /**< \brief 连接中断           */
#define AM_ECONNRESET      54        /**< \brief 连接复位           */
#define AM_ENOBUFS         55        /**< \brief 缓冲空间不足       */
#define AM_EISCONN         56        /**< \brief Socket已经连接     */
#define AM_ENOTCONN        57        /**< \brief Socket没有连接     */
#define AM_ESHUTDOWN       58        /**< \brief Socket已经关闭，不能发送数据 */
#define AM_ETOOMANYREFS    59        /**< \brief 引用太多，无法拼接 */
#define AM_ETIMEDOUT       60        /**< \brief 连接超时           */
#define AM_ECONNREFUSED    61        /**< \brief 连接被拒绝         */
#define AM_ENETDOWN        62        /**< \brief 网络已经停止       */
#define AM_ETXTBSY         63        /**< \brief 文本文件忙         */
#define AM_ELOOP           64        /**< \brief 符号链接级数太多   */
#define AM_EHOSTUNREACH    65        /**< \brief 主机不可达         */
#define AM_ENOTBLK         66        /**< \brief 非块设备           */
#define AM_EHOSTDOWN       67        /**< \brief 主机已经关闭       */

/** @} */

/**
 * \name 非阻塞和中断I/O错误值
 * @{
 */

#define AM_EINPROGRESS     68         /**< \brief 操作正在进行中 */
#define AM_EALREADY        69         /**< \brief 连接正被使用中 */

/* 70 */

#define AM_EWOULDBLOCK     AM_EAGAIN  /**< \brief 操作会阻塞（同EAGAIN） */
#define AM_ENOSYS          71         /**< \brief 不支持的功能（功能未实现）*/
 
/** @} */

/**
 * \name 异步I/O错误值
 * @{
 */
#define AM_ECANCELED       72         /**< \brief 操作已经取消 */

/* 73 */
/** @} */

/**
 * \name 流相关错误值
 * @{
 */
#define AM_ENOSR           74        /**< \brief 没有流资源 */
#define AM_ENOSTR          75        /**< \brief 不是流设备 */
#define AM_EPROTO          76        /**< \brief 协议错误 */
#define AM_EBADMSG         77        /**< \brief 损坏的消息 */
#define AM_ENODATA         78        /**< \brief 流中无数据 */
#define AM_ETIME           79        /**< \brief 流ioctl()超时 */
#define AM_ENOMSG          80        /**< \brief 没有所需的消息类型 */
#define AM_EUCLEAN         81        /**< \brief Structure需要清理 */
/** @} */                            

/**
* \name 其它自定义错误值
 * @{
 */
 
#define AM_EFULL           100     /**< \brief 满               */ 
#define AM_EEMPTY          101     /**< \brief 空               */
#define AM_ERXOV           102     /**< \brief 接收溢出         */
#define AM_ETXUR           103     /**< \brief 发送Underrun错误 */
#define AM_ESSA            104     /**< \brief 从机断言         */
#define AM_ESSD            105     /**< \brief 从机解除断言     */
#define AM_EHIGH           106     /**< \brief 值过高           */
#define AM_ELOW            107     /**< \brief 值过低           */

/** @} */

/** \brief 用户自定义错误起始值 */
#define AM_ERRNO_USER_START 2000

/** \brief 用户自定义错误值 */
#define AM_ERRNO_USER(x)    (AM_ERRNO_USER_START + (x))

/** @} */

/**
 * @} 
 */

#ifdef __cplusplus
}
#endif

#endif /* __AM_ERRNO_H */

/* end of file */
