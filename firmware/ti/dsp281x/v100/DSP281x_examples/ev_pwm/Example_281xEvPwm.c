//###########################################################################
//
// FILE:   Example_281xEvPwm.c
//
// TITLE:  DSP281x Event Manager PWM Generation.
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
//          This program sets up the EV timers (TIMER1, TIMER2, TIMER3 and TIMER4) 
//          to generate T1PWM, T2PWM, T3PWM, T4PWM and PWM1-12 waveforms.  
//          The user can then observe the waveforms using an scope.  
//          
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
void init_eva(void);
void init_evb(void);

// Global counts used in this example


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

// Initialize only GPAMUX and GPBMUX for this test
   EALLOW;
   // Enable PWM pins
   GpioMuxRegs.GPAMUX.all = 0x00FF; // EVA PWM 1-6  pins
   GpioMuxRegs.GPBMUX.all = 0x00FF; // EVB PWM 7-12 pins
   EDIS;
    
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
   
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   init_eva();
   init_evb();

// Step 5. User specific code, enable interrupts:


  // Just sit and loop forever:
  // PWM pins can be observed with a scope.	
  for(;;);

}

void init_eva()
{

// EVA Configure T1PWM, T2PWM, PWM1-PWM6 
// Initalize the timers
   // Initalize EVA Timer1 
   EvaRegs.T1PR = 0xFFFF;       // Timer1 period
   EvaRegs.T1CMPR = 0x3C00;     // Timer1 compare
   EvaRegs.T1CNT = 0x0000;      // Timer1 counter
   // TMODE = continuous up/down
   // Timer enable
   // Timer compare enable
   EvaRegs.T1CON.all = 0x1042;   


  // Initalize EVA Timer2 
  EvaRegs.T2PR = 0x0FFF;       // Timer2 period
  EvaRegs.T2CMPR = 0x03C0;     // Timer2 compare
  EvaRegs.T2CNT = 0x0000;      // Timer2 counter
  // TMODE = continuous up/down
  // Timer enable
  // Timer compare enable
  EvaRegs.T2CON.all = 0x1042;   


  // Setup T1PWM and T2PWM
  // Drive T1/T2 PWM by compare logic
  EvaRegs.GPTCONA.bit.TCMPOE = 1;
  // Polarity of GP Timer 1 Compare = Active low
  EvaRegs.GPTCONA.bit.T1PIN = 1;
  // Polarity of GP Timer 2 Compare = Active high
  EvaRegs.GPTCONA.bit.T2PIN = 2;

  // Enable compare for PWM1-PWM6
  EvaRegs.CMPR1 = 0x0C00;
  EvaRegs.CMPR2 = 0x3C00;
  EvaRegs.CMPR3 = 0xFC00;
    
  // Compare action control.  Action that takes place
  // on a cmpare event
  // output pin 1 CMPR1 - active high
  // output pin 2 CMPR1 - active low
  // output pin 3 CMPR2 - active high
  // output pin 4 CMPR2 - active low
  // output pin 5 CMPR3 - active high
  // output pin 6 CMPR3 - active low
  EvaRegs.ACTRA.all = 0x0666;
  EvaRegs.DBTCONA.all = 0x0000; // Disable deadband
  EvaRegs.COMCONA.all = 0xA600;


} 

void init_evb()
{

// EVB Configure T3PWM, T4PWM and PWM7-PWM12
// Step 1 - Initialize the Timers

  // Initialize EVB Timer3 
  // Timer3 controls T3PWM and PWM7-12
  EvbRegs.T3PR = 0xFFFF;       // Timer3 period
  EvbRegs.T3CMPR = 0x3C00;     // Timer3 compare
  EvbRegs.T3CNT = 0x0000;      // Timer3 counter
  // TMODE = continuous up/down
  // Timer enable
  // Timer compare enable
  EvbRegs.T3CON.all = 0x1042;   

  // Initialize EVB Timer4 
  // Timer4 controls T4PWM
  EvbRegs.T4PR = 0x00FF;       // Timer4 period
  EvbRegs.T4CMPR = 0x0030;     // Timer4 compare
  EvbRegs.T4CNT = 0x0000;      // Timer4 counter
  // TMODE = continuous up/down
  // Timer enable
  // Timer compare enable
  EvbRegs.T4CON.all = 0x1042;   

  // Setup T3PWM and T4PWM
  // Drive T3/T4 PWM by compare logic
  EvbRegs.GPTCONB.bit.TCMPOE = 1;
  // Polarity of GP Timer 3 Compare = Active low
  EvbRegs.GPTCONB.bit.T3PIN = 1;
  // Polarity of GP Timer 4 Compare = Active high
  EvbRegs.GPTCONB.bit.T4PIN = 2;
 
  // Enable compare for PWM7-PWM12
  EvbRegs.CMPR4 = 0x0C00;
  EvbRegs.CMPR5 = 0x3C00;
  EvbRegs.CMPR6 = 0xFC00;
    
  // Compare action control.  Action that takes place
  // on a cmpare event
  // output pin 1 CMPR4 - active high
  // output pin 2 CMPR4 - active low
  // output pin 3 CMPR5 - active high
  // output pin 4 CMPR5 - active low
  // output pin 5 CMPR6 - active high
  // output pin 6 CMPR6 - active low
  EvbRegs.ACTRB.all = 0x0666;
  EvbRegs.DBTCONB.all = 0x0000; // Disable deadband
  EvbRegs.COMCONB.all = 0xA600;
}


// No ISR's used in this example


//===========================================================================
// No more.
//===========================================================================
