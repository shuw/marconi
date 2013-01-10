The following document describes how to use SdFlash to program
over the F28xx serial port.

Requirements:
=============

1) SDFlash version 1.60 or higher.  Version 1.60 and higher expose
   a generic interface to support programming via non-jtag connections.

2) F28xx revision C or higher. This is the minimum revision that
   is tested.

3) Serial port connection between host and serial port A of the
   F2812.


Setup:
======
Programming over the serial port using SDFlash requires additional
setup vs JTAG based programming. Follow the step below.

1) Setup sdopts.cfg.  SD tools store emulator setup and configuration
   in sdopts.cfg which is generally configured by SdConfig.
   SdConfig will NOT configure a non-JTAG tool so this has to be
   done manually.  To configure for host COM1 add the following
   to sdopts.cfg. This has to be added before the line
   "# End of sdopts.cfg" comment.  SDFlash will set up COM1
   through COM4.  To change to a different serial port change
   "C1" to C2,C3, or C4.
   
   # SDFlash setup for host COM1
   [EmulatorId=C1]
   EmuPortAddr=0xC1
   EmuPortMode=RS232
   EmuProductName=SERIAL_FLASH

   The file "AddToSdOpts.cfg" is preconfigured for all serial ports
   and can be pasted into sdopts.cfg.  The file is in the f2812
   sdflash project directory.

2) Connect serial cable between target and host. 
   Set F28xx for serial bootload mode.
   Power up target and reset F28xx.

3) Start SDFlash and load the project F2812SerialFlash.sdp.
   The project should be located at:
   <ccs_install_dir>\specdig\sdflash\sdf28x_v2_1_serial\f2812.
   The project is setup with <ccs_install_dir> set to "c:\ti".
   If you installed in a different directory then you will
   need to update your project settings. 

   All the files specific to serial programming are located in
   the <ccs_install_dir>\specdig\sdflash\sdf28x_v2_1_serial\f2812
   directory.  The required files are:

      F2812SerialFlash.sdp  -- SDFlash serial project file.
      F28xxRS232Flash.dll   -- SDFlash driver file.
      F2812SerialFlash.out  -- SDFlash algo file.
      ccBrd028x.dat         -- CCS board.dat file. 
      mem_test.out          -- Example test file.
      AddToSdOpts.cfg       -- sdopts.cfg setup parameters.

   The file ccBrd028x.dat is not used but required to satify
   SDFlash project options.

   AddToSdOpts.cfg can be pasted into sdopts.cfg.

F2812/Host Configuration:
========================
The algo and host side software is configured for the following:
  F2812 support
  PLL = x10/2 
  Algo's configured for 150MHz operation 
  Passwords assumed to be erased (0xFFFF)
  Baudrate 9600

This configuration is tested on a TI F2812 Verification and Debug
board (VDB).  Other configurations are possible but may require
modification of the host or F2812 applications.  All source and
and project files are included.  See section "SDFlash Driver and Algo Files"
for more details.
      
Programming:
============
Programming over the serial port is limited in a few areas
when compared to a JTAG emulator.
  
1) F28xx must be in a functional state and running the
   serial bootloader on startup. This implies that F28xx
   is configured for SCI boot option.  If you have an
   emulator connected to the JTAG port then you should
   remove it.  Emulators that support Wait-In-Reset like
   the XDS510PP_PLUS will hold the F28xx in reset following
   a power cycle.

2) The reset button of SDFlash CANNOT bring a target back
   to a good state. If the target is in a bad state or not
   in bootloader mode then user must use a hardware reset
   to get target back to bootloader mode.  Once that is 
   done the reset button used to inform SDFlash that
   target is in bootloader mode.

3) There can only be one flash programming algo file to
   support erase, program and erase.  On the first connection
   to the target SDFlash will load the algo file while
   the F28xx is in bootloader mode.  Once the algo is loaded
   the F28xx exits bootloader mode.  At this point no other
   algo can be loaded until the F28xx returns to bootloader
   mode.  To return to bootloader mode you have to do the
   following:
     A. Apply hardware reset to F28xx or cycle power.
     B. From SDFlash perform a reset.  This will inform
        SDFlash that target is in bootloader mode.
 

SDFlash Driver and Algo Files:
==============================
SDFlash v1.60 has been modified to expose a generic interface
to support a generic communications connection. All the source code, 
project files and documentation to build F28xxRS232Flash.dll and
F2812SerialFlash.out are installed in the following directory,

  <ccs_install_dir>\specdig\sdflash\mydrivers\DSP281x_v2_1

The source\build documentation is in the subdirectory
"DSL281x_serial\docs".  The source is provided as is without
technical support.

Known Limitations
=================
1) As delivered the package only supports the F2812. The user
   can reconfigure the algos to support the other F28xx devices.

2) The "Toggle Test" as described in the TI Flash API Beta 1
   package has a bug in the lib.. The erase algo has been modified
   to ignore the Toggle Test option that uses bits in the "F" gpio
   register.  The Flash API corrupts the F gpio register which
   cause SCIA to fail.  Source code documents this problem.

3) The Verify algo performs a simple "checksum" on the programmed
   data.  Verify is done COFF section by COFF section after all sections
   have been programmed.  A more complex verify is left to the
   user to implement.





