@REM   *************************************************************************
@REM   ** Texas  Instruments Inc.
@REM   ** Script file for builing the driver libraries
@REM   **
@REM   *************************************************************************
@cd    ..\src
@echo  Assembling Source Files .....
@asm2000 -v28 -q -gs  DLOG4CHC.asm   ..\obj\DLOG4CHC.obj    -l ..\lst\DLOG4CHC.lst
@asm2000 -v28 -q -gs  sgti1c.asm   	 ..\obj\sgti1c.obj   	-l ..\lst\sgti1c.lst
@asm2000 -v28 -q -gs  Sintb360.asm   ..\obj\Sintb360.obj    -l ..\lst\Sintb360.lst

@echo  Done !

@cd  ..\obj
@echo Deleting the old library file (stb.lib) .... 
@del ..\lib\stb.lib
@echo .
@echo .
@echo  Building archives .....
@ar2000 -rv ..\lib\stb.lib DLOG4CHC.obj
@ar2000 -rv ..\lib\stb.lib sgti1c.obj
@ar2000 -rv ..\lib\stb.lib Sintb360.obj
@echo  ***************************    DONE    *********************************
