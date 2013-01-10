//###########################################################################
//
// FILE:   Example_281xSpi_FFDLB.c
//
// TITLE:  DSP281x Device Spi Digital Loop Back program. 
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
// This program is a SPI example that uses the internal loopback of
// the peripheral.  Interrupts are not used.
//
// A stream of data is sent and then compared to the recieved stream.
//  
// The sent data looks like this:
// 0000 0001 0002 0003 0004 0005 0006 0007 .... FFFE FFFF
//
// This pattern is repeated forever.  
//
//          Watch Variables:         
//                sdata - sent data
//                rdata - received data
//		
////###########################################################################		
//		
// Original Author: S.S.
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Changes since previous version (v.58 Alpha) 
//      |             |      | Cleaned up the example.
//###########################################################################


#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Prototype statements for functions found within this file.
// interrupt void ISRTimer2(void);
void delay_loop(void);
void spi_xmit(Uint16 a);
void spi_fifo_init(void);
void spi_init(void);
void error(void);

void main(void)
{
   Uint16 sdata;  // send data
   Uint16 rdata;  // received data
   
// Step 1. Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the DSP281x_SysCtrl.c file.
   InitSysCtrl();

// Step 2. Initalize GPIO: 
// This example function is found in the DSP281x_Gpio.c file and
// illustrates how to set the GPIO to it's default state.
// InitGpio();  // Skipped for this example  
// Setup only the GP I/O only for SPI functionality
   EALLOW;
   GpioMuxRegs.GPFMUX.all=0x000F;	// Select GPIOs to be SPI pins	 
 									// Port F MUX - x000 0000 0000 1111
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
   spi_fifo_init();	  // Initialize the Spi FIFO
   spi_init();		  // init SPI

// Step 5. User specific code:
// Interrupts are not used in this example. 
   sdata = 0x0000;							
   for(;;)
   {    
     // Transmit data
     spi_xmit(sdata);
     // Wait until data is received
     while(SpiaRegs.SPIFFRX.bit.RXFFST !=1) { } 			
     // Check against sent data
     rdata = SpiaRegs.SPIRXBUF;				
     if(rdata != sdata) error();
     sdata++;
   }
} 	


// Step 7. Insert all local Interrupt Service Routines (ISRs) and functions here:	

void delay_loop()
{
    long      i;
    for (i = 0; i < 1000000; i++) {}
}


void error(void)
{
    asm("     ESTOP0");						// Test failed!! Stop!
    for (;;);
}

void spi_init()
{    
	SpiaRegs.SPICCR.all =0x000F;	             // Reset on, rising edge, 16-bit char bits  
	SpiaRegs.SPICTL.all =0x0006;    		     // Enable master mode, normal phase,
                                                 // enable talk, and SPI int disabled.
	SpiaRegs.SPIBRR =0x007F;									
    SpiaRegs.SPICCR.all =0x009F;		         // Relinquish SPI from Reset   
    SpiaRegs.SPIPRI.bit.FREE = 1;                // Set so breakpoints don't disturb xmission
}

void spi_xmit(Uint16 a)
{
    SpiaRegs.SPITXBUF=a;
}    

void spi_fifo_init()										
{
// Initialize SPI FIFO registers
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;
}  

//===========================================================================
// No more.
//===========================================================================

