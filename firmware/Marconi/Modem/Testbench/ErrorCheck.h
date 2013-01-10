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


#ifndef __ERRORCHECK_H__
	#define __ERRORCHECK_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define SIZE_OF_ERRORCHECK_FIFO 256

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************

//
// Options Flags for Error Checking Block,
// Set in (ERROR_CHECK_INIT_STRUCT)####.optionsFlags
//
#define ERROR_CHECK_FILE_PROGRESS_MONITOR_FLAG		BIT0
#define ERROR_CHECK_LIVE_COMPARISON_FLAG			BIT1

//
// Options for visual display of error checking progress
//
#define ERROR_CHECK_STATUS_DELTA_KB						5
#define ERROR_CHECK_COMPARISON_CHAR_PER_SIDE			26
#define ERROR_CHECK_ERROR_DISPLAY_CHAR					'E'

typedef struct
{
	DWORD 	*totalBitsCoded;
	DWORD 	*totalBitsThroughChannel;
	WORD	*viterbiPathError;
	WORD 	*syncDropped;
	WORD 	*bitShiftDetected;
	DWORD 	*packetsTransmitted;
	DWORD 	*packetsReceived;
	WORD 	*bitsDroppedInChannel;
	WORD	optionsFlags;
} ERROR_CHECK_INIT_STRUCT;


typedef struct
{
	clock_t 	clockSartTimer;
	WORD 	fifoBuffer[SIZE_OF_ERRORCHECK_FIFO];
	WORD *	fifoPushPtr;
	WORD *	fifoPopPtr;
	WORD 	fifoFillLevel;
	DWORD 	accumErrorMetric;
	DWORD 	totalBitsTransferred;
	DWORD 	progressCounter;
	WORD 	progressSecondCounter;
	BYTE 	displayCompareCharCounter;
	BYTE	displayCompareDestinationBuffer[(ERROR_CHECK_COMPARISON_CHAR_PER_SIDE) + 10]; //leave extra WORD for null at end
	BYTE	displayCompareSourceBuffer[(ERROR_CHECK_COMPARISON_CHAR_PER_SIDE ) + 10];		//leave extra WORD for null at end
	DWORD	displayCompareLineErrorMetric;
	WORD 	optionsFlags;
	DWORD *totalBitsCoded;
	DWORD *totalBitsThroughChannel;
	WORD	*viterbiPathError;
	WORD 	*bitShiftDetected;
	DWORD 	*packetsTransmitted;
	DWORD 	*packetsReceived;
	WORD *syncDropped;
	WORD 	*bitsDroppedInChannel;
} ERROR_CHECK_OBJ_STRUCT;


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void ErrorCheckInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);

WORD ErrorCheckSource(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD ErrorCheckDestination(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD ErrorCheckSourceReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);
WORD ErrorCheckDestinationReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);
void	ErrorCheckPrintStatistics(BYTE *BlockObjStruct);
#endif
