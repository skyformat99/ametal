
/*
 * int am_int_cpu_lock();
 */
    .global am_int_cpu_lock
    .type am_int_cpu_lock, %function
am_int_cpu_lock:
    MRS     R0, PRIMASK
    CPSID   I
    BX      LR

/*
 * void am_int_cpu_unlock(int level);
 */
    .global am_int_cpu_unlock
    .type am_int_cpu_unlock, %function
am_int_cpu_unlock:
    MSR     PRIMASK, R0
    BX      LR
