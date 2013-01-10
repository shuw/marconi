//###########################################################################
//
// FILE:   Example_281xMCBSP_FFDLB_int.c
//
// TITLE:  DSP281x Device McBSP Digital Loop Back porgram
//
// ASSUMPTIONS:
//
//         This program requires the DSP281x V1.00 header files.  
//         As supplied, this project is configured for "boot to H0" operation.
//
//         Other then boot mode pin configuration, no other hardware configuration
//         is required.
//
// DESCRIPTION:
//
// This program is a McBSP example that uses the internal loopback of
// the peripheral.  Both interrupts and the McBSP FIFOs are used.
//
// A stream of data is sent and then compared to the recieved stream.
//  
// The sent data looks like this:
// 00 01 02 03 04 05 06 07
// 01 02 03 04 05 06 07 08
// 02 03 04 05 06 07 08 09 
// ....
// FE FF 00 01 02 03 04 05
// FF 00 01 02 03 04 05 06
// etc..
//
// This pattern is repeated forever.  
//
//###########################################################################
//
// Original Source by S.D. 
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | McBSP Interrupt Example 
//###########################################################################



#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Prototype statements for functions found within this file.
void delay_loop(void);
interrupt void mcbspTxFifoIsr(void);
interrupt void mcbspRxFifoIsr(void);
void mcbsp_init(void);	
void error(void);

// Global data variables used for this example
Uint16 sdata[8];    // Sent Data
Uint16 rdata[8];    // Recieved Data
Uint16 rdata_point; // Keep track of where we 
                    // are in the data stream


void main(void)
{     
   Uint16 i;

// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP281x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO: 
// This example function is found in the DSP281x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example  
// Setup only the GP I/O only for McBSP functionality
   EALLOW;                      // Allow access to EALLOW protected registers
   GpioMuxRegs.GPFMUX.all=0x3F00;    // Select GPIOs to be McBSP pins     
                                     // Port F MUX - x111 1110 0000 0000
   EDIS;                        // Disable access to EALLOW protected registers

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
   EALLOW;	// Allow access to EALLOW protected registers
   PieVectTable.MRINTA= &mcbspRxFifoIsr; 
   PieVectTable.MXINTA=&mcbspTxFifoIsr;
   EDIS;   // Disable access to EALLOW protected registers

// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   mcbsp_init();      // For this example, only initialize the Mcbsp


// Step 5. User specific code, enable interrupts:

// Initalize the send data buffer
   for(i=0; i<8; i++)
   {
       sdata[i]=i;
   }
    
   rdata_point = 0;

// Enable interrupts required for this example
   PieCtrlRegs.PIECRTL.bit.ENPIE = 1;   // Enable the PIE block
   PieCtrlRegs.PIEIER6.bit.INTx5=1;     // Enable PIE Group 6, INT 5
   PieCtrlRegs.PIEIER6.bit.INTx6=1;     // Enable PIE Group 6, INT 6
   IER=0x20;                            // Enable CPU INT6
   EINT;                                // Enable Global Interrupts

// Step 6. IDLE loop. Just sit and loop forever (optional):
   for(;;);
    
}     


// Step 7. Insert all local Interrupt Service Routines (ISRs) and functions here:    

// Just a long delay loop
void delay_loop()
{
    long      i;
    for (i = 0; i < 1000000; i++) {}
}


void error(void)
{
    asm("     ESTOP0"); // Test failed!! Stop!
    for (;;);
} 

void mcbsp_init()
{

//*************** RESET MCBSP
   McbspaRegs.SPCR2.bit.FRST=0; // Frame Sync generator reset
   McbspaRegs.SPCR2.bit.GRST=0; // Sample Rate generator Reset
   McbspaRegs.SPCR2.bit.XRST=0; // Transmitter reset
   McbspaRegs.SPCR1.bit.RRST=0; // Receiver reset

//*************** Initialise McBSP Registers
   // McBSP register settings for Digital loop back 
   McbspaRegs.SPCR2.all=0x0000; // XRST =0
   McbspaRegs.SPCR1.all=0x8000; // RRST =0, DLB enabled
   McbspaRegs.RCR2.all=0x1321;
   McbspaRegs.RCR1.all=0x0;
   McbspaRegs.XCR2.all=0x1321;
   McbspaRegs.XCR1.all=0x0;
    
   McbspaRegs.SRGR2.all=0x3140;                      
   McbspaRegs.SRGR1.all=0x010f;
   McbspaRegs.MCR2.all=0x0;
   McbspaRegs.MCR1.all=0x0;
   McbspaRegs.PCR.all=0x00a00;
 
   McbspaRegs.MFFTX.all=0x4028;
   McbspaRegs.MFFRX.all=0x0028;
   McbspaRegs.MFFCT.all=0x0000;
   McbspaRegs.MFFINT.all=0x0000;
   McbspaRegs.MFFST.all=0x000;

//************** Enable FIFO
   McbspaRegs.MFFTX.bit.TXFIFO_RESET=1;
   McbspaRegs.MFFRX.bit.RXFIFO_RESET=1;
   
//************* Enable Sample rate generator
   McbspaRegs.SPCR2.bit.GRST=1;
   delay_loop();   

//************ Enable TX/RX unit
   McbspaRegs.SPCR2.bit.XRST=1;
   McbspaRegs.SPCR1.bit.RRST=1;
   
//************ Frame Sync generator reset
   McbspaRegs.SPCR2.bit.FRST=1;    
}  

interrupt void mcbspTxFifoIsr(void)
{
   Uint16 i;

   for(i=0; i<8; i++) 
   {
       McbspaRegs.DXR1.all=sdata[i];
   }

// Increment the send data for the next transmit cycle
   for(i=0; i<8; i++) 
   {
       sdata[i] = sdata[i]+1;
       sdata[i] = sdata[i] & 0x00FF;
   }   

   McbspaRegs.MFFTX.bit.TXFFINT_CLEAR=1;
   PieCtrlRegs.PIEACK.all|=0x20; // Issue PIE ACK
}

interrupt void mcbspRxFifoIsr(void)
{
   Uint16 i;
  
   for(i=0; i<8; i++)
   {
       rdata[i]=McbspaRegs.DRR1.all;
   }
   
   for(i=0; i<8; i++)
   {
      if (rdata[i] != ( (rdata_point+i) & 0x00FF) ) error();
   }                                  
   rdata_point = (rdata_point+1) & 0x00FF;
 
   McbspaRegs.MFFRX.bit.RXFFOVF_CLEAR=1;   // Clear Overflow flag
   McbspaRegs.MFFRX.bit.RXFFINT_CLEAR=1;   // Clear Interrupt flag
   PieCtrlRegs.PIEACK.all|=0x20;     // Issue PIE ack
}



//===========================================================================
// No more.
//===========================================================================

