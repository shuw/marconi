*******************************************************
* TMS320C2x/C2xx/C5x ANSI C Codegen Version 7.00       
*******************************************************
;	d:\tic2xx\c2000\cgtools\bin\dspac.exe -v2xx -q f2407adc1.c C:\TEMP\f2407adc1.if 
;	dspopt NOT RUN
;	d:\tic2xx\c2000\cgtools\bin\dspcg.exe -v2xx -o -q -o C:\TEMP\f2407adc1.if C:\TEMP\f2407adc1.asm C:\TEMP\f2407adc1.tmp 
	.port
	.file	"f2407adc1.c"
	.file	"..\include\regs240x.h"
	.globl	_portffff
	.globl	_portff0f
	.globl	_port0
	.globl	_port1
	.globl	_port2
	.globl	_port3
	.globl	_port4
	.file	"..\include\F2407ADC.h"

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
	.globl	_F2407_ADC_Init
	.globl	_F2407_ADC_Update
	.file	"f2407adc1.c"
	.text

	.sym	_F2407_ADC_Init,_F2407_ADC_Init,32,2,0
	.globl	_F2407_ADC_Init

	.func	37
;>>>> 	void inline F2407_ADC_Init(ADCVALS *p)
******************************************************
* FUNCTION DEF : _F2407_ADC_Init
******************************************************
_F2407_ADC_Init:

LF1	.set	0

	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+,AR3

	.sym	_p,-3+LF1,24,9,16,.fake0
	.line	2
	.line	4
;>>>> 	        SCSR1=SCSR1|0x0080;                 /* Turn on the clocks to the ADC module*/
	LARK	AR3,28696
	LACK	128
	OR	* 
	SACL	* 
	.line	5
;>>>> 	        CALIBRATION=CALIBRATION_CONSTANT;
	LACK	0
	ADRK	160
	SACL	* 
	.line	7
;>>>> 	        ADCTRL1=ADC_RESET_FLAG;             /* Reset the ADC Module    */
	SBRK	24
	SPLK	#16384,* 
	.line	8
;>>>> 	        ADCTRL1=ADCTRL1_INIT_STATE;         /* Set up ADCTRL1 register */
	SPLK	#8464,*+
	.line	9
;>>>> 	        ADCTRL2=ADCTRL2_INIT_STATE;         /* Set up ADCTRL2 register */
	SPLK	#8706,*+
	.line	10
;>>>> 	        MAXCONV=3;                          /* Specify four conversions  */
	LACK	3
	SACL	* ,AR2
	.line	12
;>>>> 	        CHSELSEQ1=p->a4_ch_sel;            /* Configure channel selection */ 
	LARK	AR2,-3+LF1
	MAR	*0+
	LAR	AR4,* ,AR4
	ADRK	8
	LAC	* ,AR3
	MAR	*+
	SACL	* ,AR1
EPI0_1:
	.line	13
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	49,000000000H,1
	.end
