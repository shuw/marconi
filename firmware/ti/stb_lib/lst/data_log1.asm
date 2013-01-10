*******************************************************
* TMS320C2x/C2xx/C5x ANSI C Codegen Version 7.00       
*******************************************************
;	d:\tic2xx\c2000\cgtools\bin\dspac.exe -v2xx -q data_log1.c C:\TEMP\data_log1.if 
;	dspopt NOT RUN
;	d:\tic2xx\c2000\cgtools\bin\dspcg.exe -v2xx -o -q -o C:\TEMP\data_log1.if C:\TEMP\data_log1.asm C:\TEMP\data_log1.tmp 
	.port
	.file	"data_log1.c"
	.file	"..\include\data_log.h"

	.stag	.fake0,224
	.member	_dlog_iptr1,0,20,8,16
	.member	_dlog_iptr2,16,20,8,16
	.member	_trig_value,32,4,8,16
	.member	_graph_ptr1,48,4,8,16
	.member	_graph_ptr2,64,4,8,16
	.member	_dlog_skip_cntr,80,4,8,16
	.member	_dlog_cntr,96,4,8,16
	.member	_task_ptr,112,4,8,16
	.member	_dlog_prescale,128,4,8,16
	.member	_dlog_cntr_max,144,4,8,16
	.member	_dl_buffer1_adr,160,4,8,16
	.member	_dl_buffer2_adr,176,4,8,16
	.member	_init,192,148,8,16
	.member	_update,208,148,8,16
	.eos
	.sym	_DATALOG,0,8,13,224,.fake0
	.globl	_data_log_init
	.globl	_data_log_update
	.file	"data_log1.c"
	.text

	.sym	_data_log_init,_data_log_init,32,2,0
	.globl	_data_log_init

	.func	44
;>>>> 	void data_log_init(DATALOG *p)
******************************************************
* FUNCTION DEF : _data_log_init
******************************************************
_data_log_init:

LF1	.set	0

	POPD	*+
	SAR	AR0,*+
	SAR	AR1,*
	LARK	AR0,2
	LAR	AR0,*0+,AR2

	.sym	_p,-3+LF1,24,9,16,.fake0
	.sym	_temp_add,1,20,1,16
	.line	2
;>>>> 	    int *temp_add;
	.line	5
;>>>> 	    temp_add = &p->task_ptr+6; /* temp_add is pointing to the update function */
	LARK	AR2,-3+LF1
	MAR	*0+
	LAC	* 
	ADDK	13
	ADRK	4-LF1
	SACL	* 
	.line	6
;>>>> 	    p->task_ptr = *temp_add+16;      /* initialize p.task_ptr to the #POS_TRIG_S1 */
	LAR	AR3,* ,AR3
	LAC	* ,AR2
	ADDK	16
	SBRK	4-LF1
	LAR	AR4,* ,AR4
	ADRK	7
	SACL	* ,AR1
EPI0_1:
	.line	7
	SBRK	3
	LAR	AR0,*-
	PSHD	*
	RET

	.endfunc	50,000000000H,2
	.end
