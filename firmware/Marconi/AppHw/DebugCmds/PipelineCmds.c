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


#define __PIPELINECMDS_C__


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
static void FirePipelineAction(SDWORD parameters[]);
static void SetFireIterationsAction(SDWORD parameters[]);
static void SetTestPipelineAction(SDWORD parameters[]);
static void SetPipelineDebugAction(SDWORD parameters[]);
static void SetPipelineLoopAction(SDWORD parameters[]);
static void ResetAction(SDWORD parameters[]);
static void InfoAction(SDWORD parameters[]);

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
static ROM DEBUG_CMD_STRUCT	pipelineCmdArray[] =
{
	{
		"fire", 
		"fires the pipeline for set number of iterations\n\r \
	param0 - pipelineID",
		1,
		FirePipelineAction,
		NULL
	},
	{
		"seti", 
		"sets the number of iterations to fire pipeline",
		1,
		SetFireIterationsAction,
		NULL
	},

	{
		"list", 
		"lists IDs of all instantiated pipelines",
		0,
		ListAllPipelinesAction,
		NULL
	},
	{
		"set", 
		"sets up the TEST pipeline using Controller IDs",
		NUMOFCMDPARAMETERS_CANBEANYSIZE,
		SetTestPipelineAction,
		NULL
	},
	{
		"debug", 
		"turns on/off the pipeline debug mode\n\r  \
	param0 - pipeline ID \n\r \
	param1 - debug on/off",
		2,
		SetPipelineDebugAction,
		NULL
	},
	{
		"loop", 
		"turns on/off the pipeline loop mode\n\r  \
	param0 - pipeline ID \n\r \
	param1 - debug on/off",
		2,
		SetPipelineLoopAction,
		NULL
	},
	{
		"reset", 
		"resets the pipeline\n\r  \
	param0 - pipeline ID ",
		1,
		ResetAction,
		NULL
	},
	{
		"info",
		"displays pipeline information\n\r  \
	param0 - pipeline ID ",
		1,
		InfoAction,
		NULL
	},	
};

static WORD pipelineFireNumOfIterations = 100;




//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
ROM CMD_LIST_STRUCT PipelineCmdList =
{
	pipelineCmdArray,
	NumOfCmdsInList(pipelineCmdArray)
};



//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  C O D E
//******************************************************************************

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void FirePipelineAction(SDWORD parameters[])
{
	BYTE count;

	for (count = 0; count < pipelineFireNumOfIterations; count++)
	{
		PipelineFire(PipelinesInstantiationList[parameters[0]].PipelineSelect);
	}
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void SetFireIterationsAction(SDWORD parameters[])
{
	pipelineFireNumOfIterations = parameters[0];
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void SetTestPipelineAction(SDWORD parameters[])
{
	BYTE count;

	if (numOfDebugParametersFound > MAX_SIZE_OF_TEST_PIPELINE)
	{
		DebugMsg("\n\rTest Pipeline Set Too Big");
		return;
	}
	for (count = 0; count < numOfDebugParametersFound; count++)
	{
		//
		// formats the pipeline with controllers IDs passed in by parameter
		//
		DebugMsgW("\n\rPosition #%d = ", count);
		DebugMsg(DataControllersInstantiationList[parameters[count]].ControllerSelect->nameString);
		
		HwTestPipelineControllersList[count] = 
			DataControllersInstantiationList[parameters[count]].ControllerSelect;
	}
	//the last controller in the pipeline is always a NULL
	HwTestPipelineControllersList[count] = NULL;

	PipelineInit(&PipelinesInstantiationList[TEST_PIPELINE_ID]);
}



//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
void ListAllPipelinesAction(SDWORD parameters[])
{
	BYTE count;

	DebugMsg("\n\r\n\r-- Pipelines Instantiation List --");
	for (count = 0; count <NUM_OF_PIPELINES; count++)
	{
		DebugMsgW("\n\rID #%d - ", count);
		DebugMsg(PipelinesInstantiationList[count].PipelineSelect->nameString);

	}
	
}


//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void SetPipelineDebugAction(SDWORD parameters[])
{
	if (parameters[1] == 1)
	{
		PipelinesInstantiationList[parameters[0]].PipelineSelect->optionsFlags |= PIPELINE_OPTIONS_PRINT_DEBUG;
	}
	else
	{
		PipelinesInstantiationList[parameters[0]].PipelineSelect->optionsFlags &= ~PIPELINE_OPTIONS_PRINT_DEBUG;
	}
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void SetPipelineLoopAction(SDWORD parameters[])
{
	if (parameters[1] == 1)
	{
		PipelinesInstantiationList[parameters[0]].PipelineSelect->optionsFlags |= PIPELINE_OPTIONS_RUN_IN_MAIN_LOOP;
	}
	else
	{
		PipelinesInstantiationList[parameters[0]].PipelineSelect->optionsFlags &= ~PIPELINE_OPTIONS_RUN_IN_MAIN_LOOP;
	}
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void ResetAction(SDWORD parameters[])
{
	PipelineInit(&PipelinesInstantiationList[parameters[0]]);
}



//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void InfoAction(SDWORD parameters[])
{
	PIPELINE *pipelineSelect = PipelinesInstantiationList[parameters[0]].PipelineSelect;
	DATA_CONTROLLER * ROM * controllersList;
	WORD count = 0;
	
	DebugMsg("\n\r-- Information for '");
	DebugMsg(pipelineSelect->nameString);
	DebugMsg("' Pipeline --\n\r");

	controllersList = pipelineSelect->Controllers;
	//displays controllers in selected pipeline
	while (*controllersList != NULL)
	{
		DebugMsgW("\n\rPosition #%d - ", count++);
		DebugMsg((*controllersList)->nameString);
		controllersList++;
	}
}

/***********************************  END  ************************************/
