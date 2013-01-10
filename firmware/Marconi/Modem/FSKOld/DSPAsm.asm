    .def _ConvASM

    .sect "RamCode"

    ; Convolution with single-cycle MAC

_ConvASM:

    push    xar0
    push    xar7
    push    xar5
    setc    sxm, ovm
    spm     -6 
    movl    xar7, xar4
    addb    xar5, #1
    subb    al, #1
    mov     ar0, al
    zapa
    rpt     ar0 || mac p, *--xar5, *xar7++
    addl    acc, p << pm
    spm     0
    clrc    ovm
    pop     xar5
    pop     xar7
    pop     xar0
    lretr
