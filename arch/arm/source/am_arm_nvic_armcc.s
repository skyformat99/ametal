;/*
; * section
; */
    AREA CPU_LOCK, CODE
   
;/*
; * rt_base_t am_int_cpu_lock();
; */
am_int_cpu_lock    PROC
    EXPORT  am_int_cpu_lock
    MRS     r0, PRIMASK
    CPSID   I
    BX      LR
    ENDP

;/*
; * void am_int_cpu_unlock(rt_base_t level);
; */
am_int_cpu_unlock    PROC
    EXPORT  am_int_cpu_unlock
    MSR     PRIMASK, r0
    BX      LR
    ENDP

;/*
; * end of file
; */
    END
