*******************************************************
* TMS320C2x/C2xx/C5x ANSI C Codegen Version 7.00       
*******************************************************
;	d:\tic2xx\c2000\cgtools\bin\dspac.exe -v2xx -q f24xwdog.c C:\TEMP\f24xwdog.if 
;	dspopt NOT RUN
;	d:\tic2xx\c2000\cgtools\bin\dspcg.exe -v2xx -o -q -o C:\TEMP\f24xwdog.if C:\TEMP\f24xwdog.asm C:\TEMP\f24xwdog.tmp 
	.port
	.file	"f24xwdog.c"
	.file	"..\include\regs24x.h"
	.globl	_portffff
	.globl	_port0
	.globl	_port1
	.globl	_port2
	.globl	_port3
	.globl	_port4
	.file	"f24xwdog.c"
	.text

	.sym	_F24x_WD_ResetCounter,_F24x_WD_ResetCounter,32,2,0
	.globl	_F24x_WD_ResetCounter

	.func	33
;>>>> 	void F24x_WD_ResetCounter(void)
******************************************************
* FUNCTION DEF : _F24x_WD_ResetCounter
******************************************************
_F24x_WD_ResetCounter:
	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+,AR3

	.line	3
;>>>> 	        WDKEY=0x5555;                  /* Reset WDog   */
	LARK	AR3,28709
	SPLK	#21845,* 
	.line	4
;>>>> 	        WDKEY=0xaaaa;
	SPLK	#-21846,* ,AR1
EPI0_1:
	.line	5
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	37,000000000H,1

	.sym	_F24x_WD_Disable,_F24x_WD_Disable,32,2,0
	.globl	_F24x_WD_Disable

	.func	39
;>>>> 	void F24x_WD_Disable(void)
******************************************************
* FUNCTION DEF : _F24x_WD_Disable
******************************************************
_F24x_WD_Disable:
	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+,AR3

	.line	3
;>>>> 	        WDCR=0x006f;                /* Write Control Register to Disable Watchdog */
	LARK	AR3,28713
	LACK	111
	SACL	* ,AR1
EPI0_2:
	.line	4
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	42,000000000H,1
	.end
