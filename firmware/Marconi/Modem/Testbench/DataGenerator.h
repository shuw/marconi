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


#ifndef __DATAGENERATOR_H__
	#define __DATAGENERATOR_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define DATAGEN_SYNC		0xAC78

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
typedef struct
{
	WORD counter;
	WORD startSync;
} DATA_GENERATOR_OBJ_STRUCT;


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern WORD testArray[16];

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

void DataGeneratorInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD DataGenerator(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
 WORD DataGeneratorReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);


#endif
