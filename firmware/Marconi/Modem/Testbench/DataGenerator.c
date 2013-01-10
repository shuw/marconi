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


#define __DATAGENERATOR_C__


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


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  C O D E
//******************************************************************************
WORD testArray[16] = 
{
       16961,    17475,    17989,    18503,    19017,    19531,    20045,    20559, 
       21073,    21587,    22101,    22615,    23129,    23643,    24157,    24671, 
};

//******************************************************************************
//
//	Data Generator Initialization Block
//
//******************************************************************************
void DataGeneratorInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	DATA_GENERATOR_OBJ_STRUCT *DataGeneratorObjStruct =
		(DATA_GENERATOR_OBJ_STRUCT *)BlockObjStruct;

	DataGeneratorObjStruct->counter = 0;
	DataGeneratorObjStruct->startSync = 1;
}

//******************************************************************************
//
//	Data Generator Execution Block
//
//******************************************************************************
WORD DataGenerator(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	DATA_GENERATOR_OBJ_STRUCT *DataGeneratorObjStruct =
		(DATA_GENERATOR_OBJ_STRUCT *)BlockObjStruct;
	WORD counter = DataGeneratorObjStruct->counter;
	WORD i;
	WORD outSizeAvail = *outSize;

/*
    for(i = 0; i < *outSize; ++i) {
        // ASCII characters from '0' to 'z' inclusive
        *outBuffer++ = ('0' + counter % 75) | ('0' + (counter + 1) % 75) << 8;
        counter += 2;
    }
*/
	if (DataGeneratorObjStruct->startSync)
	{
		*outBuffer++ = DATAGEN_SYNC;
		outSizeAvail -= 1;
		DataGeneratorObjStruct->startSync = 0;			
	}
	for(i = 0; i < outSizeAvail; ++i) {
		// ASCII characters from '0' to 'z' inclusive
		*outBuffer++ = testArray[counter++];
		if (counter >= (16))
		{
			counter = 0;
		}
	}


	DataGeneratorObjStruct->counter = counter;
	return 0;
}

//******************************************************************************
//
//	Data Generator Ready Check
//
//******************************************************************************
WORD DataGeneratorReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*outputSize >= 1)
	{
		return TRUE;
	}
	
	return FALSE;
}

/***********************************  END  ************************************/
