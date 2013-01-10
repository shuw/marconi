//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Transmits (framed) bursts of bytes. 
//
//******************************************************************************


#include <string.h>
#include "All.h"
#include "BurstTx.h"
#include "BurstCommon.h"


void BurstTxInit(BYTE *blockInitStruct, BYTE *blockObjStruct) {
    BURST_TX_STATE_TYPE *state = (BURST_TX_STATE_TYPE *)blockObjStruct;

    state->state = BURST_TX_IDLE;
}


static void OutputBits(BURST_TX_STATE_TYPE *state, DWORD bits, WORD numBits) {
    state->buffer |= (bits & ((1UL << numBits) - 1)) << state->bufferCount;
    state->bufferCount += numBits;
}


WORD BurstTx(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct) {
    BURST_TX_STATE_TYPE *state = (BURST_TX_STATE_TYPE *)blockObjStruct;

    switch(state->state) {
        case BURST_TX_IDLE:
            // No burst in process
            state->txLength = 0;
            state->payloadLength = 0;
            state->checksum = 0;
            state->buffer = 0;
            state->bufferCount = 0;

            OutputBits(state, BURST_SYNC, BURST_SYNC_BITS);

            state->state = BURST_TX_TX;
            *inSize = 0;

            break;
        case BURST_TX_TX:
            if(*inSize > 0) {
                // Receiving characters, transmitting burst
                OutputBits(state, *inBuffer, BURST_BYTE_BITS);
                state->payloadLength++;
                state->checksum ^= *inBuffer;
                *inSize = 1;

                state->txLength++;
                if(state->txLength == BURST_PAYLOAD_LENGTH) {
                    state->state = BURST_TX_END;
                }
            }
            else if(IsCurrentPipelineEmptyDownStream()) {
                // No more characters when we need characters: flush with zeros
                state->state = BURST_TX_FLUSH;
            }

            break;
        case BURST_TX_FLUSH:
            // No characters remaining: flush with zeros
            OutputBits(state, 0, BURST_BYTE_BITS);

            state->txLength++;
            if(state->txLength == BURST_PAYLOAD_LENGTH) {
                state->state = BURST_TX_END;
            }
            *inSize = 0;

            break;
        case BURST_TX_END:
            // End burst with length and checksum fields 
            OutputBits(state, state->payloadLength - 1, BURST_LENGTH_BITS);
            OutputBits(state, state->checksum, BURST_CHECKSUM_BITS);

            state->state = BURST_TX_IDLE;
            *inSize = 0;

            break;
    }

    // Output a WORD if one is available: we guarantee no dangling bits are left
    // in the buffer at the end by making burst length a multiple of WORD length
    if(state->bufferCount >= BITS_IN_WORD) {
        *outBuffer = (WORD)(state->buffer & 0xFFFFUL);
        state->bufferCount -= BITS_IN_WORD;
        state->buffer >>= BITS_IN_WORD;
        *outSize = 1;
    }
    else {
        *outSize = 0;
    }

    return 0;
}


WORD BurstTxReady(WORD *inSize, WORD *outSize, BYTE *blockObjStruct) {
    BURST_TX_STATE_TYPE *state = (BURST_TX_STATE_TYPE *)blockObjStruct;

    if((*inSize > 0 || state->state != BURST_TX_IDLE) && *outSize > 0) {
        return 1;
    }

    return 0;
}
