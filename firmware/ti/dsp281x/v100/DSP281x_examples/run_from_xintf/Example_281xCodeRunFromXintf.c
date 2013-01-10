//###########################################################################
//
// FILE:    Example_281xCodeRunFromXintf.c
//
// TITLE:   Example Program That Executes From XINTF (assumes MPNMC = 1)
//
// ASSUMPTIONS:
//
//          This program requires the DSP281x V1.00 header files.  
//          As supplied, this project is configured for "boot from XINTF Zone 7" 
//          operation. 
//
//          XMP/MCn pin = 1
//
//          Map the region for XINTF Zone 7 as RAM in Code Composer
//          (Refer to the F2812.gel file supplied with CCS Studio)  
//
// DESCRIPTION:
//
//          This example configures CPU Timer0 and increments
//          a counter each time the timer asserts an interrupt.
//
//          Watch Variables:
//                 CpuTimer0.InterruptCount
//                 BackGroundCounter
//###########################################################################
//
// Original Author: A.T.
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Changed the method for copying code from
//      |             |      |    XINTF to internal RAM via the MemCopy
//      |             |      |    function and symbols created by the 
//      |             |      |    linker.
//      |             |      | Added pipeline flush after the XINTF Init
//###########################################################################

// Step 0: Include required header files:
//         DSP281x_Device.h: device specific definitions #include statements for
//         all of the peripheral .h definition files.

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Prototype statements for functions found within this file:
// Assign this function to a section to be linked to internal RAM
#pragma CODE_SECTION(xintf_zone6and7_timing,"ramfuncs");
void xintf_zone6and7_timing(void);
interrupt void ISR_CPUTimer0(void);
void error(void);

// Global variables found within this file:
Uint32 BackgroundCount = 0;

// RAM Count - This counter will be stored in RAM,  to compare with that stored
// in XINTF.
#pragma DATA_SECTION(RamInterruptCount,"ramdata");
Uint32 RamInterruptCount = 0;

void main(void)
{
    //-----------------------------------------------------------------------
    // Step 1 to Step 5 should be followed in all designs:
    //
    // Step 1: Disable and clear all CPU interrupts:
       DINT;
       IER = 0x0000;
       IFR = 0x0000;

    // Step 2. Initialize System Control registers, PLL, WatchDog, Clocks to default state:
    //         This function is found in the DSP281x_SysCtrl.c file.
    InitSysCtrl();

    // Step 3. Select GPIO for the device or for the specific application:
    //         This function is found in the DSP281x_Gpio.c file.
    // InitGpio();  // Not required for this example

    // Step 4. Initialize PIE to known state (control registers and vector table):
    //         The PIE vector table is initialized with pointers to shell Interrupt 
    //         Service Routines (ISR). The shell routines are found in DSP281x_DefaultIsr.c.
    //         Insert user specific ISR code in the appropriate shell ISR routine in 
    //         the DSP281x_DefaultIsr.c file.
    //
    //         Initialize Pie Control Registers To Default State:
    //         This function is found in the DSP281x_PieCtrl.c file.
    InitPieCtrl();

    //         Initialize the PIE Vector Table To a Known State:
    //         This function is found in DSP281x_PieVect.c.
    //         This function populates the PIE vector table with pointers
    //         to the shell ISR functions found in DSP281x_DefaultIsr.c.
    InitPieVectTable();

    // Enable CPU and PIE interrupts
    // This example function is found in the DSP281x_PieCtrl.c file.   
    EnableInterrupts();	
	
    // Step 5. Initialize all the Device Peripherals to a known state:
    //         This function is found in DSP281x_InitPeripherals.c
    // InitPeripherals();  // For this example just init the CPU Timers
    InitCpuTimers();
    
    //-----------------------------------------------------------------------
    // User specific code:
    // 
    // Initalize XINTF Zone 6 and Zone 7 timing
    // First copy the function to RAM, then call the function to
    // change the zone timing
    
    // The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
    // symbols are created by the linker.
    // Refer to the CodeRunFromXintfonF2812EzDSP.cmd file. 
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);	
	xintf_zone6and7_timing();
	
	EALLOW;	   									// This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &ISR_CPUTimer0;
	EDIS;      									// This is needed to disable write to EALLOW protected registers
    
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;			// Enable INT1.7 which is connected to CPU-Timer 0:
    IER |= M_INT1;
    	
    SetDBGIER(M_INT1); 							// Enable INT1 for Real-Time mode	
	EINT;   									// Enable Global interrupt INTM
	ERTM;										// Enable Global realtime debug DBGM

	// Configure and Initialize CPU Timer 0 for:
	//     > CPU Timer 0 Connect To INT1.7
	//	   > Set Up For 1 Second Interrupt Period
	//	   > Point To "ISR_CPUTimer0" function
	
	ConfigCpuTimer(&CpuTimer0, 150, 1000000);	// 150MHz CPU Freq, 1 second Period (in uSeconds)
 	StartCpuTimer0();



	for(;;)
		BackgroundCount++;                      // IDLE loop. Just sit and loop forever....

	exit(0);									// should never get here.
} 	


interrupt void ISR_CPUTimer0(void)
{
	PieCtrlRegs.PIEACK.all = 0xFFFF;
	ERTM;             							// Enable Global realtime debug DBGM
	
	CpuTimer0.InterruptCount++;					// Should increment every second.
    RamInterruptCount++;
    if(RamInterruptCount != CpuTimer0.InterruptCount) error();

}


// Configure the timing paramaters for Zone 6 and Zone 7.
// Note: this function should not be executed from the same
// zones as those being configured. 
void xintf_zone6and7_timing()
{

    // All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = SYSCLKOUT 
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
    // Buffer up to 3 writes
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;
    // XCLKOUT is enabled
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
    // XCLKOUT = XTIMCLK 
    XintfRegs.XINTCNF2.bit.CLKMODE = 0;
    
    
    // Zone 6------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING6.bit.XWRLEAD = 1;
    XintfRegs.XTIMING6.bit.XWRACTIVE = 1;
    XintfRegs.XTIMING6.bit.XWRTRAIL = 1;
    // Zone read timing
    XintfRegs.XTIMING6.bit.XRDLEAD = 1;
    XintfRegs.XTIMING6.bit.XRDACTIVE = 2;
    XintfRegs.XTIMING6.bit.XRDTRAIL = 0;
    
    // do not double all Zone read/write lead/active/trail timing 
    XintfRegs.XTIMING6.bit.X2TIMING = 0;
 
    // Zone will not sample READY 
    XintfRegs.XTIMING6.bit.USEREADY = 0;
    XintfRegs.XTIMING6.bit.READYMODE = 0;  
 
    // Size must be 1,1 - other values are reserved
    XintfRegs.XTIMING6.bit.XSIZE = 3;
 

    // Zone 7------------------------------------
    // When using ready, ACTIVE must be 1 or greater
    // Lead must always be 1 or greater
    // Zone write timing
    XintfRegs.XTIMING7.bit.XWRLEAD = 1;
    XintfRegs.XTIMING7.bit.XWRACTIVE = 1;
    XintfRegs.XTIMING7.bit.XWRTRAIL = 1;
    // Zone read timing
    XintfRegs.XTIMING7.bit.XRDLEAD = 1;
    XintfRegs.XTIMING7.bit.XRDACTIVE = 2;
    XintfRegs.XTIMING7.bit.XRDTRAIL = 0;
    
    // don't double all Zone read/write lead/active/trail timing 
    XintfRegs.XTIMING7.bit.X2TIMING = 0;
 
    // Zone will not sample XREADY signal  
    XintfRegs.XTIMING7.bit.USEREADY = 0;
    XintfRegs.XTIMING7.bit.READYMODE = 0;
 
    // Size must be 1,1 - other values are reserved
    XintfRegs.XTIMING7.bit.XSIZE = 3;
    
   //Force a pipeline flush to ensure that the write to 
   //the last register configured occurs before returning.  
   asm(" RPT #7 || NOP"); 
}

void error(void)
{
    asm("      ESTOP0");   // software breakpoint
}

//===========================================================================
// No more.
//===========================================================================
