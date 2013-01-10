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


#define __MODULATOR_C__


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
// FUNCTION:        Test Modulator, for simulation on PC
//
// USAGE:             Creates 16-level binary stream
//				
//
// INPUT:              N/A
//
// OUTPUT:           4 bit 16 level binary stream
//
// GLOBALS:         N/A
//
//******************************************************************************
WORD Modulator(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	WORD byteCounter;
	WORD bitCounter;
	WORD inputByte;
	WORD inputBit = 0;
	WORD outputBitLevel = 0;
	WORD outputForBuffer = 0;
	WORD outputBitCounter = 0;

	for (byteCounter = 0; byteCounter < *inSize; byteCounter++)
	{
		inputByte = *inBuffer++;
		
		for (bitCounter = 0; bitCounter < BITS_IN_WORD; bitCounter++)
		{
			inputBit = (inputByte >> bitCounter) & 0x001;

			if (inputBit)
			{
//				outputBitLevel = 0xFFFF >> (BITS_IN_WORD - VITERBI_NUM_SOFT_BITS);
				outputBitLevel = 0x7;
			}
			else
			{
				outputBitLevel = 0x8;
			}
			
			outputForBuffer |= (outputBitLevel) << outputBitCounter;
			outputBitCounter += VITERBI_NUM_SOFT_BITS;

			if (outputBitCounter >= BITS_IN_WORD)
			{
				outputBitCounter = 0;
				*outBuffer++ = outputForBuffer;
				outputForBuffer = 0;
			}
			
		}
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
WORD ModulatorReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*inputSize >= MODULATOR_INPUT_SIZE && *outputSize >= (MODULATOR_INPUT_SIZE * VITERBI_NUM_SOFT_BITS))
	{
		*inputSize = MODULATOR_INPUT_SIZE;
		*outputSize =  MODULATOR_INPUT_SIZE * VITERBI_NUM_SOFT_BITS;
		return TRUE;
	}
	return FALSE;
}



/***********************************  END  ************************************/
