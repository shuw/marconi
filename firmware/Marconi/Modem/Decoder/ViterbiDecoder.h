//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		Viterbi TestDecoder Header File
//
//
//
//******************************************************************************


#ifndef __VDECODER_H__
	#define __VDECODER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef struct
{
	WORD oldPathTree[VDECODER_COLUMNS_IN_TRELLIS]
						[VDECODER_ROWS_IN_TRELLIS];
	WORD pathErrors[VITERBI_NUM_OF_STATES];
	WORD nextGenerationStatesError[VITERBI_NUM_OF_STATES];
	WORD isPathInitialized;
	WORD accumPathError;
	WORD numOfBitDroppedDetected;
} VITERBI_DECODER_OBJ_STRUCT;

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************





//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern DWORD ViterbiDecoderAccumPathError;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void VDecoderInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD VDecoder(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD VDecoderReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);

#endif
