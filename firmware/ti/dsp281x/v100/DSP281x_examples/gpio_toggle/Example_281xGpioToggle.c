//###########################################################################
//
// FILE:    Example_281xGpioToggle.c
//
// TITLE:   DSP281x Device GPIO toggle test program. 
//
// ASSUMPTIONS:
//
//          This program requires the DSP281x V1.00 header files.  
//          As supplied, this project is configured for "boot to H0" operation. 
//
//          Other then boot mode pin configuration, no other hardware configuration
//          is required.
//
//          Three different examples are included. Select the example 
//          (data, set/clear or toggle) to execute before compiling using
//          the #define statements found at the top of the code.   
//
// DESCRIPTION:
//
//          Toggle all of the GPIO PORT pins 
//        
//          The pins can be observed using Oscilloscope.  
// 
//
//###########################################################################
//
// Original by S.S.
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
// 1.00 | 11 Sep 2003 | L.H. | First Release
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Select the example to compile in.  Only one example should be set as 1
// the rest should be set as 0.
#define EXAMPLE1 1  // Use DATA registers to toggle I/O's
#define EXAMPLE2 0  // Use SET/CLEAR registers to toggle I/O's
#define EXAMPLE3 0  // Use TOGGLE registers to toggle I/O's


// Prototype statements for functions found within this file.
void delay_loop(void);
void Gpio_select(void);
void Gpio_example1(void);
void Gpio_example2(void);
void Gpio_example3(void);

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
 
// For this example use the following configuration:
   Gpio_select();	  

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
	
// Step 5. User specific code:
	
#if EXAMPLE1

    // This example uses DATA registers to toggle I/O's
    Gpio_example1();

#endif  // - EXAMPLE1

#if EXAMPLE2

    // This example uses SET/CLEAR registers to toggle I/O's
    Gpio_example1();
    
#endif

#if EXAMPLE3

    // This example uses TOGGLE registers to toggle I/O's
    Gpio_example3();
    
#endif

} 	

void delay_loop()
{
    short      i;
    for (i = 0; i < 1000; i++) {}
}


void Gpio_example1(void)
{ 
   // Example 1:
   // Toggle I/Os using DATA registers
   // Note: When using the DATA reigsters, input values
   // may be lost.  If there are inputs on the port then
   // use the CLEAR/SET/TOGGLE registers instead. 
   while(1)
   {    
       GpioDataRegs.GPADAT.all    =0xAAAA;   
       GpioDataRegs.GPBDAT.all    =0xAAAA;     
       GpioDataRegs.GPDDAT.all    =0x0022;    
       GpioDataRegs.GPEDAT.all    =0x0002; 
       GpioDataRegs.GPFDAT.all    =0xAAAA;    
       GpioDataRegs.GPGDAT.all    =0x0020; 	   			  
       delay_loop();
    
       GpioDataRegs.GPADAT.all    =0x5555;     			
       GpioDataRegs.GPBDAT.all    =0x5555;     
       GpioDataRegs.GPDDAT.all    =0x0041;    // Four I/Os only
       GpioDataRegs.GPEDAT.all    =0x0005;    // ThreeI/Os only
       GpioDataRegs.GPFDAT.all    =0x5555;    
       GpioDataRegs.GPGDAT.all    =0x0010;    // Two  I/Os only
       delay_loop();	
    }
}

void Gpio_example2(void)
{ 
   // Example 2:
   // Toggle I/Os using SET/CLEAR registers
   while(1)
   {    
       GpioDataRegs.GPASET.all    =0xAAAA;
       GpioDataRegs.GPACLEAR.all  =0x5555;     			   
       
       GpioDataRegs.GPBSET.all    =0xAAAA;
       GpioDataRegs.GPBCLEAR.all  =0x5555;     
                   
       GpioDataRegs.GPDSET.all    =0x0022;
       GpioDataRegs.GPDCLEAR.all  =0x0041;    // Four I/Os only

       GpioDataRegs.GPESET.all    =0x0002;
       GpioDataRegs.GPECLEAR.all  =0x0005;    // ThreeI/Os only
               
       GpioDataRegs.GPFSET.all    =0xAAAA;
       GpioDataRegs.GPFCLEAR.all  =0x5555;    
                  
       GpioDataRegs.GPGSET.all    =0x0020;
       GpioDataRegs.GPGCLEAR.all  =0x0010;    // Two  I/Os only
               	   			  
       delay_loop();
       
       GpioDataRegs.GPACLEAR.all  =0xAAAA;
       GpioDataRegs.GPASET.all    =0x5555;     			   
       
       GpioDataRegs.GPBCLEAR.all  =0xAAAA;
       GpioDataRegs.GPBSET.all    =0x5555;     
                   
       GpioDataRegs.GPDCLEAR.all  =0x0022;
       GpioDataRegs.GPDSET.all    =0x0041;    // Four I/Os only

       GpioDataRegs.GPECLEAR.all  =0x0002;
       GpioDataRegs.GPESET.all    =0x0005;    // ThreeI/Os only
               
       GpioDataRegs.GPFCLEAR.all  =0xAAAA;
       GpioDataRegs.GPFSET.all    =0x5555;    
                  
       GpioDataRegs.GPGCLEAR.all  =0x0020;
       GpioDataRegs.GPGSET.all    =0x0010;    // Two  I/Os only        	   			      

       delay_loop();	
    }
}

void Gpio_example3(void)
{ 
   // Example 2:
   // Toggle I/Os using TOGGLE registers

   // Set pins to a known state
   GpioDataRegs.GPASET.all    =0xAAAA;
   GpioDataRegs.GPACLEAR.all  =0x5555;     			   
       
   GpioDataRegs.GPBSET.all    =0xAAAA;
   GpioDataRegs.GPBCLEAR.all  =0x5555;     
   
   GpioDataRegs.GPDSET.all    =0x0022;
   GpioDataRegs.GPDCLEAR.all  =0x0041;    // Four I/Os only

   GpioDataRegs.GPESET.all    =0x0002;
   GpioDataRegs.GPECLEAR.all  =0x0005;    // ThreeI/Os only
               
   GpioDataRegs.GPFSET.all    =0xAAAA;
   GpioDataRegs.GPFCLEAR.all  =0x5555;    
                  
   GpioDataRegs.GPGSET.all    =0x0020;
   GpioDataRegs.GPGCLEAR.all  =0x0010;    // Two  I/Os only
   
   // Use TOGGLE registers to flip the state of
   // the pins. 
   // Any bit set to a 1 will flip state (toggle)
   // Any bit set to a 0 will not toggle.   
   while(1)
   {    
       GpioDataRegs.GPATOGGLE.all = 0xFFFF;
       GpioDataRegs.GPBTOGGLE.all = 0xFFFF;
       GpioDataRegs.GPDTOGGLE.all = 0xFFFF;
       GpioDataRegs.GPETOGGLE.all = 0xFFFF;
       GpioDataRegs.GPFTOGGLE.all = 0xFFFF;
       GpioDataRegs.GPGTOGGLE.all = 0xFFFF;
               	   			  
       delay_loop();
    }
}



void Gpio_select(void)
{

    Uint16 var1;
    Uint16 var2;
    Uint16 var3;

    var1= 0x0000;		// sets GPIO Muxs as I/Os
    var2= 0xFFFF;		// sets GPIO DIR as outputs
    var3= 0x0000;		// sets the Input qualifier values
   
    EALLOW;
	 
	GpioMuxRegs.GPAMUX.all=var1;
    GpioMuxRegs.GPBMUX.all=var1;   
    GpioMuxRegs.GPDMUX.all=var1;
    GpioMuxRegs.GPFMUX.all=var1;		 
    GpioMuxRegs.GPEMUX.all=var1; 
    GpioMuxRegs.GPGMUX.all=var1;
										
    GpioMuxRegs.GPADIR.all=var2;		// GPIO PORTs  as output
    GpioMuxRegs.GPBDIR.all=var2;   		// GPIO DIR select GPIOs as output 
    GpioMuxRegs.GPDDIR.all=var2;
    GpioMuxRegs.GPEDIR.all=var2;		
    GpioMuxRegs.GPFDIR.all=var2; 
    GpioMuxRegs.GPGDIR.all=var2;

    GpioMuxRegs.GPAQUAL.all=var3;  		// Set GPIO input qualifier values
    GpioMuxRegs.GPBQUAL.all=var3;   
    GpioMuxRegs.GPDQUAL.all=var3;
    GpioMuxRegs.GPEQUAL.all=var3;
 
    EDIS;
     
}     
//===========================================================================
// No more.
//===========================================================================

