//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		This is the core of the Data Controller functions
//				Also contains initialization functions
//				Pipeline Initialization function is also included because it is highly tied with
//				Data Controller functions
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
static WORD *DataControllerGetInputBuffer(DATA_CONTROLLER *ControllerSelect, WORD sizeB);
static void DataControllerPopInputBuffer(DATA_CONTROLLER *ControllerSelect, WORD sizeB);

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
WORD	controllerInBuffer[SIZEOF_CONTROLLER_INPUT_BUFFER];
WORD	controllerOutBuffer[SIZEOF_CONTROLLER_OUTPUT_BUFFER];

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
BOOL DataControllerInit( ROM DATA_CONTROLLER_INIT_STRUCT *ControllerInit)
{	
	DATA_CONTROLLER *ControllerSelect = ControllerInit->ControllerSelect;

	ControllerSelect->nameString = ControllerInit->nameString;
	ControllerSelect->BlockFireFn = ControllerInit->BlockFireFn;
	ControllerSelect->BlockReadyFn = ControllerInit->BlockReadyFn;
	ControllerSelect->BlockInitFn = ControllerInit->BlockInitFn;
	ControllerSelect->BlockObjStruct = ControllerInit->BlockObjStruct;
	ControllerSelect->BlockInitStruct = ControllerInit->BlockInitStruct;
	ControllerSelect->optionFlags = ControllerInit->optionsFlags;
	
	//
	// Controller fifo initialization
	//
	ControllerSelect->fifoSize = ControllerInit->fifoSizeB;
	ControllerSelect->fifoFillLevel = 0;
	ControllerSelect->fifoStartAddress = ControllerInit->fifoBuffer;
	ControllerSelect->fifoEndAddress = ControllerInit->fifoBuffer + ControllerInit->fifoSizeB;
	ControllerSelect->fifoPushPtr = ControllerInit->fifoBuffer;
	ControllerSelect->fifoPopPtr = ControllerInit->fifoBuffer;

	if (ControllerSelect->BlockInitFn != NULL)
		ControllerSelect->BlockInitFn(ControllerSelect->BlockInitStruct, ControllerSelect->BlockObjStruct);

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
BOOL DataControllerFire(DATA_CONTROLLER *ControllerSelect, DATA_CONTROLLER *NextController)
{
	WORD inputBufferSize = (WORD)ControllerSelect->fifoFillLevel;
	WORD outputBufferSize = 0;
	WORD *inputBuffer;
		
	if (NextController != NULL)
	{
		outputBufferSize = (WORD)DataControllerRemainingSizeB(NextController);
	}

	if ((  ControllerSelect->BlockReadyFn(&inputBufferSize, &outputBufferSize, ControllerSelect->BlockObjStruct) )
		|| (ControllerSelect->optionFlags & CONTROLLER_OPTIONS_DISABLE))
	{		
#ifdef DATACONTROLLER_DEBUG_ON
		DebugMsg("\nBlock Executed: ");
		DebugMsg(ControllerSelect->nameString);
#endif

		inputBuffer = DataControllerGetInputBuffer(ControllerSelect, inputBufferSize);

		
                if (ControllerSelect->optionFlags & CONTROLLER_OPTIONS_DISABLE)
                {
                    return FALSE;
                }
                else if (ControllerSelect->optionFlags & CONTROLLER_OPTIONS_PASSTHRU)
                {
                    inputBufferSize = MIN(inputBufferSize, outputBufferSize);
                    outputBufferSize = inputBufferSize;
                    memcpy(controllerOutBuffer, inputBuffer, inputBufferSize * sizeof(WORD));
                }
                else
                {
                	switch (
                		ControllerSelect->BlockFireFn
                		(
                			inputBuffer,
                			&inputBufferSize,
                			controllerOutBuffer,
                			&outputBufferSize,
                			ControllerSelect->BlockObjStruct
                		) )
                	{
                		case RETURN_CONTROLLER_BLOCK_IDLE:
                			return FALSE;
                		case RETURN_RESET_PIPELINE:
                			DebugMsg("\n\r   Test Functionality Unimplemented");					
                			DebugMsg("\n\r   Reset Pipeline Down");
                			break;
                		default:
                		break;
                	}
                }
                DataControllerPopInputBuffer(ControllerSelect, inputBufferSize);

                //Controller Produces an output
                if (outputBufferSize) 
                {
                	DataControllerPushBuffer
                	(
                		NextController,
                		controllerOutBuffer, outputBufferSize
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
//					FALSE - Block not executed
//
// GLOBALS:         N/A
//
//******************************************************************************
BOOL DataControllerPushBuffer(DATA_CONTROLLER *ControllerSelect, WORD *buffer, WORD bufferSize)
{
	if ( (ControllerSelect->fifoFillLevel + bufferSize) <= ControllerSelect->fifoSize)
	{
		ControllerSelect->fifoFillLevel += bufferSize;
	}
	//
	// Fifo Full
	//
	else
	{
		DebugMsg("\nFifo Push Overflow: ");
		DebugMsg(ControllerSelect->nameString);
		return (FALSE);
	}
	if ( (ControllerSelect->fifoPushPtr + (bufferSize * 2)) < ControllerSelect->fifoEndAddress)
	{
 		memcpy(ControllerSelect->fifoPushPtr, buffer, (bufferSize * sizeof(WORD)) );
 		ControllerSelect->fifoPushPtr += bufferSize;
		return TRUE;
	}
	while (bufferSize-- > 0)
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
WORD DataControllerRemainingSizeB(DATA_CONTROLLER *ControllerSelect)
{
	return (ControllerSelect->fifoSize - ControllerSelect->fifoFillLevel);
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
static void DataControllerPopInputBuffer(DATA_CONTROLLER *ControllerSelect, WORD sizeB)
{
	ControllerSelect->fifoPopPtr += sizeB;
	ControllerSelect->fifoFillLevel -= sizeB;

	if (ControllerSelect->fifoFillLevel == 0)
	{
		ControllerSelect->fifoPopPtr = ControllerSelect->fifoEndAddress;
		ControllerSelect->fifoPushPtr = ControllerSelect->fifoEndAddress;
	}
	
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
static WORD *DataControllerGetInputBuffer(DATA_CONTROLLER *ControllerSelect, WORD sizeB)
{
	WORD *bufferPtr;
	WORD *fifoPopPtr;
	
	//
	// Check there is enough space left in fifo
	//
	if (ControllerSelect->fifoFillLevel < sizeB)
	{
		DebugMsg("\nFifo Undeflow: ");
		DebugMsg(ControllerSelect->nameString);
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
#ifdef DATACONTROLLER_DEBUG_ON		
		DebugMsg("\nFifo Wraparound in block (this should be avoided to minimize copying): ");
		DebugMsg(ControllerSelect->nameString);
#endif
		bufferPtr = controllerInBuffer;
		fifoPopPtr = ControllerSelect->fifoPopPtr;

		while (sizeB-- > 0)
		{
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
