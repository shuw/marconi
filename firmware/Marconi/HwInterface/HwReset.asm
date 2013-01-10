    .ref _c_int00

    .sect "CodeEntry"

    ; This code effectively only gets executed when running out of flash,
    ; since both in the debugger and in RAM on the actual board, the 
    ; entry point used is _c_int00, not code_entry_point. This is due to
    ; the fact that the linker FORCES _c_int00 as the entry point when
    ; linking with the C/C++ library.

code_entry_point:
    LB wd_disable       ; Branch to watchdog disable code

    ; This to be located with rest of code in text section
    .text

wd_disable:

    SETC OBJMODE        ; Set OBJMODE for 28x object code
    EALLOW              ; Enable EALLOW protected register access
    MOVZ DP, #7029h>>6  ; Set data page for WDCR register
    MOV @7029h, #0068h  ; Set WDDIS bit in WDCR to disable WD
    EDIS                ; Disable EALLOW protected register access
    LB _c_int00         ; Branch to start of boot.asm in RTS library

    .end
