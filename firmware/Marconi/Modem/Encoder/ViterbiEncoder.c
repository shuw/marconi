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
static WORD Parity(WORD inputByte);
static void ViterbiInitializePolynomialLUT(void);
static WORD ViterbiApplyPolynomials(WORD inputBits);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
BYTE ViterbiPolynomialLUT[ (1 << VITERBI_K)];




//******************************************************************************
//  L O C A L    V A R I A B L E S 
//******************************************************************************
#if (VITERBI_N == 2)
	#if (VITERBI_K == 3)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = 
		{
			(BIT0 | BIT2 ) ,
			(BIT0 | BIT1 | BIT2 )
		};
	#endif

	#if (VITERBI_K == 4)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = 
		{
			0xF,
			0xB
		};
	#endif
	#if (VITERBI_K == 5)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = 
		{
			0x17, 0x19 
		};
	#endif

	#if (VITERBI_K == 6)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = 
		{
			0x2F,
			0x35
		};
	#endif

	#if (VITERBI_K == 7)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = 
		{
			( BIT0 | BIT2 | BIT3 | BIT4 | BIT5 ),
			( BIT0 | BIT1 | BIT2 | BIT3 | BIT6)
		};
		
	#endif
	#if (VITERBI_K == 8)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0x9f, 0xe5 };		
	#endif
	#if (VITERBI_K == 9)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0x1af, 0x11d };		
	#endif
	#if (VITERBI_K == 15)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0x45dd, 0x69e3 };		
	#endif
#endif

#if (VITERBI_N == 3)
	#if (VITERBI_K == 3)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0x7, 0x7, 0x5 };
	#endif
	#if (VITERBI_K == 4)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0xf, 0xb, 0xd  };
	#endif
	#if (VITERBI_K == 5)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0x1f, 0x1b, 0x15 };
	#endif
	#if (VITERBI_K == 6)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0x2f, 0x35, 0x39 };
	#endif
	#if (VITERBI_K == 7)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0x4f, 0x57, 0x6d };
	#endif
	#if (VITERBI_K == 8)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0xef, 0x9b, 0xa9 };
	#endif
	#if (VITERBI_K == 9)
		static ROM WORD ViterbiPolynomials[VITERBI_N] = { 0x1ed, 0x19b, 0x127 };
	#endif

#endif



static ROM BYTE ParityTable[] = 
{ 
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 
	1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 
	0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 
	1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 
	0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 
	1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 
	0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 
	0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 
	1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 
	0, 1, 0, 1, 1, 0, 
};


//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
// FUNCTION:        Viterbi Encoder Initialization
//
// USAGE:             Initializes Viterbi Encoder Variables
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void VEncoderInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	VITERBI_ENCODER_OBJ_STRUCT *ViterbiEncoderObjStruct =
				(VITERBI_ENCODER_OBJ_STRUCT *)BlockObjStruct;
	
	ViterbiInitializePolynomialLUT();
	ViterbiEncoderObjStruct->bitHistory = VITERBI_ENCODER_START_STATE;	
	ViterbiEncoderObjStruct->totalBitsCoded = 0;
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
WORD VEncoder(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	WORD inputBitPosition = 0;
	WORD outputBitPosition = 0;
	WORD currentInputByte;
	WORD counter = *inSize;
	WORD outputByte = 0;
	VITERBI_ENCODER_OBJ_STRUCT *ViterbiEncoderObjStruct =
				(VITERBI_ENCODER_OBJ_STRUCT *)BlockObjStruct;
	WORD bitHistory; //stores a local copy of bit history
	
	bitHistory = ViterbiEncoderObjStruct->bitHistory;


	while (counter-- > 0)
	{
		currentInputByte = *inBuffer++;
		
		do
		{
			//adds current bit to running history
			bitHistory = ((bitHistory << 1) | (currentInputByte >> inputBitPosition++) & 0x1) & VITERBI_POLYNOMIAL_BIT_MASK;
			
			//appends output bits to output
			outputByte |= ( ViterbiPolynomialLUT[bitHistory] ) << outputBitPosition;
			outputBitPosition += VITERBI_N;

#ifdef PLATFORM_SIM
			ViterbiEncoderObjStruct->totalBitsCoded += VITERBI_N;			
#endif			
			if (outputBitPosition >= BITS_IN_WORD)
			{
				*outBuffer++ = outputByte;
				outputByte = ViterbiPolynomialLUT[bitHistory] >> (VITERBI_N - (outputBitPosition % BITS_IN_WORD));				
				outputBitPosition = outputBitPosition % BITS_IN_WORD;
			}
		} while (inputBitPosition < BITS_IN_WORD);

		inputBitPosition = 0;
	}

	ViterbiEncoderObjStruct->bitHistory = bitHistory;
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
static WORD ViterbiApplyPolynomials(WORD inputBits)
{
	WORD result = 0;
	WORD counter;

	for (counter = 0; counter < VITERBI_N; counter++)
	{
		result |= Parity( (WORD)(inputBits & (ViterbiPolynomials[counter]))) << counter;
	}

	return result;
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
static void ViterbiInitializePolynomialLUT(void)
{
	WORD counter;
	
	for (counter = 0; counter < (1 << VITERBI_K);  counter++)
	{
		ViterbiPolynomialLUT[counter] = (BYTE)ViterbiApplyPolynomials(counter);
	}

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
WORD VEncoderReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if ( (*inputSize >= 1) &&
		(*outputSize >= VITERBI_N) )
	{
		*inputSize = MIN(*inputSize, (*outputSize / VITERBI_N));
		*outputSize = *inputSize * VITERBI_N;
		return TRUE;
	}
	return FALSE;
}


//******************************************************************************
//
// FUNCTION:        Parity of WORD
//
// USAGE:             Uses Parity LUT for optimized parity finding efficiency
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
static WORD Parity(WORD inputByte)
{
//	inputByte ^= (inputByte >> 16);
	inputByte ^= (inputByte >> 8);

	return ParityTable[inputByte & 0xff];
}

/***********************************  END  ************************************/
