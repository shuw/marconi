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


#define __VENCODER_C__


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
static BBYTE ExclusiveOr(BBYTE inputByte);

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
BBYTE VEncoder(BBYTE *inBuffer, BBYTE *inSize, BBYTE *outBuffer, BBYTE *outSize)
{
	BBYTE inputBitPosition = NUM_OF_BITS_IN_BBYTE;
	BBYTE outputBitPosition = 0;
	BBYTE currentInputByte;
	BBYTE inputSizeCounter = *inSize;

	
	BBYTE bitHistory = 0;
	BBYTE currentInputBit;
	BBYTE outputByte = 0;

	
	while (inputSizeCounter > 0)
	{
		inputBitPosition = 0;
		currentInputByte = *inBuffer++;
		inputSizeCounter--;
		
		while (inputBitPosition < NUM_OF_BITS_IN_BBYTE)
		{
			//retrieves the current bit position
			currentInputBit = (currentInputByte >> inputBitPosition) & 0x1;

			//adds current bit to running history
			bitHistory = (bitHistory << 1) | currentInputBit;
			



			//appends output bits to output
			outputByte |= ( ViterbiEncodingAlgorithm(bitHistory) ) << outputBitPosition;
			outputBitPosition += 2;

			if (outputBitPosition >= NUM_OF_BITS_IN_BBYTE)
			{
				*outBuffer++ = outputByte;
				outputByte = 0;
				outputBitPosition = 0;
			}

			inputBitPosition++;
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
BBYTE ViterbiEncodingAlgorithm(BBYTE inputBits)
{
	BBYTE firstOutputBit = 0;
	BBYTE secondOutputBit = 0;

	//using 2 bit constraint length
	// TODO: write a more efficient exclusive or algorithm, using LUT possibly, TWU
	firstOutputBit = ExclusiveOr( (BBYTE)(inputBits & (FIRST_POLYNOMIAL)) );
	secondOutputBit = ExclusiveOr( (BBYTE)(inputBits & (SECOND_POLYNOMIAL)) );

	return (firstOutputBit | (secondOutputBit << 1));
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
BBYTE VEncoderInputSizeB(void)
{
	return (VENCODER_PACKET_SIZE / 2);
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
BBYTE VEncoderOutputSizeB(void)
{
	return (VENCODER_PACKET_SIZE);
}

//
// Make this more efficient!!!
//
static BBYTE ExclusiveOr(BBYTE inputByte)
{
	BBYTE highCount = 0;

	//
	// If there is an even number of 1's, then output is 0
	//
	while (inputByte != 0)
	{
		if (inputByte & 0x1)
		{
			highCount++;
		}
		inputByte = (inputByte >> 1) & ~(MOST_SIG_DIGIT_IN_BBYTE);
	}
	if (highCount % 2 == 0)
	{
		return 0;
	}
	return 1;
}

/***********************************  END  ************************************/
