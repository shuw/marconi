//###########################################################################
//
// FILE:  Example_281xGpio.c
//
// TITLE: DSP281x GPIO Loopback Test
//
//
//        This program requires the DSP281x V1.00 header files.  
//        As supplied, this project is configured for "boot to H0" operation. 
//
//        To work properly this test requires hardware configuration described
//        below.
//
//        In this test, 8 bits of a GPIO Port are configured as outputs
//        and 8 bits of the same port are configured as inputs. The pins 
//        configured as outputs need to be externally looped back to the
//        pins configured as inputs. The output data is read back on the 
//        input pins. These tests are repeated for various qualifier 
//        values and for GPIO ports A, B and F.
//
//        Similar tests could be written for GPIO ports D, E and G.

//        Tests for ports A, D and F are called by separate functions
//        An external loopback has to be established before a testcase for 
//        a given port is executed.

//        Refer to the .ebss space for global variables for test status 
//        If PASS_flag = 0xDOBE then all tests have passed.
//        If PASS_flag = 0xDEAD then some tests have failed.
//        Tests are self validating. If a test passes, Test_status array 
//        will have Test code 0x00xx. If this array has 0xDEAD, the test 
//        has failed.

// Test #1: 
// Configure Upper 8 bits of Port A as outputs and lower 8 bits as inputs
// Loop back bits [15:8] to bits [7:0]
// Input Qualifier = 0 (Don't set any input qualifier)
// Needs a delay between CLEAR/SET/TOGGLE instruction.
// This test is run 4 times.

// Test #2: 
// Configure Upper 8 bits of Port A as inputs and lower 8 bits as outputs
// Loop back bits [7:0] to bits [15:8]
// Input Qualifier = 0 (Don't set any input qualifier)
// Needs a delay between CLEAR/SET/TOGGLE instruction.
// This test is run 4 times.

// Test #3: 
// Configure Upper 8 bits of Port A as outputs and lower 8 bits as inputs
// Loop back bits [15:8] to bits [7:0]
// Input Qualifier, QUALVAL = 1 
// Needs a delay between CLEAR/SET/TOGGLE instruction.
// For a pass, the delay must be >= ( 6 * 2 * QUALVAL ) CPUCLK cycles
// i.e the delay must be >= 12 CPUCLK cycles
// This test is run 4 times.

// Test #4: 
// Configure Upper 8 bits of Port B as outputs and lower 8 bits as inputs
// Loop back bits [15:8] to bits [7:0]
// Input Qualifier,  QUALVAL = 2
// Needs a delay between CLEAR/SET/TOGGLE instruction.
// For a pass, the delay must be >= 24 CPUCLK cycles
// This test is run 4 times.

// Test #5: 
// Configure Upper 7 bits of Port F as outputs and lower 8 bits as inputs
// Loop back bits [14:8] to bits [6:0], also loopback bit 8 to bit 7
// Needs a delay between CLEAR/SET/TOGGLE instruction.
// This test is run 4 times.

//###########################################################################
//
// Original Author: T.N.P
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  1.00| 11 Sep 2003 | L.H. | No change since previous version (v.58 Alpha)
//###########################################################################

#include "DSP281x_Device.h"     // DSP281x Headerfile Include File
#include "DSP281x_Examples.h"   // DSP281x Examples Include File

// Prototype statements for functions found within this file.

void delay_loop(void);
void Gpio_select(void);
void error(int);
void program_stop();
void Gpio_PortA(void);
void Gpio_PortB(void);
void Gpio_PortF(void);
void Gpio_PortDEG(void);

Uint16 var1 = 0;
Uint16 var2 = 0;
Uint16 var3 = 0;
Uint16 test_count = 0;
Uint16 Test_flag = 0;
Uint16 Test_var  = 0;
Uint16 Test_status[32];

Uint16 PASS_flag = 0;

void main(void)
{

// Step 1. Initialize System Control registers, PLL, WatchDog, 
// peripheral Clocks to default state:
// This function is found in the DSP281x_SysCtrl.c file.
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
   
// Step 4. Initialize all the Device Peripherals:
// This function is found in DSP281x_InitPeripherals.c
// InitPeripherals(); // Not required for this example

// Step 5. User specific code 

   // Tests #1, #2, #3
   Gpio_PortA();
    
   // Test #4
   Gpio_PortB();
    
   // Test #5
   Gpio_PortF();

   program_stop();
    
// Step 6. IDLE loop. Just sit and loop forever (optional):    
//    for(;;);
     
}

// Step 7. Insert all local Interrupt Service Routines (ISRs) and functions:

void Gpio_PortA(void)
{    
    
// GPIO Test #1: 
// Configure Upper 8 bits of Port A as outputs and lower 8 bits as inputs    
// Loop back bits [15:8] to bits [7:0]
// Don't set any input qualifier
 
    var1= 0x0000;        // sets GPIO Muxs as I/Os
    var2= 0xFF00;        // sets GPIO 15-8 DIR as outputs, 7-0 DIR as inputs
    var3= 0x0000;        // Don't set any input qualifier
    
    Gpio_select();    
    
    test_count = 0;
    Test_status[Test_var] = 0x0001;
    Test_var++;
    
    Test_status[Test_var] = 0xD0BE;      // Set the default value of status 
                                         // to "PASSED"
        
    while (test_count < 4)               // repeat the tests 4 times
    {
       GpioDataRegs.GPACLEAR.all = 0xFF00;  // Test Clear
       asm(" RPT #5 ||NOP");                         
       Test_flag = GpioDataRegs.GPADAT.all;
    
       if ( Test_flag != 0x0000 ) error(1);
    
       GpioDataRegs.GPASET.all = 0x5500;    // Test Set
       asm(" RPT #5 ||NOP");                         
       Test_flag = GpioDataRegs.GPADAT.all;
    
       if ( Test_flag != 0x5555 ) error(1);
    
       GpioDataRegs.GPATOGGLE.all = 0xFF00; // Test Toggle 
       asm(" RPT #5 ||NOP");
       Test_flag = GpioDataRegs.GPADAT.all;
    
       if ( Test_flag != 0xAAAA ) error(1);
    
       test_count++;
    
    }

    Test_var++;
    
// End of Test #1

// GPIO Test #2: 
// Configure Upper 8 bits of Port as inputs and lower 8 bits as outputs    
// Loop back bits [7:0] to bits [15:8]
// Don't set any input qualifier
 
    var1= 0x0000;       // sets GPIO Muxs as I/Os
    var2= 0x00FF;       // sets GPIO 15-8 DIR as inputs, 7-0 DIR as outputs
    var3= 0x0000;       // Don't set any input qualifier
    
    Gpio_select();    
    
    test_count = 0;
    Test_status[Test_var] = 0x0002;
    Test_var++;
    
    Test_status[Test_var] = 0xD0BE;      // Set the default value of status 
                                         // to "PASSED"
        
    while (test_count < 4)               // repeat the tests 4 times
    {
       GpioDataRegs.GPACLEAR.all = 0x00FF;  // Test Clear
       asm(" RPT #5 ||NOP");                         
       Test_flag = GpioDataRegs.GPADAT.all;
    
       if ( Test_flag != 0x0000 ) error(1);
    
       GpioDataRegs.GPASET.all = 0x00AA;    // Test Set
       asm(" RPT #5 ||NOP");                         
       Test_flag = GpioDataRegs.GPADAT.all;
    
       if ( Test_flag != 0xAAAA ) error(1);
    
       GpioDataRegs.GPATOGGLE.all = 0x0055; // Test Toggle 
       asm(" RPT #5 ||NOP");
       Test_flag = GpioDataRegs.GPADAT.all;
    
       if ( Test_flag != 0xFFFF ) error(1);
    
       test_count++;
    
    }
    
    Test_var++;
    
// End of Test #2

// GPIO Test #3: 
// Configure Upper 8 bits of Port as outputs and lower 8 bits as inputs    
// Loop back bits [15:8] to bits [7:0]
// Set input qualifier to 0x0001
 
    var1= 0x0000;        // sets GPIO Muxs as I/Os
    var2= 0xFF00;        // sets GPIO 15-8 DIR as outputs, 7-0 as inputs.
    var3= 0x0001;        // Set input qualifier to 1
        
    Gpio_select();
    
// Test #3A: Set delay so that the input gets rejected.
// delay must be < ( 6 * 2 * QUALVAL ) CPUCLK cycles
    
    Test_status[Test_var] = 0x0003;
    Test_var++;
    
    Test_status[Test_var] = 0xD0BE;      // Set the default value of status 
                                         // to "PASSED"
    test_count = 0;
                                        
    while (test_count < 4)               // repeat the tests 4 times
    {
       GpioDataRegs.GPBSET.all = 0xFF00;
       delay_loop();

       GpioDataRegs.GPACLEAR.all = 0xFF00;  // Test Clear
       asm (" RPT #14 || NOP");             // Delay < ( 6 * 2 ) CPUCLK cycles
       Test_flag = GpioDataRegs.GPADAT.all;
       if ( Test_flag == 0x0000 ) error(1);
                            
       GpioDataRegs.GPASET.all = 0x5500;    // Test Set
       asm(" RPT #14 ||NOP");                         
       Test_flag = GpioDataRegs.GPADAT.all;
       if ( Test_flag == 0x5555 ) error(1);
    
       GpioDataRegs.GPATOGGLE.all = 0xFF00; // Test Toggle 
       asm(" RPT #14 ||NOP");
       Test_flag = GpioDataRegs.GPADAT.all;
       if ( Test_flag == 0xAAAA ) error(1);
    
       test_count++; 
    
    }
    
    Test_var++;
    
// Test #3B: Set delay so that the input is qualified successfully 
// delay must be >= ( 6 * 2 * QUALVAL ) CPUCLK cycles

    Test_status[Test_var] = 0x0004;
    Test_var++;
    
    Test_status[Test_var] = 0xD0BE;      // Set the default value of status 
                                         // to "PASSED"
    test_count = 0;
                                        
    while (test_count < 4)               // repeat the tests 4 times
    {
       GpioDataRegs.GPASET.all = 0xFF00;
       delay_loop();

       GpioDataRegs.GPACLEAR.all = 0xFF00;  // Test Clear    
       asm (" RPT #16 || NOP");             // Delay = (6 * 2 * 1) CPUCLK cycles
       Test_flag = GpioDataRegs.GPADAT.all;
       if ( Test_flag != 0x0000 ) error(1);
                            
       GpioDataRegs.GPASET.all = 0x5500;    // Test Set
       asm(" RPT #16 ||NOP");                         
       Test_flag = GpioDataRegs.GPADAT.all;
       if ( Test_flag != 0x5555 ) error(1);
    
       GpioDataRegs.GPATOGGLE.all = 0xFF00; // Test Toggle 
       asm(" RPT #16 ||NOP");
       Test_flag = GpioDataRegs.GPADAT.all;
       if ( Test_flag != 0xAAAA ) error(1);
    
       test_count++; 
    
    }
    
    Test_var++;    

} // End of Test #3


void Gpio_PortB(void)
{

// GPIO Test #4: 
// Configure Upper 8 bits of Port B as outputs and lower 8 bits as inputs    
// Loop back bits [15:8] to bits [7:0]
// Set input qualifier to 0x0002
 
    var1= 0x0000;        // sets GPIO Muxs as I/Os
    var2= 0xFF00;        // sets GPIO 15-8 DIR as outputs, 7-0 as inputs.
    var3= 0x0002;        // Set input qualifier to 2
        
    Gpio_select();
    
// Test #4A: Set delay so that the input gets rejected.    
// Delay must be < ( 6 * 2 * QUALVAL ) CPUCLK cycles

    Test_status[Test_var] = 0x0005;
    Test_var++;
    
    Test_status[Test_var] = 0xD0BE;      // Set the default value of status 
                                         // to "PASSED"
    test_count = 0;
                                        
    while (test_count < 4)               // repeat the tests 4 times
    {
       GpioDataRegs.GPBSET.all = 0xFF00;
       delay_loop();

       GpioDataRegs.GPBCLEAR.all = 0xFF00;  // Test Clear    
       asm (" RPT #24 || NOP");             // Delay < (6 * 2 * 2) CPUCLK cycles
       Test_flag = GpioDataRegs.GPBDAT.all;
       if ( Test_flag == 0x0000 ) error(1);
                            
       GpioDataRegs.GPBSET.all = 0x5500;    // Test Set
       asm(" RPT #24 ||NOP");                         
       Test_flag = GpioDataRegs.GPBDAT.all;
       if ( Test_flag == 0x5555 ) error(1);
    
       GpioDataRegs.GPBTOGGLE.all = 0xFF00; // Test Toggle 
       asm(" RPT #24 ||NOP");
       Test_flag = GpioDataRegs.GPBDAT.all;
       if ( Test_flag == 0xAAAA ) error(1);
    
       test_count++; 
    
    }
    
    Test_var++;
    
// Test #4B: Set delay so that the input is qualified successfully 
// Delay must be >= ( 6 * 2 * QUALVAL ) CPUCLK cycles

    Test_status[Test_var] = 0x0006;
    Test_var++;
    
    Test_status[Test_var] = 0xD0BE;      // Set the default value of status 
                                         // to "PASSED"
    test_count = 0;
                                        
    while (test_count < 4)               // repeat the tests 4 times
    {
       GpioDataRegs.GPBSET.all = 0xFF00;
       delay_loop();

       GpioDataRegs.GPBCLEAR.all = 0xFF00;  // Test Clear    
       asm (" RPT #29 || NOP");             // Delay = (6 * 2 * 2) CPUCLK cycles
       Test_flag = GpioDataRegs.GPBDAT.all;
       if ( Test_flag != 0x0000 ) error(1);
                            
       GpioDataRegs.GPBSET.all = 0x5500;    // Test Set
       asm(" RPT #29 ||NOP");                         
       Test_flag = GpioDataRegs.GPBDAT.all;
       if ( Test_flag != 0x5555 ) error(1);
    
       GpioDataRegs.GPBTOGGLE.all = 0xFF00; // Test Toggle 
       asm(" RPT #29 ||NOP");
       Test_flag = GpioDataRegs.GPBDAT.all;
       if ( Test_flag != 0xAAAA ) error(1);
    
       test_count++; 
    
    } 

    Test_var++;    

} // End of Test #4

void Gpio_PortF(void)
{

// GPIO Test #5: 
// Configure Upper bits [14:8] of Port F as outputs and bits [7:0] as inputs    
// Loop back bits [14:8] to bits [6:0], also loopback bit #8, to bit #7
 
    var1= 0x0000;        // sets GPIO Muxs as I/Os
    var2= 0xFF00;        // sets GPIO 14-8 DIR as outputs, 7-0 as inputs.
        
    Gpio_select();
    
    Test_status[Test_var] = 0x0007;
    Test_var++;
    
    Test_status[Test_var] = 0xD0BE;      // Set the default value of status 
                                         // to "PASSED"
    test_count = 0;
                                        
    while (test_count < 4)                  // repeat the tests 4 times
    {
       GpioDataRegs.GPFSET.all = 0xFF00;
       delay_loop();

       GpioDataRegs.GPFCLEAR.all = 0xFF00;  // Test Clear    
       asm (" RPT #6 || NOP");             
       Test_flag = GpioDataRegs.GPFDAT.all;
       if ( Test_flag != 0x0000 ) error(1);
                            
       GpioDataRegs.GPFSET.all = 0x5500;    // Test Set
       asm(" RPT #6 || NOP");                         
       Test_flag = GpioDataRegs.GPFDAT.all;
       if ( Test_flag != 0x55D5 ) error(1);
    
       GpioDataRegs.GPFTOGGLE.all = 0xFF00; // Test Toggle 
       asm(" RPT #6 || NOP");
       Test_flag = GpioDataRegs.GPFDAT.all;
       if ( Test_flag != 0x2A2A ) error(1); 
     
       test_count++; 
    
    }
    
    Test_var++;
    
} // End of Test #5


void delay_loop()
{
    short      i;
    for (i = 0; i < 1000; i++) {}
}

void Gpio_select(void)
{
     EALLOW;
     
    GpioMuxRegs.GPAMUX.all=var1;   // Configure MUXs as digital I/Os or
    GpioMuxRegs.GPBMUX.all=var1;   // peripheral I/Os
    GpioMuxRegs.GPDMUX.all=var1;
    GpioMuxRegs.GPFMUX.all=var1;         
    GpioMuxRegs.GPEMUX.all=var1; 
    GpioMuxRegs.GPGMUX.all=var1;
                                        
    GpioMuxRegs.GPADIR.all=var2;   // GPIO PORTs  as output
    GpioMuxRegs.GPBDIR.all=var2;   // GPIO DIR select GPIOs as output 
    GpioMuxRegs.GPDDIR.all=var2;
    GpioMuxRegs.GPEDIR.all=var2;        
    GpioMuxRegs.GPFDIR.all=var2; 
    GpioMuxRegs.GPGDIR.all=var2;

    GpioMuxRegs.GPAQUAL.all=var3;  // Set GPIO input qualifier values
    GpioMuxRegs.GPBQUAL.all=var3;   
    GpioMuxRegs.GPDQUAL.all=var3;
    GpioMuxRegs.GPEQUAL.all=var3;
 
    EDIS;
     
}     


void error (int ErrorFlag)
{
   PASS_flag = 0xDEAD;               // Test case failed
   Test_status[Test_var] = 0xDEAD;   // Sub-test failed
   return;
}

void program_stop()
{

//All tests are done */    
  if(PASS_flag !=0) 
       PASS_flag=0xDEAD;          // Test code exit here..
  else
       PASS_flag=0xD0BE;          // Test code exit he    
// end all tests      
   asm("     ESTOP0");            // Done!
   for(;;)     { }                // Dummy loop Wait forever
}

//===========================================================================
// No more.
//===========================================================================


