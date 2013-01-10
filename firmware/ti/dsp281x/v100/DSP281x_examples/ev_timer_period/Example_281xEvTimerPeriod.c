//###########################################################################
//
// FILE:    Example_281xEvTimerPeriod.c
//
// TITLE:   DSP281x Event Manager GP Timer example program.
//
// ASSUMPTIONS:
//
//          This program requires the DSP281x V1.00 header files.  
//          As supplied, this project is configured for "boot to H0" operation.
//
//          Other then boot mode pin configuration, no other hardware configuration
//          is required.  
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
//  1.00| 11 Sep 2003 | L.H. | No change since previous version (v.58 Alpha)
//###########################################################################


#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File


// Prototype statements for functions found within this file.
interrupt void eva_timer1_isr(void);
interrupt void eva_timer2_isr(void);
interrupt void evb_timer3_isr(void);
interrupt void evb_timer4_isr(void);
void init_eva_timer1(void);
void init_eva_timer2(void);
void init_evb_timer3(void);
void init_evb_timer4(void);

// Global counts used in this example
Uint32  EvaTimer1InterruptCount;
Uint32  EvaTimer2InterruptCount;
Uint32  EvbTimer3InterruptCount;
Uint32  EvbTimer4InterruptCount;

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

    // Initialize count values to 0
    EvaTimer1InterruptCount = 0;
    EvaTimer2InterruptCount = 0;
    EvbTimer3InterruptCount = 0;
    EvbTimer4InterruptCount = 0;
 
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
    for(;;);

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


interrupt void eva_timer1_isr(void)
{
   EvaTimer1InterruptCount++;

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

interrupt void eva_timer2_isr(void)
{
  EvaTimer2InterruptCount++;
  // Enable more interrupts from this timer
  EvaRegs.EVAIMRB.bit.T2PINT = 1;
 
  // Note: To be safe, use a mask value to write to the entire
  // EVAIFRB register.  Writing to one bit will cause a read-modify-write
  // operation that may have the result of writing 1's to clear 
  // bits other then those intended. 
  EvaRegs.EVAIFRB.all = BIT0;

  // Acknowledge interrupt to receive more interrupts from PIE group 3
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

interrupt void evb_timer3_isr(void)
{
  EvbTimer3InterruptCount++;
  // Note: To be safe, use a mask value to write to the entire
  // EVBIFRA register.  Writing to one bit will cause a read-modify-write
  // operation that may have the result of writing 1's to clear 
  // bits other then those intended. 
  EvbRegs.EVBIFRA.all = BIT7;

  // Acknowledge interrupt to receive more interrupts from PIE group 4
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;

}

interrupt void evb_timer4_isr(void)
{
   EvbTimer4InterruptCount++;
   // Note: To be safe, use a mask value to write to the entire
   // EVBIFRB register.  Writing to one bit will cause a read-modify-write
   // operation that may have the result of writing 1's to clear 
   // bits other then those intended. 
   EvbRegs.EVBIFRB.all = BIT0;

   // Acknowledge interrupt to receive more interrupts from PIE group 5
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP5;

}


//===========================================================================
// No more.
//===========================================================================
