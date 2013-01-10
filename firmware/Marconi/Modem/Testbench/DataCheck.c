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


#define __DATACHECK_C__


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

//******************************************************************************
//
//	Block Initialization
//
//******************************************************************************
void DataCheckInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	DATA_CHECK_OBJ_STRUCT *DataCheckObjStruct =
				(DATA_CHECK_OBJ_STRUCT *)BlockObjStruct;
	DataCheckObjStruct->dataCounter = 0;
	DataCheckObjStruct->bitErrors = 0;
	DataCheckObjStruct->wordsReceived = 0;
	DataCheckObjStruct->bitPosition = 0;
	DataCheckObjStruct->state = DATA_CHECK_NO_SYNC;
}

//******************************************************************************
//
// Block Execution
//
//******************************************************************************
WORD DataCheck(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	DATA_CHECK_OBJ_STRUCT *DataCheckObjStruct =
				(DATA_CHECK_OBJ_STRUCT *)BlockObjStruct;

	WORD j;
	WORD correctW;
	WORD inputData;
	WORD inputAvail = *inSize;
	WORD outputAvail = *outSize;
	WORD inputBitPosition = DataCheckObjStruct->bitPosition;

	WORD counter = DataCheckObjStruct->dataCounter;

	while ( ( GetShiftedDataFromBuffer(inBuffer, inputAvail, inputBitPosition, &inputData) ) &&
			(outputAvail > 0))
	{
		switch (DataCheckObjStruct->state)
		{
			case(DATA_CHECK_NO_SYNC):
				if (inputData == DATAGEN_SYNC)
				{
					DataCheckObjStruct->state = DATA_CHECK_SYNC_FOUND;
					inputAvail--;
					inBuffer++;
				}
				else
				{
					inputBitPosition++;
					if (inputBitPosition >= BITS_IN_WORD)
					{
						inputBitPosition = inputBitPosition - BITS_IN_WORD;
						inputAvail--;
						inBuffer++;
					}
				}
				break;
			case (DATA_CHECK_SYNC_FOUND):
				correctW = testArray[counter++];
				correctW = correctW ^ inputData;

				if (counter >= (16))
					counter = 0;

				for (j = 0; j < 16; j++)
				{
					DataCheckObjStruct->bitErrors += (correctW & 0x1);
					correctW = correctW >> 1;
					
				}
				DataCheckObjStruct->wordsReceived++;
				*outBuffer++ = (WORD)inputData;
				outputAvail--;
				inBuffer++;
				inputAvail--;

				if ( (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG) &&
					(outputAvail < *outSize) )
				{
					DebugMsgDW("\n\r DataCheck Words Rx: %lu, ", DataCheckObjStruct->wordsReceived);
					DebugMsgDW("Bit Err: %lu ", DataCheckObjStruct->bitErrors);
					DebugMsgW("- 0x%04X", inputData);
				}

				break;
		}

	}


	*inSize = *inSize -inputAvail;
	*outSize = *outSize - outputAvail;
	
	DataCheckObjStruct->dataCounter = counter;
	DataCheckObjStruct->bitPosition = inputBitPosition;
	return RETURN_OK;
}


//******************************************************************************
//
//	Block ready
//
//******************************************************************************
WORD DataCheckReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*inputSize > 0 && *outputSize > 0)
	{
		//*inputSize = MIN(*inputSize, *outputSize);
		//*outputSize = *inputSize;
		return TRUE;
	}
	return FALSE;
}




/***********************************  END  ************************************/
