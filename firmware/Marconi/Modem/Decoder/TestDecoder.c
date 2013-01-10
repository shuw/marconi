//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		Enter Description here
//
//
//
//******************************************************************************


#define __TESTDECODER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

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
// FUNCTION:        N/A
//
// USAGE:             N/A
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
WORD TestDecoder(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	WORD outputBitPosition = 0;
	WORD inputBit;
	WORD inputByte;
	WORD outputByte = 0;
	WORD inputSoftBit;
	WORD inputBitPosition;
	WORD inputSize = *inSize;

	while (inputSize-- > 0)
	{
		*outBuffer = 0;
		inputByte = *inBuffer;

		for (inputBitPosition = 0; inputBitPosition <  BITS_IN_WORD; inputBitPosition += VITERBI_NUM_SOFT_BITS)
		{
			inputSoftBit = (inputByte >> inputBitPosition) & SOFT_LEVEL_BINARY_BIT_MASK;


			//
			// Test Decoding of Soft Bits
			//
			if (inputSoftBit >= 8)
			{
				inputBit = 0;
			}
			else
			{
				inputBit = 1;
			}

			outputByte |= inputBit << outputBitPosition++;

			if (outputBitPosition >= BITS_IN_WORD)
			{
				*outBuffer++ = outputByte;
				outputByte = 0;
				outputBitPosition = 0;
			}

		}
		inBuffer++;
	}


	return 0;
}


//******************************************************************************
//
// FUNCTION:        N/A
//
// USAGE:             N/A
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
WORD TestDecoderReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*inputSize >= TEST_DECODER_INPUT_SIZE && *outputSize >= TEST_DECODER_OUTPUT_SIZE)
	{
		*inputSize = TEST_DECODER_INPUT_SIZE;
		*outputSize = TEST_DECODER_OUTPUT_SIZE;
		return TRUE;
	}
	return FALSE;
}




/***********************************  END  ************************************/
