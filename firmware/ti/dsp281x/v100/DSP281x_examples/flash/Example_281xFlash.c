//###########################################################################
//
// FILE:   DSP28_281xFlash.c
//
// TITLE:  DSP281x Flash example.
//
// ASSUMPTIONS:
//
//          This program requires the DSP281x V1.00 header files.  
//          As supplied, this project is configured for "boot to Flash" operation.   
//
//          This example runs the EV Timer Period example from the 
//          Flash.  
//
//          1) Build the project
//          2) Flash the .out file into the device.  This program will fit on 
//             an F2812 or an F2810
//          3) Set the hardware jumpers to boot to Flash 
//          4) Use the included GEL file to load the project, symbols 
//             defined within the project and the variables into the watch
//             window.   
//
//          Steps that were taken to convert the EV example from RAM 
//          to Flash execution:
//
//          - Change the linker cmd file to reflect the flash memory map.
//            This example uses the DSP281x F2812.cmd file.
//          - Make sure any initialized sections are mapped to Flash.  
//            In SDFlash utility this can be checked by the View->Coff/Hex
//            status utility. Any section marked as "load" should be
//            allocated to Flash.
//          - Make sure there is a branch instruction from the entry to Flash
//            at 0x3F7FF6 to the beginning of code execution. This example
//            uses the DSP281x_CodeStartBranch.asm file to accomplish this.
//          - Set boot mode Jumpers to "boot to Flash"
//          - For best performance from the flash, modify the waitstates
//            and enable the flash pipeline as shown in this example.
//            Note: any code that manipulates the flash waitstate and pipeline
//            control must be run from RAM. Thus these functions are located
//            in their own memory section called ramfuncs.
// 
// DESCRIPTION:
//
//          This program sets up EVA Timer 1, EVA Timer 2, EVB Timer 3
//          and EVB Timer 4 to fire an interrupt on a period overflow.  
//          A count is kept each time each interrupt passes through
//          the interrupt service routine. 
//
//          EVA Timer 1 has the shortest period while EVB Timer4 has the
//          longest period.
//
//          ISR Locations:
//                eva_timer1_isr  executed from RAM, puts flash in sleep mode
//                eva_timer2_isr  executed from RAM, puts flash in standby mode
//                evb_timer3_isr  executed from RAM, puts flash in sleep mode
//                                              and later moves it to standby
//                evb_timer4_isr  executed from FLASH
//        
//          Watch Variables:
//
//                EvaTimer1InterruptCount;
//                EvaTimer2InterruptCount;
//                EvbTimer3InterruptCount;
//                EvbTimer4InterruptCount;
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha)
//      |             |      | Updated comments with info on converting
//      |             |      |   a project to flash.
//      |             |      | Changed the method for copying code from
//      |             |      |    XINTF to internal RAM via the MemCopy
//      |             |      |    function and symbols created by the 
//      |             |      |    linker.
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Functions that will be run from RAM need to be assigned to 
// a different section.  This section will then be mapped using
// the linker cmd file.
#pragma CODE_SECTION(eva_timer1_isr, "ramfuncs");
#pragma CODE_SECTION(eva_timer2_isr, "ramfuncs");
#pragma CODE_SECTION(evb_timer3_isr, "ramfuncs");

// Prototype statements for functions found within this file.
interrupt void eva_timer1_isr(void);
interrupt void eva_timer2_isr(void);
interrupt void evb_timer3_isr(void);
interrupt void evb_timer4_isr(void);
void init_eva_timer1(void);
void init_eva_timer2(void);
void init_evb_timer3(void);
void init_evb_timer4(void);

// Global variables used in this example
Uint32 EvaTimer1InterruptCount;
Uint32 EvaTimer2InterruptCount;
Uint32 EvbTimer3InterruptCount;
Uint32 EvbTimer4InterruptCount;
Uint32 LoopCount;

// These are defined by the linker (see F2812.cmd)
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;


void main(void)
{
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP281x_SysCtrl.c file.
   InitSysCtrl();
   
// Step 2. Initalize GPIO: 
// This example function is found in the DSP281x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example  

// Step 3. Clear all interrupts and initialize PIE vector table:
// Disable CPU interrupts 
   DINT;

// Initialize PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags
// are cleared.  
// This function is found in the DSP281x_PieCtrl.c file.
   InitPieCtrl();

// Disable CPU interrupts and clear all CPU interrupt flags:
   IER = 0x0000;
   IFR = 0x0000;

// Initialize the PIE vector table with pointers to the shell Interrupt 
// Service Routines (ISR).  
// This will populate the entire table, even if the interrupt
// is not used in this example.  This is useful for debug purposes.
// The shell ISR routines are found in DSP281x_DefaultIsr.c.
// This function is found in DSP281x_PieVect.c.
   InitPieVectTable();

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.  	
   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.T1PINT = &eva_timer1_isr;
   PieVectTable.T2PINT = &eva_timer2_isr;
   PieVectTable.T3PINT = &evb_timer3_isr;
   PieVectTable.T4PINT = &evb_timer4_isr;
   EDIS;   // This is needed to disable write to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   init_eva_timer1();
   init_eva_timer2();
   init_evb_timer3();
   init_evb_timer4();
     
// Step 5. User specific code, enable interrupts:

// Copy time critical code and Flash setup code to RAM
// This includes the following ISR functions: EvaTimer1(), EvaTimer2()
// EvbTimer3 and and InitFlash();
// The  RamfuncsLoadStart, RamfuncsLoadEnd, and RamfuncsRunStart
// symbols are created by the linker. Refer to the F2812.cmd file. 
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);

// Call Flash Initialization to setup flash waitstates
// This function must reside in RAM
   InitFlash();

   // Initialize count values to 0
   EvaTimer1InterruptCount = 0;
   EvaTimer2InterruptCount = 0;
   EvbTimer3InterruptCount = 0;
   EvbTimer4InterruptCount = 0;
   LoopCount = 0;
 

   // Enable PIE group 2 interrupt 4 for T1PINT
   PieCtrlRegs.PIEIER2.all = M_INT4;
   // Enable PIE group 3 interrupt 1 for T2PINT
   PieCtrlRegs.PIEIER3.all = M_INT1;    
   // Enable PIE group 4 interrupt 4 for T3PINT
   PieCtrlRegs.PIEIER4.all = M_INT4;
   // Enable PIE group 5 interrupt 1 for T4PINT
   PieCtrlRegs.PIEIER5.all = M_INT1;

   // Enable CPU INT2 for T1PINT, INT3 for T2PINT, INT4 for T3PINT
   // and INT5 for T4PINT:
   IER |= (M_INT2 | M_INT3 | M_INT4 | M_INT5);

   // Enable global Interrupts and higher priority real-time debug events:

   EINT;   // Enable Global interrupt INTM
   ERTM;   // Enable Global realtime interrupt DBGM

// Step 6. IDLE loop. Just sit and loop forever:	
   while(1)
   {
       LoopCount++;
   }

} 
 

void init_eva_timer1(void)
{
    // Initialize EVA Timer 1:
    // Setup Timer 1 Registers (EV A)
    EvaRegs.GPTCONA.all = 0;
   
    // Set the Period for the GP timer 1 to 0x0200;
    EvaRegs.T1PR = 0x0200;       // Period
    EvaRegs.T1CMPR = 0x0000;     // Compare Reg
   
    // Enable Period interrupt bits for GP timer 1
    // Count up, x128, internal clk, enable compare, use own period
    EvaRegs.EVAIMRA.bit.T1PINT = 1;
    EvaRegs.EVAIFRA.bit.T1PINT = 1;

    // Clear the counter for GP timer 1
    EvaRegs.T1CNT = 0x0000;
    EvaRegs.T1CON.all = 0x1742;

    // Start EVA ADC Conversion on timer 1 Period interrupt
    EvaRegs.GPTCONA.bit.T1TOADC = 2;

}

void init_eva_timer2(void)
{
    // Initialize EVA Timer 2:
    // Setup Timer 2 Registers (EV A)
    EvaRegs.GPTCONA.all = 0;
   
    // Set the Period for the GP timer 2 to 0x0200;
    EvaRegs.T2PR = 0x0400;       // Period
    EvaRegs.T2CMPR = 0x0000;     // Compare Reg
   
    // Enable Period interrupt bits for GP timer 2
    // Count up, x128, internal clk, enable compare, use own period
    EvaRegs.EVAIMRB.bit.T2PINT = 1;
    EvaRegs.EVAIFRB.bit.T2PINT = 1;

    // Clear the counter for GP timer 2
    EvaRegs.T2CNT = 0x0000;
    EvaRegs.T2CON.all = 0x1742;

    // Start EVA ADC Conversion on timer 2 Period interrupt
    EvaRegs.GPTCONA.bit.T2TOADC = 2;
}

void init_evb_timer3(void)
{
    // Initialize EVB Timer 3:
    // Setup Timer 3 Registers (EV B)
    EvbRegs.GPTCONB.all = 0;
   
    // Set the Period for the GP timer 3 to 0x0200;
    EvbRegs.T3PR = 0x0800;       // Period
    EvbRegs.T3CMPR = 0x0000;     // Compare Reg
   
    // Enable Period interrupt bits for GP timer 3
    // Count up, x128, internal clk, enable compare, use own period
    EvbRegs.EVBIMRA.bit.T3PINT = 1;
    EvbRegs.EVBIFRA.bit.T3PINT = 1;

    // Clear the counter for GP timer 3
    EvbRegs.T3CNT = 0x0000;
    EvbRegs.T3CON.all = 0x1742;

    // Start EVA ADC Conversion on timer 3 Period interrupt
    EvbRegs.GPTCONB.bit.T3TOADC = 2;
}

void init_evb_timer4(void)
{
    // Initialize EVB Timer 4:
    // Setup Timer 4 Registers (EV B)
    EvbRegs.GPTCONB.all = 0;
   
    // Set the Period for the GP timer 4 to 0x0200;
    EvbRegs.T4PR = 0x1000;       // Period
    EvbRegs.T4CMPR = 0x0000;     // Compare Reg
   
    // Enable Period interrupt bits for GP timer 4
    // Count up, x128, internal clk, enable compare, use own period
    EvbRegs.EVBIMRB.bit.T4PINT = 1;
    EvbRegs.EVBIFRB.bit.T4PINT = 1;

    // Clear the counter for GP timer 4
    EvbRegs.T4CNT = 0x0000;
    EvbRegs.T4CON.all = 0x1742;

    // Start EVA ADC Conversion on timer 4 Period interrupt
    EvbRegs.GPTCONB.bit.T4TOADC = 2;
} 
 
// This ISR MUST be executed from RAM as it will put the Flash into Sleep
interrupt void eva_timer1_isr(void)
{
    Uint16 i;

    // Put the Flash to sleep
    FlashRegs.FPWR.bit.PWR = FLASH_SLEEP; 
    
    EvaTimer1InterruptCount++;
    // Short Delay to simulate some ISR Code
    for(i = 1; i < 0x03FF; i++) {}

    // Enable more interrupts from this timer
    EvaRegs.EVAIMRA.bit.T1PINT = 1;

    // Note: To be safe, use a mask value to write to the entire
    // EVAIFRA register.  Writing to one bit will cause a read-modify-write
    // operation that may have the result of writing 1's to clear 
    // bits other then those intended. 
    EvaRegs.EVAIFRA.all = BIT7;

    // Acknowledge interrupt to receive more interrupts from PIE group 2
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

// This ISR MUST be executed from RAM as it will put the Flash into Standby
interrupt void eva_timer2_isr(void)
{
    Uint16 i;

    // Put the Flash into standby
    FlashRegs.FPWR.bit.PWR = FLASH_STANDBY; 

    EvaTimer2InterruptCount++;

    // Short Delay to simulate some ISR Code
    for(i = 1; i < 0x02FF; i++) {}

    // Enable more interrupts from this timer
    EvaRegs.EVAIMRB.bit.T2PINT = 1;

   // Note: To be safe, use a mask value to write to the entire
   // EVAIFRB register.  Writing to one bit will cause a read-modify-write
   // operation that may have the result of writing 1's to clear 
   // bits other then those intended. 
   EvaRegs.EVAIFRB.all = BIT0;

   // Acknowledge interrupt to recieve more interrupts from PIE group 3
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

// This ISR MUST be executed from RAM as it will put the Flash to sleep
// and then later move it to Standby
interrupt void evb_timer3_isr(void)
{

    Uint16 i;

    // Put the Flash to sleep
    FlashRegs.FPWR.bit.PWR = FLASH_SLEEP; 

    EvbTimer3InterruptCount++;

    // Short Delay to simulate some ISR Code
    for(i = 1; i < 0x01FF; i++) {}

    // Sometime later, move the Flash to standby
    FlashRegs.FPWR.bit.PWR = FLASH_STANDBY; 

    // Note: To be safe, use a mask value to write to the entire
    // EVBIFRA register.  Writing to one bit will cause a read-modify-write
    // operation that may have the result of writing 1's to clear 
    // bits other then those intended. 
    EvbRegs.EVBIFRA.all = BIT7;

    // Acknowledge interrupt to receive more interrupts from PIE group 4
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

}

// This ISR will be executed out of Flash 
interrupt void evb_timer4_isr(void)
{

    Uint16 i;
    
    EvbTimer4InterruptCount++;

    // Short Delay to simulate some ISR Code
    for(i = 1; i < 0x00FF; i++) {}
 
    // Note: To be safe, use a mask value to write to the entire
    // EVBIFRB register.  Writing to one bit will cause a read-modify-write
    // operation that may have the result of writing 1's to clear 
    // bits other then those intended. 
    EvbRegs.EVBIFRB.all = BIT0;	

    // Acknowledge interrupt to recieve more interrupts from PIE group 5
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;

}


//===========================================================================
// No more.
//===========================================================================
