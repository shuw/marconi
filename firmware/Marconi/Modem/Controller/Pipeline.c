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


#define __PIPELINE_C__


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

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
PIPELINE *CurrentPipelinePtr;
DATA_CONTROLLER *CurrentControllerPtr;

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************

//******************************************************************************
//  C O D E
//******************************************************************************
static BOOL activityDetectInCurrentPipelineExecution;
static DATA_CONTROLLER * ROM *controllerListPtr;

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
void PipelineInit(ROM PIPELINE_INIT_STRUCT *PipelineInitStruct)
{
	DATA_CONTROLLER * ControllerSelect;
	PIPELINE *PipelineSelect = PipelineInitStruct->PipelineSelect;
	DATA_CONTROLLER * ROM * ControllersListPtr = PipelineInitStruct->DataControllersList;

	PipelineSelect->Controllers = ControllersListPtr;
	PipelineSelect->nameString = PipelineInitStruct->nameString;
	PipelineSelect->optionsFlags = PipelineInitStruct->optionsFlags;
	PipelineSelect->iterationCount = 0;

	//
	// Reset the controllers within the pipeline
	//
	while (*ControllersListPtr != NULL)
	{
		ControllerSelect = *ControllersListPtr;

		ControllerSelect->fifoFillLevel = 0;
		ControllerSelect->fifoPushPtr = ControllerSelect->fifoStartAddress;
		ControllerSelect->fifoPopPtr = ControllerSelect->fifoStartAddress;

		if (ControllerSelect->BlockInitFn != NULL)
			ControllerSelect->BlockInitFn(ControllerSelect->BlockInitStruct, ControllerSelect->BlockObjStruct);
		
		ControllersListPtr++;
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
BOOL PipelineFire(PIPELINE *Pipeline)
{
	WORD wasBlockFired;
	activityDetectInCurrentPipelineExecution = FALSE;

//	WORD fifoFillLevel;	
	if (Pipeline->optionsFlags & PIPELINE_OPTIONS_PRINT_DEBUG)
	{
		DebugMsgDW("\n\r#%06ld ", Pipeline->iterationCount);
		DebugMsg(Pipeline->nameString);
		DebugMsg(" ");
	}


	//
	// Sets global ptr called GlobalPipelinePtr
	//
	CurrentPipelinePtr = Pipeline;
	for (controllerListPtr = Pipeline->Controllers; *controllerListPtr != NULL; controllerListPtr++)
	{
//		fifoFillLevel = (*controllerListPtr)->fifoFillLevelB;
		if (Pipeline->optionsFlags & PIPELINE_OPTIONS_PRINT_DEBUG)
		{
//			DebugMsgW("%02LD", fifoFillLevel);

			if (wasBlockFired) DebugMsg("E>");
			else	 DebugMsg(" >");
		}
		CurrentControllerPtr = *controllerListPtr;
		wasBlockFired = DataControllerFire(*controllerListPtr, *(controllerListPtr + 1));
		activityDetectInCurrentPipelineExecution |= wasBlockFired;
	}

	if (Pipeline->optionsFlags & PIPELINE_OPTIONS_PRINT_DEBUG)
	{
		DebugMsg(" ");
	}
	Pipeline->iterationCount++;

	CurrentPipelinePtr->allControllersIdle = !activityDetectInCurrentPipelineExecution;
	return activityDetectInCurrentPipelineExecution;
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
BOOL IsCurrentPipelineIdle(void)
{
	return (BOOL)(CurrentPipelinePtr->allControllersIdle && !activityDetectInCurrentPipelineExecution);
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
BOOL IsCurrentPipelineEmptyDownStream(void)
{
	DATA_CONTROLLER * ROM *controllerPtr;

	if ((controllerListPtr + 1) != NULL)
	{
		for (controllerPtr = controllerListPtr + 1; *controllerPtr != NULL; controllerPtr++)
		{
			if ((*controllerPtr)->fifoFillLevel != 0)
			{
				return FALSE;
			}
		}
	}
	return TRUE;

}

/***********************************  END  ************************************/
