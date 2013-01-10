// Test driver for DSP routines 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DSP.h"

#include "HwInterface.h"
#include "PrintMsg.h"
#include "DSPTestVectors.h"

#define BUFFER_SIZE 256

static BOOL CompareVectors(const SWORD *x1, const SWORD *x2, WORD length);
static void TestDSPMult(void);
static void TestDSPConv(void);
static void TestDSPMagSq(void);


DWORD ConvASM(const SWORD *x, const SWORD *h, WORD length);

//#define ISHW

#if defined ISHW
void WaitForKey(HW_UART_ID_TYPE uart)
{
    char dummy;

    while(HwUartRdAvail(uart) == 0);
    HwUartRd(uart, &dummy, 1);
}
#endif


int main(void)
{
    //SWORD x[] = {32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767};
    //SWORD x[] = {32767, 32767, 32767, 32767, 32767, 32767, 32767, 32767};
    SWORD x[] = {10, 1, 1};
    SWORD h[] = {1, 1, 40};
//    SDWORD result;
    
    /*
    char buffer[80];
    volatile WORD count;
    DWORD counter = 4000UL * 10000UL;

#if defined ISHW
	HwInit();
    
    HwUartOpen(HW_UART_ID_B, 115200, HW_UART_PARITY_NONE);
    WaitForKey(HW_UART_ID_B);

    DebugMsg("*********************\r\n");
    DebugMsg("DSP tester\r\n");
    DebugMsg("*********************\r\n");


#endif

    // Delay for serial ports
    //while(counter-- > 0) count = counter;


    result = ConvASM(x, &h[2], 3);

#if defined ISHW
	sprintf(buffer, "Result = %lu\r\n", result); 
    DebugMsg(buffer);
    while(1);
#else
	printf("Result = %lu\n", result); 
#endif
*/

    //TestDSPMult();
    TestDSPConv();
    //TestDSPMagSq();
}


static void TestDSPMult(void)
{
    WORD i;
    WORD length;
    const SWORD *xPtr = dspMultTestXVectors;
    const SWORD *yPtr = dspMultTestYVectors;
    const SWORD *zPtr = dspMultTestZVectors;
    const WORD *shiftPtr = dspMultTestShiftVectors;
    SWORD buffer[BUFFER_SIZE];
    BOOL passed = TRUE;

    for(i = 0; i < DSPMULTTEST_COUNT; ++i) {
        length = dspMultTestZLengths[i];
        if(length > BUFFER_SIZE) {
            printf("TestDSPMult: Buffer too small\n");
            passed = FALSE;
            break;
        }
        DSPMult(xPtr, yPtr, buffer, length, shiftPtr[i]);

        passed &= CompareVectors(buffer, zPtr, length);

        xPtr += length;
        yPtr += length;
        zPtr += length;
        shiftPtr += dspMultTestShiftLengths[i];
    }

    if(passed) {
        printf("TestDSPMult: PASSED\n");
    }
    else {
        printf("TestDSPMult: FAILED\n");
    }
}


static void TestDSPConv(void)
{
    WORD i;
    WORD xLength;
    WORD hLength;
    WORD zLength;
    const SWORD *xPtr = dspConvTestXVectors;
    const SWORD *hPtr = dspConvTestHVectors;
    const SWORD *zPtr = dspConvTestZVectors;
    const WORD *shiftPtr = dspConvTestShiftVectors;
    SWORD buffer[BUFFER_SIZE];
    BOOL passed = TRUE;

    for(i = 0; i < DSPCONVTEST_COUNT; ++i) {
        xLength = dspConvTestXLengths[i];
        hLength = dspConvTestHLengths[i];
        zLength = dspConvTestZLengths[i];

        if(zLength > BUFFER_SIZE) {
            printf("TestDSPConv: Buffer too small\n");
            passed = FALSE;
            break;
        }
        DSPConv(xPtr, hPtr, hLength, buffer, zLength, shiftPtr[i]); 

        passed &= CompareVectors(buffer, zPtr, zLength);

        xPtr += xLength;
        hPtr += hLength;
        zPtr += zLength;
        shiftPtr += dspConvTestShiftLengths[i];
    }

    if(passed) {
        printf("TestDSPConv: PASSED\n");
    }
    else {
        printf("TestDSPConv: FAILED\n");
    }

}


static void TestDSPMagSq(void)
{
    WORD i;
    WORD length;
    const SWORD *xiPtr = dspMagSqTestXiVectors;
    const SWORD *xqPtr = dspMagSqTestXqVectors;
    const SWORD *zPtr = dspMagSqTestZVectors;
    const WORD *shiftPtr = dspMagSqTestShiftVectors;
    SWORD buffer[BUFFER_SIZE];
    BOOL passed = TRUE;

    for(i = 0; i < DSPMAGSQTEST_COUNT; ++i) {
        length = dspMagSqTestZLengths[i];
        if(length > BUFFER_SIZE) {
            printf("TestDSPMagSq: Buffer too small\n");
            passed = FALSE;
            break;
        }
        DSPMagSq(xiPtr, xqPtr, buffer, length, shiftPtr[i]);

        passed &= CompareVectors(buffer, zPtr, length);

        xiPtr += length;
        xqPtr += length;
        zPtr += length;
        shiftPtr += dspMagSqTestShiftLengths[i];
    }

    if(passed) {
        printf("TestDSPMagSq: PASSED\n");
    }
    else {
        printf("TestDSPMagSq: FAILED\n");
    }
}


static BOOL CompareVectors(const SWORD *x1, const SWORD *x2, WORD length)
{
    WORD i;
    BOOL same = TRUE;

    for(i = 0; i < length; ++i) {
        same &= x1[i] == x2[i];
        printf("%d, %d\n", x1[i], x2[i]);
    }

    return same;
}
