//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		This file contains debug functions which are controlled by the serial port
//
//
//
//******************************************************************************


#ifndef __DEBUGCMDS_H__
#define __DEBUGCMDS_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define MAX_NUM_DEBUG_PARAMETERS 		12

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef struct
{
	char 	* cmdString;				//Non Case Sensitive Command Names
	char	* cmdDescription;
	BYTE 	numOfCmdParameters;	//Max num of parameters set by MAX_NUM_DEBUG_PARAMETERS
	void  	(*CmdFunctionPtr)(SDWORD parameters[]);
	BYTE	*subCmdList;
} DEBUG_CMD_STRUCT;

typedef struct
{
	ROM DEBUG_CMD_STRUCT *	debugCmdList;
	BYTE 					numOfCmdsInList;
} CMD_LIST_STRUCT;

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

	
//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern ROM CMD_LIST_STRUCT TopDebugCmdList;

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void TestCmdAction(SDWORD parameters[]);
void ReadCmdAction(SDWORD parameters[]);
void WriteCmdAction(SDWORD parameters[]);
void TestParamCmdAction(SDWORD parameters[]);
void readDumpCmdAction(SDWORD parameters[]);
void debugModeCmdAction(SDWORD parameters[]);
void writeDumpCmdAction(SDWORD parameters[]);
void getTimeCmdAction(SDWORD parameters[]);



#endif
