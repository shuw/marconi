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


#ifndef __DATACHECK_H__
	#define __DATACHECK_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
typedef enum
{
	DATA_CHECK_NO_SYNC,
	DATA_CHECK_SYNC_FOUND,
	NUM_OF_DATA_CHECK_STATES
} DATA_CHECK_SYNC_STATES;
	
//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef struct
{
	WORD state;
	WORD dataCounter;
	WORD bitPosition;
	DWORD bitErrors;
	DWORD wordsReceived;
} DATA_CHECK_OBJ_STRUCT;

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void DataCheckInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD DataCheck(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD DataCheckReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);

#endif
