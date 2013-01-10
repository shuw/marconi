//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		Viterbi Decoder Source File
//
//
//
//******************************************************************************


#define __VDECODER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"
#include <math.h>
#include <string.h>
#include "Config.h"
//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
typedef struct
{
	WORD pathTree[VDECODER_COLUMNS_IN_TRELLIS]
		[VDECODER_ROWS_IN_TRELLIS];
	WORD pathErrors[VITERBI_NUM_OF_STATES];
	WORD nextGenerationErrorMetric[VITERBI_NUM_OF_STATES];
	WORD nextGenerationErrorMetricCleared[VITERBI_NUM_OF_STATES];
	WORD leastErrorState;	
} VDECODER_TRELLIS_STRUCT;


//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

static VDECODER_TRELLIS_STRUCT VDecoderTrellisTemp;


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static WORD ViterbiProcessTrellis(WORD *inBuffer, WORD *inSize, VDECODER_TRELLIS_STRUCT *VDecoderBuffer, WORD *viterbiPathError);

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************

#if (VITERBI_NUM_SOFT_BITS == 4)

#ifdef USE_SOFT_DECISION
/*
	static WORD	softBitLevelErrorMetricLUT[2][16] = 
	{
		{0, 1, 1, 1, 2, 2, 3, 6, 8, 10, 13, 14, 14, 15, 15, 15},
		{15, 15, 15, 14, 14, 13, 10, 8, 6, 3, 2, 2, 1, 1, 1, 0}
	};
*/

	static WORD	softBitLevelErrorMetricLUT[2][16] = 
	{
		{8, 10, 13, 14, 14, 15, 15, 15,  
		 0, 1, 1, 1, 2, 2, 3, 6, },
		{6, 3, 2, 2, 1, 1, 1, 0,
		15, 15, 15, 14, 14, 13, 10, 8 }

	};
#else
	static WORD	softBitLevelErrorMetricLUT[2][16] = 
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	};
#endif

#endif

static WORD BitPositionSwapLUT[4] = 
	{ 0x0, 0x2, 0x1, 0x3 };


//******************************************************************************
//  C O D E
//******************************************************************************
void VDecoderInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	VITERBI_DECODER_OBJ_STRUCT *VDecoderObjStruct =
				(VITERBI_DECODER_OBJ_STRUCT *)BlockObjStruct;

	WORD viterbiStateCounter;

	VDecoderObjStruct->accumPathError = 0;
	
	for (viterbiStateCounter = 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
	{
		VDecoderObjStruct->pathErrors[viterbiStateCounter] = 0x3FFF;
	}
	VDecoderObjStruct->pathErrors[VITERBI_ENCODER_START_STATE] = 0;

	VDecoderObjStruct->isPathInitialized = FALSE;
	VDecoderObjStruct->numOfBitDroppedDetected = 0;
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
WORD VDecoder(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	SWORD viterbiTreePosition = VDECODER_COLUMNS_IN_TRELLIS;
	WORD viterbiStateCounter;
	SWORD outputBitPosition;
	VITERBI_DECODER_OBJ_STRUCT *VDecoderObjStruct =
				(VITERBI_DECODER_OBJ_STRUCT *)BlockObjStruct;

	WORD *viterbiOldPathTree = 
				(WORD *)&VDecoderObjStruct->oldPathTree[VDECODER_COLUMNS_IN_TRELLIS - 1][0];
	VDECODER_TRELLIS_STRUCT *VDecoderTrellis = &VDecoderTrellisTemp;

	//
	// Process the trellis(s)
	//
	VDecoderObjStruct->accumPathError
		+= ViterbiProcessTrellis(inBuffer, inSize,VDecoderTrellis, VDecoderObjStruct->pathErrors);

	//copies path errors from trellis with lowest error
	memcpy(VDecoderObjStruct->pathErrors, VDecoderTrellis->pathErrors, 
										sizeof(VDecoderTrellis->pathErrors));
	


	//climb trellis to get to original position
	viterbiStateCounter = VDecoderTrellis->leastErrorState;
	viterbiTreePosition = VDECODER_COLUMNS_IN_TRELLIS; 

	while (viterbiTreePosition > 0)
	{
		//go up state tree
		if (viterbiStateCounter %2)
			viterbiStateCounter = VDecoderTrellis->pathTree[--viterbiTreePosition][viterbiStateCounter >> 1] >> 8;
		else
			viterbiStateCounter = VDecoderTrellis->pathTree[--viterbiTreePosition][viterbiStateCounter >> 1] & 0x00FF;

	}


	if (!VDecoderObjStruct->isPathInitialized)
	{
		VDecoderObjStruct->isPathInitialized = TRUE;
		*outSize = 0;
	}
	else
	{
		//
		// Copies path with lowest error from old trellis
		//
		viterbiTreePosition = VDECODER_COLUMNS_IN_TRELLIS;
		outputBitPosition = ((VDECODER_NUM_OF_INPUT_BITS - 1) % BITS_IN_WORD) + 1;
		outBuffer += ((VDECODER_NUM_OF_INPUT_BITS - 1) / BITS_IN_WORD);

		*outBuffer= 0;
		while ( (--viterbiTreePosition) >= 0)
		{
			if (outputBitPosition == 0)
			{
				outBuffer--;
				*outBuffer = 0;
				outputBitPosition = BITS_IN_WORD;
			}


			//the state number contains the most current bit as it is a bit history
			outputBitPosition -= 2;
			*outBuffer |= (WORD)( BitPositionSwapLUT[viterbiStateCounter & 0x3] << outputBitPosition);


			//go up state tree
			if (viterbiStateCounter %2)
				viterbiStateCounter = viterbiOldPathTree[viterbiStateCounter >> 1] >> 8;
			else
				viterbiStateCounter = viterbiOldPathTree[viterbiStateCounter >> 1] & 0x00FF;

			viterbiOldPathTree -= VITERBI_NUM_OF_STATES / 2;
		}


	}

	//
	// Copy current path to old path
	//
	memcpy(VDecoderObjStruct->oldPathTree, VDecoderTrellis->pathTree, sizeof(VDecoderTrellis->pathTree));

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
WORD VDecoderReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if ( (*inputSize >= VDECODER_INPUT_SIZE) &&
		(*outputSize >= VDECODER_OUTPUT_SIZE) )
	{
		*inputSize = VDECODER_INPUT_SIZE;
		*outputSize = VDECODER_OUTPUT_SIZE;
		return TRUE;
	}
	return FALSE;
}

//******************************************************************************
//
// FUNCTION:        Viterbi Process Trellis
//
// USAGE:             This function will use incoming bits and go create Viterbi Trellis
//				
//
// INPUT:              N/A
//
// OUTPUT:           The smallest path error in the trellis
//
// GLOBALS:         N/A
//
//******************************************************************************
#ifdef PLATFORM_HW
	#pragma CODE_SECTION(ViterbiProcessTrellis, RAM_CODE_SECTION)
#endif
static WORD ViterbiProcessTrellis(WORD *inBuffer, WORD *inSize, 
		VDECODER_TRELLIS_STRUCT *VDecoderBuffer, WORD *viterbiPathError)
{
	WORD inputBitPosition = BITS_IN_WORD;
	WORD viterbiStateCounter = 0;
	WORD currentInputByte;
	SWORD viterbiTreePosition = 0;
//	WORD firstInputSoftBit, secondInputSoftBit;
	WORD lowestErrorMetric;
	WORD childCounter;
	WORD childState;
	WORD childError;
	WORD childOutput1;
	WORD childOutput2;
	WORD counter;
	WORD inputSoftBits[VITERBI_N * NUM_OF_BITS_PER_TRELLIS_STEP];
	
	SWORD inputBitCounter = 0;
	WORD *nextGenerationErrorMetricCleared = VDecoderBuffer->nextGenerationErrorMetricCleared;
	WORD *nextGenerationErrorMetric = VDecoderBuffer->nextGenerationErrorMetric;
	WORD *viterbiPathPtr = (WORD *)VDecoderBuffer->pathTree;

	//
	// Copies previous path errors
	//
	memcpy(VDecoderBuffer->pathErrors, viterbiPathError, sizeof(VDecoderBuffer->pathErrors));
	// Modifies path errors only in buffer, does not change path errors from parameter
	viterbiPathError = VDecoderBuffer->pathErrors;

	for (viterbiStateCounter = 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
	{
		nextGenerationErrorMetricCleared[viterbiStateCounter] = 0x7FFF;
	}

	//
	// First process all input bits
	//
	while ( inputBitCounter < VDECODER_NUM_OF_INPUT_BITS)
	{
		//
		// Initialize input soft bits
		//
		for (counter = 0; counter < VITERBI_N * NUM_OF_BITS_PER_TRELLIS_STEP; counter++)
		{
			if (inputBitPosition >= BITS_IN_WORD)
			{
				inputBitPosition = 0;
				currentInputByte = *inBuffer++;
			}

			//gets two bits from the input
			inputSoftBits[counter] = (currentInputByte >> inputBitPosition) & SOFT_LEVEL_BINARY_BIT_MASK;

			inputBitPosition += VITERBI_NUM_SOFT_BITS;
		}

		//
		// Clears error metrics to allow for compare operation
		//
		memcpy(nextGenerationErrorMetric, nextGenerationErrorMetricCleared, VITERBI_NUM_OF_STATES  * sizeof(WORD));
		
		for (viterbiStateCounter = 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
		{
			for (childCounter = 0; childCounter < NUM_OF_BRANCHES_FOR_EACH_NODE; childCounter++)
			{

				//
				// Following steps are Hardcoded for 2 bits per trellis step
				//
				childState = (viterbiStateCounter << 2) | childCounter;


				childOutput1 = ViterbiPolynomialLUT[childState >> 1];
				childOutput2 = ViterbiPolynomialLUT[childState & VITERBI_POLYNOMIAL_BIT_MASK];
				
				childState &= VITERBI_STATE_BIT_MASK;
				childError = viterbiPathError[viterbiStateCounter];

				for (counter = 0; counter < VITERBI_N; counter++)
				{
					childError += softBitLevelErrorMetricLUT[(childOutput1  >> counter) & 1][inputSoftBits[counter]];
					childError += softBitLevelErrorMetricLUT[(childOutput2  >> counter) & 1][inputSoftBits[counter + 2]];
				}
				
				if ( childError < nextGenerationErrorMetric[ childState ] )
				{
					//
					// Pack 2 trellis points into each WORD
					//
					if (childState % 2)
					{
						viterbiPathPtr[childState >> 1] &= 0x00FF;
						viterbiPathPtr[childState >> 1] |= viterbiStateCounter << 8;
					}
					else
					{
						viterbiPathPtr[childState >> 1] &= 0xFF00;
						viterbiPathPtr[childState >> 1] |= viterbiStateCounter;
					}					
					nextGenerationErrorMetric[childState] =  childError;
				}
			}
			
		}
		
		//Copies calculated errors
		memcpy(viterbiPathError, nextGenerationErrorMetric, VITERBI_NUM_OF_STATES * sizeof(WORD));

		viterbiPathPtr += VDECODER_ROWS_IN_TRELLIS;
		viterbiTreePosition++;
		inputBitCounter += NUM_OF_BITS_PER_TRELLIS_STEP;
	}

	//
	// Start with the tree with the least error
	//
	lowestErrorMetric = 0x7FFF;
	for (viterbiStateCounter= 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
	{
		if (viterbiPathError[viterbiStateCounter] < lowestErrorMetric)
		{
			lowestErrorMetric = viterbiPathError[viterbiStateCounter];
			VDecoderBuffer->leastErrorState = viterbiStateCounter;
		}
	}

	//
	// Normalize Path Errors
	//
	for (viterbiStateCounter= 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
	{
		viterbiPathError[viterbiStateCounter] -= lowestErrorMetric;
	}

	return lowestErrorMetric;
}

/***********************************  END  ************************************/
