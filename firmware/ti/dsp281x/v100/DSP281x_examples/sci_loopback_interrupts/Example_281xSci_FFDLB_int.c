//###########################################################################
//
// FILE:   Example_281xSci_FFDLB_int.c
//
// TITLE:  DSP281x Device SCI Digital Loop Back porgram.
// 
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
// This program is a SCI example that uses the internal loopback of
// the peripheral.  Both interrupts and the SCI FIFOs are used.
//
// A stream of data is sent and then compared to the recieved stream.
//  
// The SCI-A sent data looks like this:
// 00 01 02 03 04 05 06 07
// 01 02 03 04 05 06 07 08
// 02 03 04 05 06 07 08 09 
// ....
// FE FF 00 01 02 03 04 05
// FF 00 01 02 03 04 05 06
// etc..
//
//
// The SCI-B sent data looks like this:
// FF FE FD FC FB FA F9 F8
// FE FD FC FB FA F9 F8 F7
// FD FC FB FA F9 F8 F7 F6 
// ....
// 01 00 FF FE FD FC FB FA
// 00 FF FE FD FC FB FA F9
// etc..
//
// Both patterns are repeated forever.  
//
// Watch Variables:  
//  
//     SCI-A           SCI-B
//     ----------------------
//     sdataA          sdataB           Data being sent
//     rdataA          rdataB           Data received
//     rdata_pointA    rdata_pointB     Keep track of where we are in the datastream
//                                      This is used to check the incoming data           
//###########################################################################
// Original Source by S.D. 
// 
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | EzDSP Alpha Release
//###########################################################################


#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

#define CPU_FREQ 	150E6
#define SCI_FREQ 	100E3
#define SCI_PRD 	CPU_FREQ/(SCI_FREQ*8)

// Prototype statements for functions found within this file.
interrupt void sciaTxFifoIsr(void);
interrupt void sciaRxFifoIsr(void);
interrupt void scibTxFifoIsr(void);
interrupt void scibRxFifoIsr(void);
void scia_fifo_init(void);
void scib_fifo_init(void);
void error(void);

// Global variables
Uint16 sdataA[8];    // Send data for SCI-A
Uint16 sdataB[8];    // Send data for SCI-B
Uint16 rdataA[8];    // Received data for SCI-A
Uint16 rdataB[8];    // Received data for SCI-A
Uint16 rdata_pointA; // Used for checking the received data
Uint16 rdata_pointB;


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
// InitGpio();    
// Setup only the GP I/O only for SCI-A and SCI-B functionality
   EALLOW;
   GpioMuxRegs.GPFMUX.bit.SCITXDA_GPIOF4 = 1;
   GpioMuxRegs.GPFMUX.bit.SCIRXDA_GPIOF5 = 1; 									         
   GpioMuxRegs.GPGMUX.bit.SCITXDB_GPIOG4 = 1;
   GpioMuxRegs.GPGMUX.bit.SCIRXDB_GPIOG5 = 1; 									         
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

// Interrupts that are used in this example are re-mapped to
// ISR functions found within this file.  
   EALLOW;	// This is needed to write to EALLOW protected registers
   PieVectTable.RXAINT = &sciaRxFifoIsr;
   PieVectTable.TXAINT = &sciaTxFifoIsr;
   PieVectTable.RXBINT = &scibRxFifoIsr;
   PieVectTable.TXBINT = &scibTxFifoIsr;
   EDIS;   // This is needed to disable write to EALLOW protected registers 
 
 
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals(); // Not required for this example
   scia_fifo_init();  // Init SCI-A
   scib_fifo_init();  // Init SCI-B
	
// Step 5. User specific code, enable interrupts:
 
// Init send data.  After each transmission this data
// will be updated for the next transmission
   for(i = 0; i<8; i++)
   {
      sdataA[i] = i;
   }

   for(i = 0; i<8; i++)
   {
      sdataB[i] = 0xFF - i;
   }

   rdata_pointA = sdataA[0];
   rdata_pointB = sdataB[0]; 
 
// Enable interrupts required for this example
   PieCtrlRegs.PIECRTL.bit.ENPIE = 1;   // Enable the PIE block
   PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, INT1
   PieCtrlRegs.PIEIER9.bit.INTx2=1;     // PIE Group 9, INT2
   PieCtrlRegs.PIEIER9.bit.INTx3=1;     // PIE Group 9, INT3
   PieCtrlRegs.PIEIER9.bit.INTx4=1;     // PIE Group 9, INT4
   IER = 0x100;	// Enable CPU INT
   EINT;   

// Step 6. IDLE loop. Just sit and loop forever (optional):	
	for(;;);

} 	

void error(void)
{
    asm("     ESTOP0"); // Test failed!! Stop!
    for (;;);
}


interrupt void sciaTxFifoIsr(void)
{   
    Uint16 i;
    for(i=0; i< 8; i++)
    {
 	   SciaRegs.SCITXBUF=sdataA[i];     // Send data
	}

    for(i=0; i< 8; i++)                 //Increment send data for next cycle
    {
 	   sdataA[i] = (sdataA[i]+1) & 0x00FF;  
	}	

	SciaRegs.SCIFFTX.bit.TXINTCLR=1;	// Clear SCI Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;      // Issue PIE ACK
}

interrupt void sciaRxFifoIsr(void)
{   
    Uint16 i;
	for(i=0;i<8;i++)
	{
	   rdataA[i]=SciaRegs.SCIRXBUF.all;	 // Read data
	}
	for(i=0;i<8;i++)                     // Check received data
	{
	   if(rdataA[i] != ( (rdata_pointA+i) & 0x00FF) ) error();
	}
	rdata_pointA = (rdata_pointA+1) & 0x00FF;                                 

	SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;   // Clear Overflow flag
	SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;   // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;       // Issue PIE ack
}

void scia_fifo_init()										
{
   SciaRegs.SCICCR.all =0x0007;   // 1 stop bit,  No loopback 
                                  // No parity,8 char bits,
                                  // async mode, idle-line protocol
   SciaRegs.SCICTL1.all =0x0003;  // enable TX, RX, internal SCICLK, 
                                  // Disable RX ERR, SLEEP, TXWAKE
   SciaRegs.SCICTL2.bit.TXINTENA =1;
   SciaRegs.SCICTL2.bit.RXBKINTENA =1;
   SciaRegs.SCIHBAUD = 0x0000;
   SciaRegs.SCILBAUD = SCI_PRD;
   SciaRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back  
   SciaRegs.SCIFFTX.all=0xC028;
   SciaRegs.SCIFFRX.all=0x0028;
   SciaRegs.SCIFFCT.all=0x00;

   SciaRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset 
   SciaRegs.SCIFFTX.bit.TXFIFOXRESET=1;
   SciaRegs.SCIFFRX.bit.RXFIFORESET=1;	 
     									
     									    									    									
}  

interrupt void scibTxFifoIsr(void)
{       
    Uint16 i;
    for(i=0; i< 8; i++)
    {
 	   ScibRegs.SCITXBUF=sdataB[i];     // Send data
	}

    for(i=0; i< 8; i++)                 //Increment send data for next cycle
    {
 	   sdataB[i] = (sdataB[i]-1) & 0x00FF;  
	}	

	ScibRegs.SCIFFTX.bit.TXINTCLR=1;    // Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;      // Issue PIE ACK
}

interrupt void scibRxFifoIsr(void)
{
    Uint16 i;
	for(i=0;i<8;i++)
	{
	   rdataB[i]=ScibRegs.SCIRXBUF.all;	 // Read data
	}
	for(i=0;i<8;i++)                     // Check received data
	{
	   if(rdataB[i] != ( (rdata_pointB-i) & 0x00FF) ) error();
	}
	rdata_pointB = (rdata_pointB-1) & 0x00FF; 
	                                
	ScibRegs.SCIFFRX.bit.RXFFOVRCLR=1;  // Clear Overflow flag
	ScibRegs.SCIFFRX.bit.RXFFINTCLR=1; 	// Clear Interrupt flag
	PieCtrlRegs.PIEACK.all|=0x100;  	// Issue PIE ack
}

void scib_fifo_init()										
{
   ScibRegs.SCICCR.all =0x0007;    // 1 stop bit,  No loopback 
                                   // No parity,8 char bits,
                                   // async mode, idle-line protocol
   ScibRegs.SCICTL1.all =0x0003;   // enable TX, RX, internal SCICLK, 
                                   // Disable RX ERR, SLEEP, TXWAKE
   ScibRegs.SCICTL2.bit.TXINTENA =1;
   ScibRegs.SCICTL2.bit.RXBKINTENA =1;
   ScibRegs.SCIHBAUD    =0x0000;
   ScibRegs.SCILBAUD    =SCI_PRD;
   ScibRegs.SCICCR.bit.LOOPBKENA =1; // Enable loop back  
   ScibRegs.SCIFFTX.all=0xC028;
   ScibRegs.SCIFFRX.all=0x0028;
   ScibRegs.SCIFFCT.all=0x00;

   ScibRegs.SCICTL1.all =0x0023;     // Relinquish SCI from Reset 
   ScibRegs.SCIFFTX.bit.TXFIFOXRESET=1;
   ScibRegs.SCIFFRX.bit.RXFIFORESET=1;	 
      
}  

//===========================================================================
// No more.
//===========================================================================

