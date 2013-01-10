//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Definitions for BurstTx controller.
//
//******************************************************************************

#ifndef __BURST_TX_H__
#define __BURST_TX_H__

#include "GlobalDefs.h"
#include "BurstCommon.h"


typedef enum {
    BURST_TX_IDLE,
    BURST_TX_TX,
    BURST_TX_FLUSH,
    BURST_TX_END
} BURST_TX_FSM_STATE_TYPE;


typedef struct {
    BURST_TX_FSM_STATE_TYPE state;
    WORD txLength;
    WORD payloadLength;
    WORD checksum;
    DWORD buffer;
    DWORD bufferCount; 
} BURST_TX_STATE_TYPE;


#define SIZEOF_BURST_TX_BUFFER  16


void BurstTxInit(BYTE *blockInitStruct, BYTE *blockObjStruct);
WORD BurstTx(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct);
WORD BurstTxReady(WORD *inSize, WORD *outSize, BYTE *blockObjStruct);

#endif
