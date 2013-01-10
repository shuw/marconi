*******************************************************
* TMS320C2x/C2xx/C5x ANSI C Codegen Version 7.00       
*******************************************************
;	d:\tic2xx\c2000\cgtools\bin\dspac.exe -v2xx -q pwmdac1.c C:\TEMP\pwmdac1.if 
;	dspopt NOT RUN
;	d:\tic2xx\c2000\cgtools\bin\dspcg.exe -v2xx -o -q -o C:\TEMP\pwmdac1.if C:\TEMP\pwmdac1.asm C:\TEMP\pwmdac1.tmp 
	.port
	.file	"pwmdac1.c"
	.file	"..\include\regs240x.h"
	.globl	_portffff
	.globl	_portff0f
	.globl	_port0
	.globl	_port1
	.globl	_port2
	.globl	_port3
	.globl	_port4
	.file	"..\include\pwmdac.h"

	.stag	.fake0,96
	.member	_period_max,0,4,8,16
	.member	_pwmdac_iptr0,16,20,8,16
	.member	_pwmdac_iptr1,32,20,8,16
	.member	_pwmdac_iptr2,48,20,8,16
	.member	_init,64,148,8,16
	.member	_update,80,148,8,16
	.eos
	.sym	_PWMDAC,0,8,13,96,.fake0
	.sym	_PWMDAC_handle,0,24,13,16,.fake0
	.globl	_PWM_DAC_DRV_Init
	.globl	_PWM_DAC_DRV_Update
	.file	"pwmdac1.c"
	.text

	.sym	_PWM_DAC_DRV_Init,_PWM_DAC_DRV_Init,36,2,0
	.globl	_PWM_DAC_DRV_Init

	.func	28
;>>>> 	int PWM_DAC_DRV_Init(PWMDAC *p) 
******************************************************
* FUNCTION DEF : _PWM_DAC_DRV_Init
******************************************************
_PWM_DAC_DRV_Init:

LF1	.set	0

	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,1
	LAR	AR0,*0+,AR3

	.sym	_p,-3+LF1,24,9,16,.fake0
	.line	2
	.line	3
;>>>> 	        SCSR1|=0x0008;                       /* Turn EVB Clocks on            */
	LARK	AR3,28696
	LACK	8
	OR	* 
	SACL	* ,AR2
	.line	4
;>>>> 	        T3PR=p->period_max;                  /* Init Timer 3 period Register  */
	LARK	AR2,-3+LF1
	MAR	*0+
	LAR	AR4,* ,AR4
	LARK	AR5,29955
	LAC	* ,AR5
	SACL	*+
	.line	5
;>>>> 	        T3CON=PWM_INIT_STATE;                /* Symmetrical Operation         */
	SPLK	#-30656,* 
	.line	6
;>>>> 	        DBTCONB=DBTCON_INIT_STATE;                                    
	LACK	0
	ADRK	17
	SACL	* 
	.line	7
;>>>> 	        ACTRB=ACTR_INIT_STATE;                
	SBRK	2
	SPLK	#2457,* 
	.line	9
;>>>> 	        COMCONB=0xa200;
	SBRK	2
	SPLK	#-24064,* ,AR4
	.line	11
;>>>> 	        CMPR4=p->period_max;
	LAC	* ,AR5
	ADRK	6
	SACL	*+,AR4
	.line	12
;>>>> 	        CMPR5=p->period_max;
	LAC	* ,AR5
	SACL	*+,AR4
	.line	13
;>>>> 	        CMPR6=p->period_max;
	LAC	* ,AR5
	SACL	* ,AR3
	.line	15
;>>>> 	        MCRC|=0x007e;                    /* Set up the full compare
;>>>> 	                                                PWM pins to primary functions.*/
	ADRK	124
	LACK	126
	OR	* 
	SACL	* 
	.line	18
;>>>> 	        return(0);
	LACK	0
EPI0_1:
	.line	20
	MAR	* ,AR1
	SBRK	2
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	47,000000000H,1
	.end
