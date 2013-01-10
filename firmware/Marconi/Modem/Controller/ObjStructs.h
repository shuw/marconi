//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		This file contains the structures of the objects used by
//				controllers and pipelines
//
//
//
//******************************************************************************


#ifndef __OBJSTRUCTS_H__
	#define __OBJSTRUCTS_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define PIPELINE_OPTIONS_PRINT_DEBUG						BIT0
#define PIPELINE_OPTIONS_RUN_IN_MAIN_LOOP			BIT1


#define CONTROLLER_OPTIONS_PRINT_DEBUG						BIT0
#define CONTROLLER_OPTIONS_DISABLE						    BIT1
#define CONTROLLER_OPTIONS_PASSTHRU						    BIT2

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


typedef struct
{
	char 			*nameString;
	WORD			(*BlockFireFn)(WORD *inBuffer, WORD *inSizeB, WORD *outBuffer, WORD *outSizeB, BYTE *BlockObjStruct);
	WORD 			(*BlockReadyFn)(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);		//Number of packets to buffer for execution
	void 			(*BlockInitFn)(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
	
	BYTE 			*BlockObjStruct;
	BYTE 			*BlockInitStruct;


	WORD 			fifoSize;
 	WORD			fifoFillLevel;
	WORD 			*fifoStartAddress;
	WORD 			*fifoEndAddress;	
	WORD			*fifoPushPtr;
	WORD			*fifoPopPtr;
	WORD 			optionFlags;
} DATA_CONTROLLER;

typedef struct
{
	DATA_CONTROLLER *ControllerSelect;
	char *nameString;
	WORD (*BlockFireFn)(WORD *inBuffer, WORD *inSizeB, WORD *outBuffer, WORD *outSizeB, BYTE *BlockObjStruct);
	WORD (*BlockReadyFn)(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);
	void (*BlockInitFn)(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
	BYTE *BlockObjStruct;
	BYTE *BlockInitStruct;
	WORD *fifoBuffer;
	WORD fifoSizeB;
	WORD optionsFlags;
} DATA_CONTROLLER_INIT_STRUCT;

typedef struct
{
    char   *nameString;
    DWORD iterationCount;
    DATA_CONTROLLER  * ROM *Controllers;
    WORD optionsFlags;
    WORD allControllersIdle;
} PIPELINE;




typedef struct
{
	PIPELINE *PipelineSelect;
	char *nameString;
	DATA_CONTROLLER  * ROM * DataControllersList;
	WORD optionsFlags;	
} PIPELINE_INIT_STRUCT;

	

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
