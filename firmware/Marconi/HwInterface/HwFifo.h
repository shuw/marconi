//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:   Implements FIFOs used to transfer data between ISRs and application
//          code. Reads and writes are fully asynchronous: either operation
//          may be interrupted by the other (via an ISR) without disrupting
//          behaviour or corrupting data.
//
//******************************************************************************

#ifndef __HWFIFO_H__
#define __HWFIFO_H__

//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "GlobalDefs.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

typedef struct
{
    WORD            *buffer;
    WORD            length;
    volatile WORD   readIndex;      // volatile: ensure we always read current value
    volatile WORD   writeIndex;     // volatile: ensure we always read current value
} HW_FIFO_TYPE;

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION:    HwFifoInit - initializes or re-initializes FIFO using a
//                  specific buffer and length.
//
//              Must be called before any other operations are performed on
//              the FIFO.
//              
//              NOTE: One word of the buffer is always wasted due to how
//              asynchronous reads and writes are supported. The buffer
//              should be one word larger than necessary. Thus HwFifoRdAvail 
//              and HwFifoWrAvail will never return a value greater than
//              length - 1.
//
// INPUT:       fifo - pointer to FIFO structure
//              buffer - pointer to array that will be used to store
//                  FIFO data (MUST be length words long)
//              length - the length (in words) of the FIFO buffer to use
//
// OUTPUT:      None
//
//******************************************************************************
static inline void HwFifoInit(HW_FIFO_TYPE *fifo, WORD *buffer, WORD length)
{
    fifo->buffer = buffer;
    fifo->length = length;
    fifo->readIndex = 0;
    fifo->writeIndex = 0;
}


//******************************************************************************
//
// FUNCTION:    HwFifoReset - resets the given FIFO to empty state. All
//                  stored data is lost.
//
// INPUT:       fifo - pointer to FIFO structure
//
// OUTPUT:      None 
//
//******************************************************************************
static inline void HwFifoReset(HW_FIFO_TYPE *fifo)
{
    fifo->readIndex = 0;
    fifo->writeIndex = 0;
}


//******************************************************************************
//
// FUNCTION:    HwFifoRdAvail - returns the number of words available for 
//                  reading via HwFifoRd. Since words may be written to the
//                  FIFO by an ISR at any time, the number of available words 
//                  may increase by the time HwFifoRd is actually called.
//
// INPUT:       fifo - pointer to FIFO structure
//
// OUTPUT:      returns - the number of words available for reading
//
//******************************************************************************
static inline WORD HwFifoRdAvail(HW_FIFO_TYPE *fifo)
{
    SDWORD count;

    // This is safe as long as a read of writeIndex or 
    // readIndex is atomic (cannot be interrupted) at the CPU 
    // instruction level
    count = (SDWORD)fifo->writeIndex - (SDWORD)fifo->readIndex;
    if(count < 0) 
    {
        count += fifo->length;
    }

    return (WORD)count;
}


//******************************************************************************
//
// FUNCTION:    HwFifoRd - reads words from the specified fifo, copying them
//                  into the buffer pointed to by data. A maximum of length 
//                  words are read. The actual number of words read, which is
//                  dependent upon the number of available words, is
//                  returned.
//
// INPUT:       fifo - pointer to FIFO structure
//              data - pointer to buffer in which to store words 
//              length - maximum number of words to read
//
// OUTPUT:      returns - the number of words actually read
//
//******************************************************************************
static inline WORD HwFifoRd(HW_FIFO_TYPE *fifo, WORD *data, WORD length)
{
    WORD count;
    WORD index;
    WORD i;

    // Adjust length based upon number of available bytes
    count = HwFifoRdAvail(fifo);
    length = MIN(count, length);
    index = fifo->readIndex;

    // Read up to end of FIFO
    count = MIN(length, fifo->length - index);
    for(i = 0; i < count; ++i) 
    {
        *data++ = fifo->buffer[index++];
    }

    if(index == fifo->length)
    {
        index = 0;
    }
        
    // Continue reading at beginning of FIFO (if necessary)
    count = length - count;
    for(i = 0; i < count; ++i) 
    {
        *data++ = fifo->buffer[index++];
    }

    // Adjust read pointer last (after data consumed!)
    fifo->readIndex = index;

    return length;
}


//******************************************************************************
//
// FUNCTION:    HwFifoWrAvail - returns the number of words that still may be 
//                  written via HwFifoWr. Since words may be read from the
//                  FIFO by an ISR at any time, the number of available words 
//                  may increase by the time HwFifoWr is actually called.
//
// INPUT:       fifo - pointer to FIFO structure
//
// OUTPUT:      returns - the number of words available for writing 
//
//******************************************************************************
static inline WORD HwFifoWrAvail(HW_FIFO_TYPE *fifo)
{
    SDWORD count;

    // This is safe as long as a read of writeIndex or 
    // readIndex is atomic (cannot be interrupted) at the CPU 
    // instruction level
    count = (SDWORD)fifo->readIndex - (SDWORD)fifo->writeIndex;
    if(count <= 0) 
    {
        count += fifo->length;
    }
    count--;

    return (WORD)count;
}


//******************************************************************************
//
// FUNCTION:    HwFifoWr - writes words to the specified fifo, copying them
//                  from the buffer pointed to by data. A maximum of length 
//                  words are written. The actual number of words written, 
//                  which is dependent upon the available space, is returned.
//
// INPUT:       fifo - pointer to FIFO structure
//              data - pointer to buffer from which to write words
//              length - maximum number of words to write 
//
// OUTPUT:      returns - the number of words actually written
//
//******************************************************************************
static inline WORD HwFifoWr(HW_FIFO_TYPE *fifo, const WORD *data, WORD length)
{
    WORD count;
    WORD index;
    WORD i;

    // Adjust length based upon number of available bytes
    count = HwFifoWrAvail(fifo);
    length = MIN(count, length);
    index = fifo->writeIndex;

    // Write up to end of FIFO
    count = MIN(length, fifo->length - index);
    for(i = 0; i < count; ++i) 
    {
        fifo->buffer[index++] = *data++;
    }

    if(index == fifo->length)
    {
        index = 0;
    }
        
    // Continue writing at beginning of FIFO (if necessary)
    count = length - count;
    for(i = 0; i < count; ++i) 
    {
        fifo->buffer[index++] = *data++;
    }

    // Adjust write pointer last (after data written!)
    fifo->writeIndex = index;

    return length;
}


//
// The following is a self test of the FIFO (excluding the
// asynchronous aspects). To run, simply compile with
// __HW_FIFO_TEST__ defined (-D__HW_FIFO_TEST__) and execute
// the resulting image.
//


#if defined(__HW_FIFO_TEST__)


#define TEST_FIFO_LEN 32


static WORD fifoBuffer[TEST_FIFO_LEN];
static WORD tempBuffer[TEST_FIFO_LEN];
static HW_FIFO_TYPE fifo;
static WORD writeAvail;
static WORD readAvail;
static WORD writeData;
static WORD readData;

static BYTE testFailed = FALSE;


void Error(char *msg)
{
    printf("ERROR: %s\r\n", msg);
    testFailed = TRUE;
}


void ResetChecker(void)
{
    writeAvail = TEST_FIFO_LEN - 1;
    readAvail = 0;
    writeData = 0x0F00;
    readData = 0x0F00;
}


void WriteAndCheck(WORD count)
{
    WORD length;
    WORD writeCount;
    WORD i;

    length = MIN(count, writeAvail);
    for(i = 0; i < length; ++i) {
        tempBuffer[i] = writeData + i;
    }

    if(HwFifoWrAvail(&fifo) != writeAvail) {
        Error("Returned write available incorrect");
    }

    writeCount = HwFifoWr(&fifo, tempBuffer, count);

    if(length != writeCount) {
        Error("Returned write count incorrect");
    }

    //printf("    writeAvail = %d, writeCount = %d, length = %d\r\n", 
        //writeAvail, writeCount, length);

    writeData += length;
    writeAvail -= length;
    readAvail += length;
}


void ReadAndCheck(WORD count)
{
    WORD length;
    WORD readCount;
    WORD i;

    length = MIN(count, readAvail);

    if(HwFifoRdAvail(&fifo) != readAvail) {
        Error("Returned read available incorrect");
    }

    readCount = HwFifoRd(&fifo, tempBuffer, count);

    if(length != readCount) {
        Error("Returned read count incorrect");
    }

    for(i = 0; i < length; ++i) {
        if(tempBuffer[i] != readData + i) {
            Error("Read returned incorrect data");
        }
    }

    //printf("    readAvail = %d, readCount = %d, length = %d\r\n", 
        //readAvail, readCount, length);

    readData += length;
    readAvail -= length;
    writeAvail += length;
}


int main(void)
{
    printf("HwFifo TEST\n");

    ResetChecker();
    HwFifoInit(&fifo, fifoBuffer, TEST_FIFO_LEN);

    // Make sure we can read/write zero
    WriteAndCheck(0);
    ReadAndCheck(0);

    // Fill and empty (wrap around to beginning)
    WriteAndCheck(TEST_FIFO_LEN - 1);
    ReadAndCheck(TEST_FIFO_LEN - 1);

    // Check read cases
    WriteAndCheck(10);
    ReadAndCheck(5);        // read < number avail
    ReadAndCheck(5);        // read = number avail
    WriteAndCheck(10);
    ReadAndCheck(11);       // read > number avail

    // Test wrap around on read and write
    WriteAndCheck(TEST_FIFO_LEN - 1);
    ReadAndCheck(TEST_FIFO_LEN - 1);

    // Check write cases
    WriteAndCheck(20);                      // write < number avail
    WriteAndCheck(TEST_FIFO_LEN - 20 - 1);  // write = number avail
    ReadAndCheck(10);
    WriteAndCheck(20);                      // write > number avail
    ReadAndCheck(TEST_FIFO_LEN);

    // Check reset
    WriteAndCheck(10);
    ResetChecker();
    HwFifoReset(&fifo);
    WriteAndCheck(10);
    ReadAndCheck(10);
  
    if(testFailed) 
    {
        printf("FAILED\r\n");
        return -1;
    }
    else
    {
        printf("PASSED\r\n");
        return 0;
    }
}


#endif // __HW_FIFO_TEST__

#endif

/***********************************  END  ************************************/
