*******************************************************
* TMS320C2x/C2xx/C5x ANSI C Codegen Version 7.00       
*******************************************************
;	d:\tic2xx\c2000\cgtools\bin\dspac.exe -v2xx -q miscsfns.c C:\TEMP\miscsfns.if 
;	dspopt NOT RUN
;	d:\tic2xx\c2000\cgtools\bin\dspcg.exe -v2xx -o -q -o C:\TEMP\miscsfns.if C:\TEMP\miscsfns.asm C:\TEMP\miscsfns.tmp 
	.port
	.file	"miscsfns.c"
	.file	"..\include\regs24x.h"
	.globl	_portffff
	.globl	_port0
	.globl	_port1
	.globl	_port2
	.globl	_port3
	.globl	_port4
	.file	"miscsfns.c"
	.text

	.sym	_disable_ints,_disable_ints,32,2,0
	.globl	_disable_ints

	.func	14
;>>>> 	void inline disable_ints()
******************************************************
* FUNCTION DEF : _disable_ints
******************************************************
_disable_ints:
	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+

	.line	3
;>>>> 	        asm("                setc        intm        ");        
                setc        intm        
EPI0_1:
	.line	4
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	17,000000000H,1

	.sym	_enable_ints,_enable_ints,32,2,0
	.globl	_enable_ints

	.func	19
;>>>> 	void inline enable_ints()
******************************************************
* FUNCTION DEF : _enable_ints
******************************************************
_enable_ints:
	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+

	.line	3
;>>>> 	        asm("                clrc        intm        ");
                clrc        intm        
EPI0_2:
	.line	4
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	22,000000000H,1

	.sym	_enable_dmc1000,_enable_dmc1000,32,2,0
	.globl	_enable_dmc1000

	.func	23
;>>>> 	void enable_dmc1000(void)
******************************************************
* FUNCTION DEF : _enable_dmc1000
******************************************************
_enable_dmc1000:
	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+

EPI0_3:
	.line	5
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	27,000000000H,1

	.sym	_enable_labdrive,_enable_labdrive,32,2,0
	.globl	_enable_labdrive

	.func	29
;>>>> 	void enable_labdrive(void)
******************************************************
* FUNCTION DEF : _enable_labdrive
******************************************************
_enable_labdrive:
	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+,AR3

	.line	3
;>>>> 	        PBDATDIR=((PBDATDIR|0x4000)&0xffbf);
	LARK	AR3,28826
	LACK	16384
	OR	* 
	ANDK	65471
	SACL	* 
	.line	4
;>>>> 	        OCRB=OCRB&0xffdf;
	LACK	65503
	SBRK	8
	AND	* 
	SACL	* 
	.line	5
;>>>> 	        PCDATDIR=PCDATDIR|0x2020;
	LACK	8224
	ADRK	10
	OR	* 
	SACL	* 
	.line	6
;>>>> 	        PDDATDIR=(PDDATDIR|0x1000)&0xffef;
	LACK	4096
	ADRK	2
	OR	* 
	ANDK	65519
	SACL	* ,AR1
EPI0_4:
	.line	8
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	36,000000000H,1
	.end
