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


#ifndef __VENCODER_H__
	#define __VENCODER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
typedef struct
{
	WORD bitHistory;	
	WORD viterbiPathTree[VITERBI_NUM_OF_STATES][VDECODER_NUM_OF_INPUT_BITS];
	DWORD totalBitsCoded;
} VITERBI_ENCODER_OBJ_STRUCT;


//#define VENCODER_OUTPUT_PACKET_SIZE 			32
//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern BYTE ViterbiPolynomialLUT[ (1 << VITERBI_K)];

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
WORD VEncoder(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD VEncoderReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);
void VEncoderInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);


#endif
