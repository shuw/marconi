//###########################################################################
//
// FILE:	Example_281xMCBSP_FFDLB.c
//
// TITLE:	DSP281x Device McBSP Digital Loop Back program. 
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
//          Digital loopback tests for the McBSP peripheral.
//
//          Three different serial word sizes can be tested.  
// 
//          Before compiling this project:
//          * Select the serial word size (8/16/32) by using
//            the #define statements at the beginning of the code.
//
//          * Select the FIFO level by using the #define statement 
//            at the beginning of the code. 
//		
//          This example does not use interrupts.  Instead a polling method
//          is used to check the level of the receive FIFO.  The incoming
//          data is checked for accuracy.  If an error is found the error()
//          function is called and execution stops. 
//
//          This program will execute until terminated by the user. 
//
//###########################################################################
//
// Original Author: S.S.
// 
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | Major cleanup of the example
//###########################################################################



#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Define the level of the FIFO 1-16
#define FIFO_LEVEL 1

// Choose a word size.  Uncomment one of the following lines
#define WORD_SIZE    8    // Run a loopback test in 8-bit mode 
//#define WORD_SIZE 16      // Run a loopback test in 16-bit mode 
//#define WORD_SIZE 32        // Run a loopback test in 32-bit mode 


// Prototype statements for functions found within this file.
void delay_loop(void);
void init_mcbsp_dlb(void);
void init_mcbsp_8bit(void);
void init_mcbsp_16bit(void);
void init_mcbsp_32bit(void);
void mcbsp_xmit(int a, int b);
void mcbsp_fifo_init(void);	
void error(void);

// Global data for this example
Uint16 sdata1 = 0x000;    // Sent Data
Uint16 rdata1 = 0x000;    // Recieved Data

Uint16 sdata2 = 0x000;    // Sent Data
Uint16 rdata2 = 0x000;    // Recieved Data

Uint16 rdata1_point;
Uint16 rdata2_point;


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
// For this example, only enable the GPIO for McBSP 
   EALLOW;
   GpioMuxRegs.GPFMUX.all=0x7E00;    // Select GPIOs to be McBSP pins     
                                     // Port F MUX - x111 1110 0000 0000
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
// InitPeripherals();     // Not required for this example
   mcbsp_fifo_init();     // Initialize the Mcbsp FIFO
   init_mcbsp_dlb();      // Initalize the Mcbsp in loopback test mode
    
// Step 5. User specific code, enable interrupts:

      if(WORD_SIZE == 8)             // Run a loopback test in 8-bit mode     
      {
          init_mcbsp_8bit();
          sdata2 = 0x0000;           // value is a don't care for 8-bit mode
          sdata1 = 0x0000;           // 8-bit value to send
          rdata1_point = sdata1;      
          while(1) 
          {
             for(i = 1; i <= FIFO_LEVEL; i++)
             {
                 mcbsp_xmit(sdata1,sdata2);
                 sdata1++;
                 sdata1 = sdata1 & 0x00FF;                     // Keep it to 8-bits
             } 
             while(McbspaRegs.MFFRX.bit.RXFFST != FIFO_LEVEL ) { }  // Check for receive
             for(i = 1; i <= FIFO_LEVEL; i++)
             {
                 rdata1 = McbspaRegs.DRR1.all;                  // read DRR1 
                 if(rdata1 != rdata1_point) error();
                 rdata1_point++;
                 rdata1_point = rdata1_point & 0x00FF;         // Keep it to 8-bits
             }
             asm("    nop");                                    // Good place for a breakpoint
          }
       }      
          

      if(WORD_SIZE == 16)            // Run a loopback test in 16-bit mode     
      {
          init_mcbsp_16bit();
          sdata2 = 0x0000;           // value is a don't care for 16-bit mode
          sdata1 = 0x0000;           // 16-bit value to send
          rdata1_point = sdata1;      
          while(1) 
          {
             for(i = 1; i <= FIFO_LEVEL; i++)
             {
                 mcbsp_xmit(sdata1,sdata2);
                 sdata1++;
             } 
             while(McbspaRegs.MFFRX.bit.RXFFST != FIFO_LEVEL ) { }  // Check for receive
             for(i = 1; i <= FIFO_LEVEL; i++)
             {
                 rdata1 = McbspaRegs.DRR1.all;                  // read DRR1 
                 if(rdata1 != rdata1_point) error();
                 rdata1_point++;
             }
             asm("    nop");                                    // Good place for a breakpoint
          }
       }      


      if(WORD_SIZE == 32)            // Run a loopback test in 16-bit mode     
      {
          init_mcbsp_32bit();
          sdata1 = 0x0000;
          sdata2 = 0xFFFF;
          rdata1_point = sdata1;
          rdata2_point = sdata2;
          while(1)
          {
             for(i = 1; i <= FIFO_LEVEL; i++)
             {
                 mcbsp_xmit(sdata1,sdata2);
                 sdata1++;
                 sdata2--;
             }           
             while(McbspaRegs.MFFRX.bit.RXFFST != FIFO_LEVEL ) { }  // Check for receive
             for(i = 1; i <= FIFO_LEVEL; i++)
             {
                 rdata2 = McbspaRegs.DRR2.all;
                 rdata1 = McbspaRegs.DRR1.all;                  
                 if(rdata1 != rdata1_point) error();
                 if(rdata2 != rdata2_point) error();
                 rdata1_point++;
                 rdata2_point--;
             }
             asm("    nop");                                    // Good place for a breakpoint
          
           }         

       }
}     



// Some Useful local functions
void delay_loop()
{
    long      i;
    for (i = 0; i < 1000000; i++) {}
}


void error(void)
{
    asm("     ESTOP0");  // test failed!! Stop!
    for (;;);
}

void init_mcbsp_dlb()
{
    // McBSP register settings for Digital loop back 
    McbspaRegs.SPCR2.all=0x0000;
    McbspaRegs.SPCR1.all=0x8000;
    McbspaRegs.RCR2.all=0x0;
    McbspaRegs.RCR1.all=0x0;
    McbspaRegs.XCR2.all=0x0;
    McbspaRegs.XCR1.all=0x0;
    McbspaRegs.SRGR2.all=0x200f;                      
    McbspaRegs.SRGR1.all=0x0001;
    McbspaRegs.MCR2.all=0x0;
    McbspaRegs.MCR1.all=0x0;
    McbspaRegs.PCR.all=0x00a00;
}

void init_mcbsp_8bit()
{    

    McbspaRegs.SPCR1.bit.RJUST=0;      // Right justify word 
    McbspaRegs.RCR2.bit.RCOMPAND=0;    // R/XCOMPAND with ulaw
    McbspaRegs.XCR2.bit.XCOMPAND=0;    // R/XCOMPAND with ulaw
    McbspaRegs.RCR1.bit.RWDLEN1=0;     // 8-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=0;     // 8-bit word    
    McbspaRegs.SPCR2.bit.XRST=1;       // enable XRST/RRST
    McbspaRegs.SPCR1.bit.RRST=1;
    delay_loop();   
    McbspaRegs.SPCR2.all |=0x00C0;     // Only enable FRST,GRST 
}


void init_mcbsp_16bit()
{
    McbspaRegs.SPCR1.bit.RJUST=0;       // word Rjustifed 
    McbspaRegs.RCR2.bit.RCOMPAND=00;    // No R/XCOMPAND 
    McbspaRegs.XCR2.bit.XCOMPAND=00; 
    McbspaRegs.RCR1.bit.RWDLEN1=2;      // 16-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=2;      // 16-bit word
    McbspaRegs.SPCR2.bit.XRST=1;        // enable XRST/RRST
    McbspaRegs.SPCR1.bit.RRST=1;
    delay_loop();   
    McbspaRegs.SPCR2.all |=0x00C0;      // Only enable FRST,GRST 
}

void init_mcbsp_32bit()
{
    McbspaRegs.SPCR1.bit.RJUST =0;     // Right justifed 
    McbspaRegs.RCR2.bit.RCOMPAND =00;  // No R/XCOMPAND 
    McbspaRegs.XCR2.bit.XCOMPAND =00; 
    McbspaRegs.RCR1.bit.RWDLEN1=5;     // 32-bit word
    McbspaRegs.XCR1.bit.XWDLEN1=5;     // 32-bit word
    
 //  McBSP Reset to enable 
    McbspaRegs.SPCR2.bit.XRST =1;      // enable XRST/RRST
    McbspaRegs.SPCR1.bit.RRST=1;
    delay_loop();   
    McbspaRegs.SPCR2.all |=0x00C0;     // Only enable FRST,GRST used after 
}




void mcbsp_xmit(int a, int b)
{
    McbspaRegs.DXR2.all=b;
    McbspaRegs.DXR1.all=a;
}

void mcbsp_fifo_init()                                        
{

    McbspaRegs.MFFTX.all=0x0000;
    McbspaRegs.MFFRX.all=0x001F;
    McbspaRegs.MFFCT.all=0x0;
    McbspaRegs.MFFINT.all=0x0;
    McbspaRegs.MFFST.all=0x0;  
    McbspaRegs.MFFTX.bit.MFFENA=1;         // Enable FIFO
    McbspaRegs.MFFTX.bit.TXFIFO_RESET=1;  // Enable Transmit channel
    McbspaRegs.MFFRX.bit.RXFIFO_RESET=1;  // Enable Receive channel
      
    
}  



//===========================================================================
// No more.
//===========================================================================

