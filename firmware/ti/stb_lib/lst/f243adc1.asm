*******************************************************
* TMS320C2x/C2xx/C5x ANSI C Codegen Version 7.00       
*******************************************************
;	d:\tic2xx\c2000\cgtools\bin\dspac.exe -v2xx -q f243adc1.c C:\TEMP\f243adc1.if 
;	dspopt NOT RUN
;	d:\tic2xx\c2000\cgtools\bin\dspcg.exe -v2xx -o -q -o C:\TEMP\f243adc1.if C:\TEMP\f243adc1.asm C:\TEMP\f243adc1.tmp 
	.port
	.file	"f243adc1.c"
	.file	"..\include\regs24x.h"
	.globl	_portffff
	.globl	_port0
	.globl	_port1
	.globl	_port2
	.globl	_port3
	.globl	_port4
	.file	"..\include\F243_ADC.h"

	.stag	.fake0,176
	.member	_c1_gain,0,4,8,16
	.member	_c2_gain,16,4,8,16
	.member	_c3_gain,32,4,8,16
	.member	_c4_gain,48,4,8,16
	.member	_c1_out,64,4,8,16
	.member	_c2_out,80,4,8,16
	.member	_c3_out,96,4,8,16
	.member	_c4_out,112,4,8,16
	.member	_a4_ch_sel,128,4,8,16
	.member	_init,144,148,8,16
	.member	_update,160,148,8,16
	.eos
	.sym	_ADCVALS,0,8,13,176,.fake0
	.sym	_ADCVALS_handle,0,24,13,16,.fake0
	.globl	_F243_ADC_Init
	.globl	_F243_ADC_Update
	.file	"f243adc1.c"
	.text

	.sym	_F243_ADC_Init,_F243_ADC_Init,32,2,0
	.globl	_F243_ADC_Init

	.func	36
;>>>> 	void inline F243_ADC_Init(ADCVALS *p)
******************************************************
* FUNCTION DEF : _F243_ADC_Init
******************************************************
_F243_ADC_Init:

LF1	.set	0

	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+,AR3

	.sym	_p,-3+LF1,24,9,16,.fake0
	.line	2
	.line	3
;>>>> 	        ADCTRL2=ADCTRL2_INIT_STATE;
	LARK	AR3,28724
	LACK	2
	SACL	* 
	.line	4
;>>>> 	        ADCTRL1=ADCTRL1_INIT_STATE;
	SBRK	2
	SPLK	#-1792,* ,AR2
	.line	5
;>>>> 	        ADCTRL1=ADCTRL1+((p->a4_ch_sel)&ADC2CHSEL_7)+(((p->a4_ch_sel)<<1)&ADC1CHSEL_7);
	LARK	AR2,-3+LF1
	MAR	*0+
	LAR	AR4,* ,AR4
	ADRK	8
	LAC	* ,1,AR1
	ANDK	14
	SACL	* ,AR4
	LACK	112
	AND	* ,AR3
	ADD	* ,AR1
	ADD	* ,AR3
	SACL	* ,AR1
EPI0_1:
	.line	7
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	42,000000000H,1
	.end
