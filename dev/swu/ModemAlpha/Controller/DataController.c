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


#define __DATACONTROLLER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"
#include <string.h>

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static BOOL DataControllerIsReady(DATA_CONTROLLER_STRUCT *ControllerSelect);
static BYTE *DataControllerGetInputBuffer(DATA_CONTROLLER_STRUCT *ControllerSelect, BYTE sizeB);
static void DataControllerPopInputBuffer(DATA_CONTROLLER_STRUCT *ControllerSelect, BYTE sizeB);

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
BYTE	controllerInBuffer[SIZEOF_CONTROLLER_INPUT_BUFFER];
BYTE	controllerOutBuffer[SIZEOF_CONTROLLER_OUTPUT_BUFFER];

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
BOOL DataControllerInit( DATA_CONTROLLER_STRUCT *ControllerSelect, 
							BYTE *nameString,
							BYTE (*BlockFire)(BYTE *inBuffer, BYTE *inSizeB, BYTE *outBuffer, BYTE *outSizeB),
							BYTE (*BlockInputSizeB)(void), 
							BYTE (*BlockOutputSizeB)(void),
							void (*BlockReset)(void),
							BYTE *fifoBuffer,
							WORD fifoSizeB )
{	

	ControllerSelect->nameString = nameString;
	ControllerSelect->BlockFire = BlockFire;
	ControllerSelect->BlockInputSizeB = BlockInputSizeB;
	ControllerSelect->BlockOutputSizeB = BlockOutputSizeB;
	ControllerSelect->BlockReset = BlockReset;
	ControllerSelect->pipelinePosition = CONTROLLER_PIPELINE_MIDDLE; //default to middle pipeline	

	//
	// Controller fifo initialization
	//
	ControllerSelect->fifoSizeB = fifoSizeB;
	ControllerSelect->fifoFillLevelB = 0;
	ControllerSelect->fifoStartAddress = fifoBuffer;
	ControllerSelect->fifoEndAddress = fifoBuffer + fifoSizeB;
	ControllerSelect->fifoPushPtr = fifoBuffer;
	ControllerSelect->fifoPopPtr = fifoBuffer;
	return TRUE;
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
BOOL DataControllerPipelineInit( DATA_CONTROLLER_STRUCT *ControllerSelect,
							PIPELINE_CONTROLLER_TYPE controllerType,
							DATA_CONTROLLER_STRUCT * NextDataController )
{
	ControllerSelect->pipelinePosition = controllerType;
	ControllerSelect->NextDataController = (BYTE *)NextDataController;

	return TRUE;
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
// OUTPUT:           TRUE - Block executed
//				FALSE - Block not executed
//
// GLOBALS:         N/A
//
//******************************************************************************
BOOL DataControllerFire(DATA_CONTROLLER_STRUCT *ControllerSelect)
{
	if ( DataControllerIsReady(ControllerSelect) )
	{
		BYTE *inputBuffer;
		BYTE inputBufferSizeB;
		BYTE outputBufferSizeB;
	
		PrintMsg("\nBlock Executed: ");
		PrintMsg(ControllerSelect->nameString);


		inputBuffer = DataControllerGetInputBuffer(ControllerSelect, ControllerSelect->BlockInputSizeB());

		if (inputBuffer != NULL)
		{
			inputBufferSizeB = ControllerSelect->BlockInputSizeB();
		}
		else
		{
			inputBufferSizeB = 0;
		}
		
		outputBufferSizeB = ControllerSelect->BlockOutputSizeB();
		
		//
		// Process Encoder
		//
		switch (
			ControllerSelect->BlockFire
			(
				inputBuffer,
				&inputBufferSizeB,
				controllerOutBuffer,
				&outputBufferSizeB
			) )
		{
			case RESET_PIPELINE_DOWN_CONTROLLER:
				DataControllerResetPipeline(ControllerSelect);
					
				PrintMsg("\n   Reset Pipeline Down");
				break;
			default:
			break;
		}

		DataControllerPopInputBuffer(ControllerSelect, inputBufferSizeB);

		if (ControllerSelect->pipelinePosition != CONTROLLER_PIPELINE_TAIL) // Controller Produces an output
		{
			DataControllerPushBuffer
			(
				(DATA_CONTROLLER_STRUCT *)ControllerSelect->NextDataController, 
				controllerOutBuffer, outputBufferSizeB
			);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
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
// OUTPUT:           TRUE - Block executed
//				FALSE - Block not executed
//
// GLOBALS:         N/A
//
//******************************************************************************
void DataControllerResetPipeline(DATA_CONTROLLER_STRUCT *ControllerSelect)
{
	if (ControllerSelect->BlockReset != NULL)
	{
		ControllerSelect->BlockReset();
	}
	
	ControllerSelect->fifoFillLevelB = 0;
	ControllerSelect->fifoPushPtr = ControllerSelect->fifoStartAddress;
	ControllerSelect->fifoPopPtr = ControllerSelect->fifoStartAddress;

	//
	// Reset Down Pipeline
	//
	if (ControllerSelect->NextDataController != NULL)
	{
		DataControllerResetPipeline((DATA_CONTROLLER_STRUCT *)ControllerSelect->NextDataController);
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
// OUTPUT:           TRUE - Block executed
//				FALSE - Block not executed
//
// GLOBALS:         N/A
//
//******************************************************************************
BOOL DataControllerPushBuffer(DATA_CONTROLLER_STRUCT *ControllerSelect, BYTE *buffer, BYTE bufferSizeB)
{
	BYTE *packetPtr = ControllerSelect->fifoPushPtr;

	if ( (ControllerSelect->fifoFillLevelB + bufferSizeB) <= ControllerSelect->fifoSizeB)
	{
		ControllerSelect->fifoFillLevelB += bufferSizeB;
	}
	//
	// Fifo Full
	//
	else
	{
		PrintMsg("\nFifo Push Overflow: ");
		PrintMsg(ControllerSelect->nameString);
		return (FALSE);
	}
	// TODO: Change to memcopy
	while (bufferSizeB-- > 0)
	{
		//wrap around fifo		
		if (ControllerSelect->fifoPushPtr >= ControllerSelect->fifoEndAddress)
		{
			ControllerSelect->fifoPushPtr = ControllerSelect->fifoStartAddress;
		}
		*(ControllerSelect->fifoPushPtr++) = *buffer++;
	}


	return (TRUE);
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
WORD DataControllerRemainingSizeB(DATA_CONTROLLER_STRUCT *ControllerSelect)
{
	return (ControllerSelect->fifoSizeB - ControllerSelect->fifoFillLevelB);
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
static BOOL DataControllerIsReady(DATA_CONTROLLER_STRUCT *ControllerSelect)
{
	if (ControllerSelect->pipelinePosition == CONTROLLER_PIPELINE_ALWAYSFIRE)
	{
		return TRUE;
	}
		
	if ( ControllerSelect->pipelinePosition != CONTROLLER_PIPELINE_HEAD  )
	{
		if ( ControllerSelect->fifoFillLevelB <ControllerSelect->BlockInputSizeB() )
		{
			return (FALSE);
		}
	}
	if ( ControllerSelect->pipelinePosition != CONTROLLER_PIPELINE_TAIL )
	{
		if ( DataControllerRemainingSizeB((DATA_CONTROLLER_STRUCT *)(ControllerSelect->NextDataController) )
			< ControllerSelect->BlockOutputSizeB() )
		{
			return (FALSE);
		}
	}

	return (TRUE);
}

//******************************************************************************
//
// FUNCTION:        N/A
//
// USAGE:             Decreases fifo fill size and increments pop pointer
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
static void DataControllerPopInputBuffer(DATA_CONTROLLER_STRUCT *ControllerSelect, BYTE sizeB)
{
	ControllerSelect->fifoPopPtr += sizeB;
	ControllerSelect->fifoFillLevelB -= sizeB;
	
	//wrap around fifo		
	if (ControllerSelect->fifoPopPtr >= ControllerSelect->fifoEndAddress)
	{
		ControllerSelect->fifoPopPtr = ControllerSelect->fifoStartAddress + (ControllerSelect->fifoPopPtr - ControllerSelect->fifoEndAddress);
	}
}

//******************************************************************************
//
// FUNCTION:        N/A
//
// USAGE:             gets input buffer
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
static BYTE *DataControllerGetInputBuffer(DATA_CONTROLLER_STRUCT *ControllerSelect, BYTE sizeB)
{
	BYTE *bufferPtr;
	BYTE *fifoPopPtr;
	
	//
	// Check there is enough space left in fifo
	//
	if (ControllerSelect->fifoFillLevelB < sizeB)
	{
		PrintMsg("\nFifo Undeflow: ");
		PrintMsg(ControllerSelect->nameString);
		return (NULL);	
	}
	
	//
	// return buffer directly if no wrap around needed
	//
	if ( (ControllerSelect->fifoEndAddress - ControllerSelect->fifoPopPtr) >= sizeB )
	{
		return (ControllerSelect->fifoPopPtr);
	}
	else
	{
		bufferPtr = controllerInBuffer;
		fifoPopPtr = ControllerSelect->fifoPopPtr;
		
		while (sizeB-- > 0)
		{
			// TODO: Change to memcopy
			//wrap around fifo		
			if (fifoPopPtr >= ControllerSelect->fifoEndAddress)
			{
				fifoPopPtr = ControllerSelect->fifoStartAddress;
			}
			*bufferPtr++ = *(fifoPopPtr++);
		}
		
		
		return (controllerInBuffer);
	}


	

}





/***********************************  END  ************************************/
