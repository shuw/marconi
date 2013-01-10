//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Definitions for BurstRx controller.
//
//******************************************************************************

#ifndef __BURST_RX_H__
#define __BURST_RX_H__

#define BURST_RX_BUFFER_LENGTH      32
#define BURST_RX_READ_LENGTH_MASK   ((WORD)(BURST_RX_BUFFER_LENGTH << 4) - 1) 
#define BURST_RX_READ_BIT_MASK      (BITS_IN_WORD - 1)
#define BURST_RX_READ_WORD_SHIFT    4
#define BURST_RX_WRITE_LENGTH_MASK  (BURST_RX_BUFFER_LENGTH - 1)
#define BURST_RX_REQUIRED_WORDS     (BURST_TOTAL_BITS / BITS_IN_WORD + 2)

#define SIZEOF_BURST_RX_BUFFER  16


typedef struct {
    WORD    buffer[BURST_RX_BUFFER_LENGTH];
    WORD    readBit;
    WORD    writeWord;
} BURST_RX_STATE_TYPE;


void BurstRxInit(BYTE *blockInitStruct, BYTE *blockObjStruct);
WORD BurstRx(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct);
WORD BurstRxReady(WORD *inSize, WORD *outSize, BYTE *blockObjStruct);

#endif
