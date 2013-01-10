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


#define __VDECODER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"
#include <math.h>

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static void ViterbiStateGetChildren(BBYTE vibertiState, BBYTE inputBits, BBYTE *childrenStates, BBYTE *childrenErrorMetrics);


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
static BBYTE viterbiPathTree[VITERBI_NUM_OF_STATES][VITERBI_MAX_PATH_LENGTH];
static BBYTE viterbiPathError[VITERBI_NUM_OF_STATES];




static BBYTE nextGenerationStatesError[VITERBI_NUM_OF_STATES];
//******************************************************************************
//  C O D E
//******************************************************************************
void VDecoderInit(void)
{

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
BBYTE VDecoder(BBYTE *inBuffer, BBYTE *inSize, BBYTE *outBuffer, BBYTE *outSize)
{
	BBYTE inputSizeCounter = *inSize;
	BBYTE inputBitPosition;
	BBYTE inputBits;
	BBYTE childrenCounter;
	BBYTE viterbiStateCounter = 0;
	BBYTE currentInputByte;
	BBYTE viterbiTreePosition = 0;

	

	BBYTE childrenStates[VITERBI_NUM_OF_CHILDREN_STATES];
	BBYTE childrenErrorMetrics[VITERBI_NUM_OF_CHILDREN_STATES];
	BBYTE nextGenerationErrorMetric[VITERBI_NUM_OF_STATES];

	for (viterbiStateCounter = 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
	{
		viterbiPathError[viterbiStateCounter] = 0;
	}


	//
	// First process all input bits
	//
	while (inputSizeCounter-- > 0)
	{
		inputBitPosition = 0;
		currentInputByte = *inBuffer++;

		while (inputBitPosition < NUM_OF_BITS_IN_BBYTE)
		{
			//gets two bits from the input
			inputBits = (currentInputByte >> inputBitPosition) & 0x03;

			
			for (viterbiStateCounter = 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
			{
				nextGenerationErrorMetric[viterbiStateCounter] = (BBYTE)(0xFFFF & ~(MOST_SIG_DIGIT_IN_BBYTE));
			}
			
			for (viterbiStateCounter = 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
			{

				ViterbiStateGetChildren(viterbiStateCounter, inputBits, childrenStates, childrenErrorMetrics);

				for (childrenCounter = 0; childrenCounter < VITERBI_NUM_OF_CHILDREN_STATES; childrenCounter++)
				{
					BBYTE childState = childrenStates[childrenCounter];
					BBYTE childErrorMetric = childrenErrorMetrics[childrenCounter] + viterbiPathError[viterbiStateCounter];

					//
					// Sets the parent pointers for the child states
					//
					if ( childErrorMetric < nextGenerationErrorMetric[ childState ] )
					{
						viterbiPathTree[childState][viterbiTreePosition] = viterbiStateCounter;
						nextGenerationErrorMetric[childState] =  childErrorMetric;
					}
				}
			}
			
			//Copies calculated errors			
			for (viterbiStateCounter = 0; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
			{
				viterbiPathError[viterbiStateCounter] = nextGenerationErrorMetric[viterbiStateCounter];
			}
			

			inputBitPosition += 2;

			viterbiTreePosition++;
			if (viterbiTreePosition >= VITERBI_MAX_PATH_LENGTH)
			{
				PrintMsg("Viberti Tree Overflow");
			}
			
		}
		

		
	}

	//
	// Now trace back through path to get output bits
	// Each tree position represents one bit, so allocate that much in outbuffer and retrive bits backwards up tree
	//
	viterbiTreePosition--;
	{
		//
		// currently, this will produce exactly inputsize / 2
		// however, flexibility is added to match output size to size of tree
		//
		BBYTE sizeOfTreeInBytes = (viterbiTreePosition / NUM_OF_BITS_IN_BBYTE);
		BBYTE outputBitPosition = viterbiTreePosition % NUM_OF_BITS_IN_BBYTE;
		BBYTE stateWithLeastError = 0;
		BBYTE lowestErrorMetric = viterbiPathError[0];
		BBYTE currentState;
		BBYTE parentState;
		
		outBuffer = outBuffer + sizeOfTreeInBytes;

		*outBuffer = 0;

		//
		// Start with the tree with the least error
		//
		for (viterbiStateCounter= 1; viterbiStateCounter < VITERBI_NUM_OF_STATES; viterbiStateCounter++)
		{
			if (viterbiPathError[viterbiStateCounter] < lowestErrorMetric)
			{
				lowestErrorMetric = viterbiPathError[viterbiStateCounter];
				stateWithLeastError = viterbiStateCounter;
			}
		}

		currentState = stateWithLeastError;
		while ( (viterbiTreePosition) >= 0)
		{
			
			parentState = viterbiPathTree[currentState][viterbiTreePosition--];

			//the state number contains the most current bit as it is a bit history
			*outBuffer |= (BBYTE)( (currentState & 0x1) << outputBitPosition );

			currentState = parentState;


			if (outputBitPosition == 0)
			{
				outBuffer--;
				*outBuffer = 0;
				outputBitPosition = NUM_OF_BITS_IN_BBYTE;
			}
			outputBitPosition--;
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
static void ViterbiStateGetChildren(BBYTE vibertiState, BBYTE inputBits, BBYTE *childrenStates, BBYTE *childrenErrorMetrics)
{
	BBYTE outputBits;
	BBYTE getHammingsDistance;
	BBYTE errorMetric;
	//	
	//get child 1
	//
	outputBits = ViterbiEncodingAlgorithm( (BBYTE)((vibertiState << 1) | 0 ));
	getHammingsDistance = (outputBits ^ inputBits);

	errorMetric = 0;
	//count number of bits different
	while (getHammingsDistance)
	{
		errorMetric += getHammingsDistance & 0x1;
		getHammingsDistance = getHammingsDistance >> 1;
	}


	*childrenErrorMetrics++ = errorMetric;
	//vibertiState is simply the history of previous bits
	*childrenStates++ = (vibertiState  << 1) & VITERBI_STATES_BIT_MASK;
		
	//
	// get child 2
	//
	outputBits = ViterbiEncodingAlgorithm( (BBYTE)((vibertiState << 1) | 1 ));
	//add hammings distance to error metric
	getHammingsDistance = (outputBits ^ inputBits);

	errorMetric = 0;
	//count number of bits different
	while (getHammingsDistance)
	{
		errorMetric += getHammingsDistance & 0x1;
		getHammingsDistance = getHammingsDistance >> 1;
	}

	*childrenErrorMetrics = errorMetric;
	*childrenStates++ = ((vibertiState  << 1) & VITERBI_STATES_BIT_MASK) | 1;
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
BBYTE VDecoderInputSizeB(void)
{
	return (VENCODER_PACKET_SIZE);
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
BBYTE VDecoderOutputSizeB(void)
{
	return (VENCODER_PACKET_SIZE / 2);
}


/***********************************  END  ************************************/
