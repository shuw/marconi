//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Receives (framed) bursts of bytes. 
//
//******************************************************************************


#include <string.h>
#include "All.h"
#include "BurstRx.h"


static WORD payloadBuffer[BURST_PAYLOAD_LENGTH];


void BurstRxInit(BYTE *blockInitStruct, BYTE *blockObjStruct) {
    BURST_RX_STATE_TYPE *state = (BURST_RX_STATE_TYPE *)blockObjStruct;
    state->readBit = 0;
    state->writeWord = 0;

    memset(state->buffer, 0, sizeof(state->buffer));
}


static DWORD ReadBits(BURST_RX_STATE_TYPE *state, WORD numBits) {
    WORD bitsLeft;
    WORD readBit = state->readBit; 
    WORD count;
    DWORD outBuffer = 0;
    WORD shift = 0;
    DWORD mask;

    while(numBits > 0) {
        bitsLeft = BITS_IN_WORD - (readBit & BURST_RX_READ_BIT_MASK);
        count = MIN(bitsLeft, numBits);
        numBits -= count;
        mask = (1UL << count) - 1;
        outBuffer |= 
            (DWORD)((state->buffer[readBit >> BURST_RX_READ_WORD_SHIFT] 
                    >> (readBit & BURST_RX_READ_BIT_MASK)) & mask) << shift;
        readBit = (readBit + count) & BURST_RX_READ_LENGTH_MASK;
        shift += count;
    }

    return outBuffer;
}


static void Increment(BURST_RX_STATE_TYPE *state, WORD increment) {
    state->readBit = (state->readBit + increment) & BURST_RX_READ_LENGTH_MASK;
}


WORD BurstRx(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct) {
    BURST_RX_STATE_TYPE *state = (BURST_RX_STATE_TYPE *)blockObjStruct;
    WORD i;
    WORD j;
    DWORD sync;
    SWORD available;
    WORD calcCheckSum;
    WORD burstLength;
    WORD burstCheckSum;

    if(*inSize > 0) {
        // Write incoming WORDs into circular buffer
        state->buffer[state->writeWord] = *inBuffer;
        state->writeWord = (state->writeWord + 1) & BURST_RX_WRITE_LENGTH_MASK;
        *inSize = 1;
    }
    *outSize = 0;

    available = (SWORD)state->writeWord - (SWORD)(state->readBit >> BURST_RX_READ_WORD_SHIFT);
    if(available < 0) {
        available += BURST_RX_BUFFER_LENGTH;
    }

    if(available >= BURST_RX_REQUIRED_WORDS) {
        for(i = 0; i < BITS_IN_WORD; ++i) {
            sync = ReadBits(state, BURST_SYNC_BITS);
            if(sync == BURST_SYNC) {
                // Found sync bits: skip over them
                Increment(state, BURST_SYNC_BITS);

                // Read payload and calculate checksum
                calcCheckSum = 0;
                for(j = 0; j < BURST_PAYLOAD_LENGTH; ++j) {
                    payloadBuffer[j] = (WORD)ReadBits(state, BURST_BYTE_BITS);
                    Increment(state, BURST_BYTE_BITS);
                    calcCheckSum ^= payloadBuffer[j];
                }

                // Read length and checksum
                burstLength = (WORD)ReadBits(state, BURST_LENGTH_BITS) + 1;
                Increment(state, BURST_LENGTH_BITS);
                burstCheckSum = (WORD)ReadBits(state, BURST_CHECKSUM_BITS);
                Increment(state, BURST_CHECKSUM_BITS);

                if(calcCheckSum == burstCheckSum) {
                    // Checksum matches: output payload
                    memcpy(outBuffer, payloadBuffer, sizeof(outBuffer[0]) * burstLength);
                    *outSize = burstLength;
                    break;
                }

                // Bad checksum: rewind (exploiting the wrap-around nature of the buffer)
                Increment(state, BURST_RX_BUFFER_LENGTH * BITS_IN_WORD - BURST_TOTAL_BITS);
            }
            Increment(state, 1);
        }
    }

    return 0;
}


WORD BurstRxReady(WORD *inSize, WORD *outSize, BYTE *blockObjStruct) {
    BURST_RX_STATE_TYPE *state = (BURST_RX_STATE_TYPE *)blockObjStruct;
    SWORD available;

    available = state->writeWord - (state->readBit >> BURST_RX_READ_WORD_SHIFT);
    if(available < 0) {
        available += BURST_RX_BUFFER_LENGTH;
    }

    if((*inSize > 0 || available >= BURST_RX_REQUIRED_WORDS) && *outSize >= BURST_PAYLOAD_LENGTH) {
        return 1;
    }

    return 0;
}
