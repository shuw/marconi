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


#define __ERRORCHECKCMDS_C__


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
static void SetLiveComparisonAction(SDWORD parameters[]);
static void StatisticalSummaryAction(SDWORD parameters[]);


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
static ROM DEBUG_CMD_STRUCT	ErrorCheckCmdArray[] =
{
	{
		"setCompare", 
		"param0 - turn on/off live comparison",
		1,
		SetLiveComparisonAction,
		NULL
	},
	{
		"stats", 
		"prints collected statistics",
		0,
		StatisticalSummaryAction,
		NULL
	}
};


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
ROM CMD_LIST_STRUCT ErrorCheckCmdList =
{
	ErrorCheckCmdArray,
	NumOfCmdsInList(ErrorCheckCmdArray)
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
void SetLiveComparisonAction(SDWORD parameters[])
{
	ERROR_CHECK_OBJ_STRUCT *ErrorCheckObjStruct = 
		(ERROR_CHECK_OBJ_STRUCT *)ErrorCheckDestinationController.BlockObjStruct;
	
	if (parameters[0] == 1)
	{
		ErrorCheckObjStruct->optionsFlags |= ERROR_CHECK_LIVE_COMPARISON_FLAG;
	}
	else
	{
		ErrorCheckObjStruct->optionsFlags &= ~ERROR_CHECK_LIVE_COMPARISON_FLAG;
	}
}

//******************************************************************************
//
//		DEBUG COMMAND
//
//******************************************************************************
static void StatisticalSummaryAction(SDWORD parameters[])
{
	ErrorCheckPrintStatistics(ErrorCheckDestinationController.BlockObjStruct);
}


/***********************************  END  ************************************/
