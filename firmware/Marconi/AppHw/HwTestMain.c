// Interactive hardware test application

// Don't not include this!
#include <stdio.h>
#include "Dsp281x_Device.h"

#include "GlobalDefs.h"
#include "HwInterface.h"

#define MENU_BASE_CHAR  'a'

/*
const char *MAIN_MENU[] = {
    "Codec loop-back test",
    "Codec tone test",
    "Enable timer",
    "Disable timer",
    "Enable PTT",
    "Disable PTT",
    0
};

enum {
    TEST_CODEC_LOOPBACK,
    TEST_CODEC_TONE,
    TEST_TIMER_ENABLE,
    TEST_TIMER_DISABLE,
    TEST_PTT_ENABLE,
    TEST_PTT_DISABLE
};
*/


#define CHAR_PTT_ENABLE     'P'
#define CHAR_PTT_DISABLE    'p'
#define CHAR_TONE_ENABLE    'T'
#define CHAR_TONE_DISABLE   't'


static const SWORD TEST_TONE_DATA[] = {
           0,    14876,    26510,    32365,    31164,    23170,    10126,    -5126, 
      -19261,   -29197,   -32768,   -29197,   -19261,    -5126,    10126,    23170, 
       31164,    32365,    26510,    14876,        0,   -14876,   -26510,   -32365, 
      -31164,   -23170,   -10126,     5126,    19261,    29197,    32767,    29197, 
       19261,     5126,   -10126,   -23170,   -31164,   -32365,   -26510,   -14876, 
       /*
           0,    19261,    31164,    31164,    19261,        0,   -19261,   -31164, 
      -31164,   -19261,        0,    19261,    31164,    31164,    19261,        0, 
      -19261,   -31164,   -31164,   -19261,        0,    19261,    31164,    31164, 
       19261,        0,   -19261,   -31164,   -31164,   -19261,        0,    19261, 
       31164,    31164,    19261,        0,   -19261,   -31164,   -31164,   -19261, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    26510,    31164,    10126,   -19261,   -32768,   -19261,    10126, 
       31164,    26510,        0,   -26510,   -31164,   -10126,    19261,    32767, 
       19261,   -10126,   -31164,   -26510,        0,    26510,    31164,    10126, 
      -19261,   -32768,   -19261,    10126,    31164,    26510,        0,   -26510, 
      -31164,   -10126,    19261,    32767,    19261,   -10126,   -31164,   -26510, 
      */
};


#define TEST_TONE_DATA_LEN  (sizeof(TEST_TONE_DATA) / sizeof(SWORD))

/*
WORD ExecuteMenu(const char *items[])
{
    WORD i;
    char choice;
    char buffer[80];

    while(TRUE) {
        PrintMsg("Please choose from the following items:\r\n");
        
        for(i = 0; items[i] != 0; ++i) {
            sprintf(buffer, " [%c] - %s\r\n", MENU_BASE_CHAR + i, items[i]);
            PrintMsg(buffer);
        }

        while(HwUartRdAvail(HW_UART_ID_A) == 0);
        HwUartRd(HW_UART_ID_A, &choice, 1);
        choice -= MENU_BASE_CHAR;
        if(choice >= 0 && choice < i) {
            return choice;
        }
    }
}


void PrintMenuLine(char menuChar, const char *menuItem)
{
    char buffer[80];

    sprintf(buffer, " [%c] - %s\r\n", menuChar, menuItem);
    PrintMsg(buffer);
}
*/

#define BUF_SIZE 32

void WaitForKey(HW_UART_ID_TYPE uart)
{
    char dummy;

    while(HwUartRdAvail(uart) == 0);
    HwUartRd(uart, &dummy, 1);
}


void HwUartTest(void)
{
    DWORD counter = 4000UL * 10000UL;
    DWORD counterStart = 1;
    volatile WORD count;
    DWORD iteration = 0;
    //char buffer[80];
    char *buffer;
    HW_UART_ID_TYPE uart;

    while(counter-- > 0) count = counter;
    uart = HW_UART_ID_B;

    HwUartOpen(uart, 115200, HW_UART_PARITY_NONE);
    HwUartOpen(HW_UART_ID_A, 9600, HW_UART_PARITY_NONE);
    WaitForKey(uart);
    //HwCodecOpen(HW_CODEC_SAMPLE_RATE_8_KHZ);

    while(1) {
        //buffer = "Main Message from UART B\r\n";
        counter = counterStart;
        HwUartWr(uart, "12", 2);
        while(counter-- > 0);
        HwUartWr(HW_UART_ID_A, "AB", 2);
        HwGpioSetLEDs(0x1);
        while(HwUartWrAvail(uart) < 128);/* {
            counter = 1000;
            while(counter-- > 0) {
                PieCtrlRegs.PIEIFR9.bit.INTx2 = 1;
            }
        }*/
        HwGpioSetLEDs(0x0);
        counterStart = (counterStart + 1) % 5000;
        counter = 5000;
        while(counter-- > 0);
        while(HwUartWrAvail(HW_UART_ID_A) < 128);
    }
}


int main(void)
{
#if 1

	HwInit();
    HwUartTest();
    while(1);

#else 
    char buffer;
    char rxByte;
    char txByte;
    WORD error;
    volatile WORD count;
    WORD toneIndex;
    DWORD counter = 4000UL * 10000UL;
    BOOL toneEnabled = FALSE;
    WORD samples[BUF_SIZE];



    // Delay for serial ports
    while(counter-- > 0) count = counter;

	HwUartOpen(HW_UART_ID_A, 115200, HW_UART_PARITY_NONE);
    WaitForKey(HW_UART_ID_A); 

    PrintMsg("Camillo Interactive Hardware Test\r\n");

    //////////////////////////////////////////////
    // Test loopback for UART B
    //////////////////////////////////////////////

    /*
    PrintMsg("Press any key to test UART B (ensure that jumper SCIB_LPBK is set)...");
    WaitForKey(HW_UART_ID_A);

    rxByte = 0;
    txByte = 0;
    error = 0;
    count = 0;

    HwUartOpen(HW_UART_ID_B, 115200, HW_UART_PARITY_NONE);
    while(count < 2048) { 
        if(HwUartWrAvail(HW_UART_ID_B) >= 1) {
            HwUartWr(HW_UART_ID_B, &txByte, 1);
            txByte = (txByte + 1) & 0xFF;
        }
        if(HwUartRdAvail(HW_UART_ID_B) >= 1) {
            HwUartRd(HW_UART_ID_B, &buffer, 1);
            error |= buffer != rxByte;
            rxByte = (rxByte + 1) & 0xFF;
            ++count;
        }
    }
    HwUartClose(HW_UART_ID_B);

    if(!error) {
        PrintMsg("PASSED\r\n");
    }
    else {
        PrintMsg("FAILED\r\n");
    }
        
    */



    HwCodecOpen(HW_CODEC_SAMPLE_RATE_8_KHZ);
    HwCodecSetRxChannel(HW_CODEC_RX_CHANNEL_1);
    //HwCodecSetTxPGAGain(HW_CODEC_GAIN_PLUS_3_DB);





    while(1) {
        if(HwUartRdAvail(HW_UART_ID_A) > 0) {
            HwUartRd(HW_UART_ID_A, &buffer, 1);
            switch(buffer) {
                case CHAR_TONE_ENABLE:
                    toneEnabled = TRUE;
                    break;
                case CHAR_TONE_DISABLE:
                    toneEnabled = FALSE;
                    break;
                case CHAR_PTT_ENABLE:
                    PrintMsg("Enabling PTT\r\n");
                    HwGpioSetPTT(TRUE);
                    break;
                case CHAR_PTT_DISABLE:
                    PrintMsg("Disabling PTT\r\n");
                    HwGpioSetPTT(FALSE);
                    break;
            }

            PrintMenuLine(CHAR_PTT_ENABLE, "Enable PTT");
            PrintMenuLine(CHAR_PTT_DISABLE, "Disable PTT");
            PrintMenuLine(CHAR_TONE_ENABLE, "Enable test tone");
            PrintMenuLine(CHAR_TONE_DISABLE, "Disable test tone");
        }

        if(toneEnabled) {
            count = HwCodecWrAvail();
            while(count-- > 0) {
                HwCodecWr((WORD *)TEST_TONE_DATA + toneIndex, 1);
                toneIndex = (toneIndex + 1) % TEST_TONE_DATA_LEN;
            }
        }
        else {
            count = MIN(HwCodecRdAvail(), HwCodecWrAvail());

            if(HwCodecRdAvail() >= BUF_SIZE) {
                HwCodecRd(samples, BUF_SIZE);
                HwCodecWr(samples, BUF_SIZE);
            }
        }
    }
#endif
}
