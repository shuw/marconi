//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Converts individual BYTES (1 per WORD) into WORD-packed BYTES.
//
//******************************************************************************


#include "All.h"


void ByteToWordInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
}


WORD ByteToWord(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	WORD count;

	for(count = 0; count < *outSize; ++count) {
		*outBuffer++ = (*inBuffer & 0x00FF) | (*(inBuffer + 1) << 8);
        inBuffer += 2;
	}

	return 0;
}


WORD ByteToWordReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*inputSize >= 2 && *outputSize >= 1) {
        // Bitwise AND with 0xFFFE to round to even multiple of two
        *inputSize = MIN(*outputSize * BYTES_IN_WORD, *inputSize & 0xFFFE);
		*outputSize = *inputSize / BYTES_IN_WORD;
		return TRUE;
	}
	return FALSE;
}


/***********************************  END  ************************************/
