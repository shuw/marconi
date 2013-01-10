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


#define __ERRORCHECK_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"
#include <math.h>
#include <time.h>

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static void ErrorCheckDisplayCompare(BYTE errorSourceBufferData, BYTE inputData, 
				BYTE hammingsDistance, ERROR_CHECK_OBJ_STRUCT *ErrorCheckObjStruct);


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
void ErrorCheckInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	ERROR_CHECK_OBJ_STRUCT *ErrorCheckObjStruct = (ERROR_CHECK_OBJ_STRUCT *)BlockObjStruct;
	ERROR_CHECK_INIT_STRUCT*ErrorCheckInitStruct = (ERROR_CHECK_INIT_STRUCT *)BlockInitStruct;

	
	ErrorCheckObjStruct->progressCounter = 0;
	ErrorCheckObjStruct->progressSecondCounter = 0;
	ErrorCheckObjStruct->clockSartTimer = clock();
	ErrorCheckObjStruct->fifoFillLevel = 0;
	ErrorCheckObjStruct->accumErrorMetric = 0;
	ErrorCheckObjStruct->totalBitsTransferred = 0;
	ErrorCheckObjStruct->fifoPushPtr = &ErrorCheckObjStruct->fifoBuffer[0];
	ErrorCheckObjStruct->fifoPopPtr = &ErrorCheckObjStruct->fifoBuffer[0];
	ErrorCheckObjStruct->optionsFlags = ErrorCheckInitStruct->optionsFlags;
	ErrorCheckObjStruct->displayCompareLineErrorMetric = 0;
	ErrorCheckObjStruct->displayCompareCharCounter = 0;
	
	ErrorCheckObjStruct->totalBitsCoded = ErrorCheckInitStruct->totalBitsCoded;
	ErrorCheckObjStruct->totalBitsThroughChannel = ErrorCheckInitStruct->totalBitsThroughChannel;
	ErrorCheckObjStruct->syncDropped = ErrorCheckInitStruct->syncDropped;
	ErrorCheckObjStruct->viterbiPathError = ErrorCheckInitStruct->viterbiPathError;
	ErrorCheckObjStruct->bitShiftDetected = ErrorCheckInitStruct->bitShiftDetected;
 	ErrorCheckObjStruct->packetsReceived = ErrorCheckInitStruct->packetsReceived;
 	ErrorCheckObjStruct->packetsTransmitted = ErrorCheckInitStruct->packetsTransmitted;
 	ErrorCheckObjStruct->bitsDroppedInChannel = ErrorCheckInitStruct->bitsDroppedInChannel;




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
WORD ErrorCheckSource(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
#ifndef ERROR_CHECK_DISABLE_COMPARISON
	ERROR_CHECK_OBJ_STRUCT *ErrorCheckObjStruct = (ERROR_CHECK_OBJ_STRUCT *)BlockObjStruct;
	WORD inputByte;
#endif
	WORD inputSize = *inSize;

	memcpy(outBuffer, inBuffer, inputSize * sizeof(WORD));

#ifndef ERROR_CHECK_DISABLE_COMPARISON
	while (inputSize--)
	{
		inputByte = *inBuffer++;
		
		*ErrorCheckObjStruct->fifoPushPtr++ = inputByte;
			
		ErrorCheckObjStruct->fifoFillLevel++;

		if (ErrorCheckObjStruct->fifoFillLevel > SIZE_OF_ERRORCHECK_FIFO)
		{
			DebugMsg("\n\n\rError Check Fifo Overflow - ");
			DebugMsg(CurrentControllerPtr->nameString);
		}
			
		if (ErrorCheckObjStruct->fifoPushPtr >= (ErrorCheckObjStruct->fifoBuffer + SIZE_OF_ERRORCHECK_FIFO))
		{
			ErrorCheckObjStruct->fifoPushPtr = &ErrorCheckObjStruct->fifoBuffer[0];
		}
	}
#endif
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
WORD ErrorCheckDestination(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
#ifndef ERROR_CHECK_DISABLE_COMPARISON
	ERROR_CHECK_OBJ_STRUCT *ErrorCheckObjStruct = (ERROR_CHECK_OBJ_STRUCT *)BlockObjStruct;
	WORD inputData, errorSourceBufferData;
	WORD tempData;
	BYTE hammingsDistance;
#endif	
	WORD inputSize = *inSize;

	memcpy(outBuffer, inBuffer, inputSize * sizeof(WORD));

#ifndef ERROR_CHECK_DISABLE_COMPARISON
	while (inputSize--)
	{
		inputData = *inBuffer++;
		errorSourceBufferData = *ErrorCheckObjStruct->fifoPopPtr++;
		
		//compare incoming bytes with fifo
		tempData = inputData ^ errorSourceBufferData;

		// TODO: Improve the efficiency of method to get hamming's distance
		hammingsDistance = 0;
		while (tempData)
		{
			hammingsDistance += tempData & 0x1;
			tempData = (tempData >> 1) & ~(MOST_SIG_DIGIT_IN_WORD);
		}
		ErrorCheckObjStruct->accumErrorMetric += hammingsDistance;
		ErrorCheckObjStruct->totalBitsTransferred += BITS_IN_WORD;

		//
		// Displays file progress for long files
		//
#ifdef PLATFORM_SIM
		if (ErrorCheckObjStruct->optionsFlags & ERROR_CHECK_FILE_PROGRESS_MONITOR_FLAG)
		{
			if ((ErrorCheckObjStruct->totalBitsTransferred - ErrorCheckObjStruct->progressCounter) >
                ((DWORD)(1024 * 8) * (DWORD)ERROR_CHECK_STATUS_DELTA_KB))
			{
				ErrorCheckObjStruct->progressCounter = ErrorCheckObjStruct->totalBitsTransferred;
				ErrorCheckObjStruct->progressSecondCounter += ERROR_CHECK_STATUS_DELTA_KB;
				DebugMsgDW("\n\rTransfer Progress (kiloBytes): %lu", ErrorCheckObjStruct->progressSecondCounter);
				DebugMsgDW("  Accum Bit Error: %lu", ErrorCheckObjStruct->accumErrorMetric);
			}
		}
#endif
		//
		// Displays live comparison
		//
		if (ErrorCheckObjStruct->optionsFlags & ERROR_CHECK_LIVE_COMPARISON_FLAG)
		{
			ErrorCheckDisplayCompare((BYTE)errorSourceBufferData, (BYTE)inputData, 
								hammingsDistance, ErrorCheckObjStruct);
			ErrorCheckDisplayCompare((BYTE)(errorSourceBufferData >> 8), (BYTE)(inputData >> 8), 
								hammingsDistance, ErrorCheckObjStruct);

		}

		ErrorCheckObjStruct->fifoFillLevel--;


		if (ErrorCheckObjStruct->fifoFillLevel > SIZE_OF_ERRORCHECK_FIFO)
		{
			DebugMsg("\n\n\rError Check Fifo Undeflow - ");
			DebugMsg(CurrentControllerPtr->nameString);
		}

		if (ErrorCheckObjStruct->fifoPopPtr >= (ErrorCheckObjStruct->fifoBuffer + SIZE_OF_ERRORCHECK_FIFO))
		{
			ErrorCheckObjStruct->fifoPopPtr = ErrorCheckObjStruct->fifoBuffer;
		}
	}
#endif
	return 0;
}


//******************************************************************************
//
// FUNCTION:        Error Checker Display Live Comparison
//
// USAGE:             Displays a live comparison of the file being tested
//				
//
// INPUT:              errorSourceBufferData - WORD for destination
//				inputData - WORD for source
//				getHammingsDistance - hammings distance of the WORD
//				Memory Structure for Error Checker Object
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
static void ErrorCheckDisplayCompare(BYTE errorSourceBufferData, BYTE inputData, 
				BYTE hammingsDistance, ERROR_CHECK_OBJ_STRUCT *ErrorCheckObjStruct)
{
	BYTE charCounter = ErrorCheckObjStruct->displayCompareCharCounter += 1;

			
	//
	// Wrong Characters are displayed with special character
	//
	if (inputData == ERROR_CHECK_ERROR_DISPLAY_CHAR)
	{
		inputData = ' ';
		errorSourceBufferData = ' ';
	}
	if (inputData != errorSourceBufferData)
	{
#ifdef ERROR_CHECK_REPLACE_ERROR_CHARACTERS
		inputData = ERROR_CHECK_ERROR_DISPLAY_CHAR;
#endif
	}

	
	ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] = (BYTE)errorSourceBufferData;
	ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] = (BYTE)inputData;
	charCounter++;
	
	if (charCounter >= ERROR_CHECK_COMPARISON_CHAR_PER_SIDE)
	{
		BYTE displayLineErrorMetric;
		
		ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] = 0x00;
		ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] = 0x00;
		ErrorCheckObjStruct->displayCompareCharCounter = 0;

		for (charCounter = 0; charCounter < ERROR_CHECK_COMPARISON_CHAR_PER_SIDE; charCounter++)
		{
			//
			// Clears the buffer of unprintable characters, backspace and ascii char 0->0x20
			//
			if ( 	( ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] < 0x20) ||
				(ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] == 0x7F) ||
				(ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] == 0x25) ||
				(ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] == 0x5C) ||
				( (ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] >= 0x7F)  &&
				(ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] <= 0x9F) ) )
				
			{
				ErrorCheckObjStruct->displayCompareDestinationBuffer[charCounter] = ' ';
			}
			if (  ( ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] < 0x20) ||
				(ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] == 0x7F) ||
				(ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] == 0x25) ||
				(ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] == 0x5C) ||
				((ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] >= 0x7F)  &&
				(ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] <= 0x9F) ) )
			{
				ErrorCheckObjStruct->displayCompareSourceBuffer[charCounter] = ' ';
			}

		}

		displayLineErrorMetric = (BYTE)(ErrorCheckObjStruct->accumErrorMetric
					- ErrorCheckObjStruct->displayCompareLineErrorMetric);
		ErrorCheckObjStruct->displayCompareLineErrorMetric = ErrorCheckObjStruct->accumErrorMetric;

		DebugMsg("\n\r");
		DebugMsg(CurrentControllerPtr->nameString);
		DebugMsg(" - S: ");
		DebugMsg((char *)ErrorCheckObjStruct->displayCompareSourceBuffer);
		DebugMsg("  D: ");
		DebugMsg((char *)ErrorCheckObjStruct->displayCompareDestinationBuffer);

		if (displayLineErrorMetric > 0)
		{
			DebugMsgW("  E: %d", displayLineErrorMetric);
		}
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
WORD ErrorCheckSourceReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*inputSize >= 4 && *outputSize >= 4)
	{
		*inputSize = 4;
		*outputSize = 4;
		return TRUE;
	}
	return FALSE;
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
WORD ErrorCheckDestinationReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*inputSize >= 4 && *outputSize >= 4)
	{
		*inputSize = 4;
		*outputSize = 4;
		return TRUE;
	}
	return FALSE;
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
void	ErrorCheckPrintStatistics(BYTE *BlockObjStruct)
{
	ERROR_CHECK_OBJ_STRUCT *ErrorCheckObjStruct = (ERROR_CHECK_OBJ_STRUCT *)BlockObjStruct;

	double bitErrorRate;
	double logOfBitErrorRate;
	double codecEfficiency = 0;

	if (*ErrorCheckObjStruct->totalBitsCoded!= 0)
		codecEfficiency = (double)ErrorCheckObjStruct->totalBitsTransferred / (double)*ErrorCheckObjStruct->totalBitsCoded;

	DebugMsg("\n\n\r*** Statistical Summary for: ");
#ifdef PLATFORM_SIM
	DebugMsg(INPUT_FILE_NAME);
#endif
	DebugMsg(" ***");
	DebugMsgDW("\n\n\rError Check Accumulated Hammings Distance: %lu", ErrorCheckObjStruct->accumErrorMetric);

	DebugMsgDW("\n\rError Check Total Bits Transferred: %lu", ErrorCheckObjStruct->totalBitsTransferred);

	DebugMsgDW("\n\rSync Dropped: %lu", *ErrorCheckObjStruct->syncDropped);
	DebugMsgDW("\n\rPackets Transmitted: %lu", *ErrorCheckObjStruct->packetsTransmitted);
	DebugMsgDW("\n\rPackets Dropped: %lu", *ErrorCheckObjStruct->packetsTransmitted - *ErrorCheckObjStruct->packetsReceived);

//	DebugMsgFloat("\nSignal to Noise Ratio in dB: %f", AWGNSignalToNoiseDB);
	bitErrorRate = (double)ErrorCheckObjStruct->accumErrorMetric / (double)ErrorCheckObjStruct->totalBitsTransferred;
	logOfBitErrorRate = log10(bitErrorRate);

	DebugMsgFloat("\n\rBit Error Rate: %f", bitErrorRate);
	DebugMsgFloat("\n\rBit Error Rate: 10^(%f)", logOfBitErrorRate);


	DebugMsgDW("\n\rViterbi Accum Path Error Metric: %lu", *ErrorCheckObjStruct->viterbiPathError / SOFT_LEVEL_BINARY_BIT_MAX_LEVEL);	
	DebugMsgDW("\n\rNum Of Bits Dropped In Channel: %lu", *ErrorCheckObjStruct->bitsDroppedInChannel);
	DebugMsgDW("\n\rViterbi Num Of Bit Shifts Detected: %lu", *ErrorCheckObjStruct->bitShiftDetected);
	
	DebugMsgFloat("\n\rCodec Efficiency (Bits Transferred / Bits Coded): %f", codecEfficiency);


	if ((clock() - ErrorCheckObjStruct->clockSartTimer) != 0)
		DebugMsgFloat("\n\rThroughput (KBits transferred / sec): %f\n\r", ((ErrorCheckObjStruct->totalBitsTransferred / (clock() - ErrorCheckObjStruct->clockSartTimer))  ));	
}
		
/***********************************  END  ************************************/
