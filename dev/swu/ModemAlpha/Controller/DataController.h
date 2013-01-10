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


#ifndef __DATACONTROLLER_H__
	#define __DATACONTROLLER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define		SIZEOF_CONTROLLER_INPUT_BUFFER			128
#define		SIZEOF_CONTROLLER_OUTPUT_BUFFER		128

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
typedef enum
{
	CONTROLLER_PIPELINE_TAIL,	//No Output
	CONTROLLER_PIPELINE_HEAD, //Always Execute even if no input
	CONTROLLER_PIPELINE_MIDDLE,
	CONTROLLER_PIPELINE_ALWAYSFIRE
} PIPELINE_CONTROLLER_TYPE;

typedef enum
{
	OK = 0,
	RESET_PIPELINE_DOWN_CONTROLLER
} CONTROLLER_BLOCK_RETURN_TYPE;

typedef struct
{
	BYTE 			*nameString;
	PIPELINE_CONTROLLER_TYPE	pipelinePosition;
	BYTE			(*BlockFire)(BYTE *inBuffer, BYTE *inSizeB, BYTE *outBuffer, BYTE *outSizeB);
	
	BYTE 			(*BlockInputSizeB)(void);		//Number of packets to buffer for execution
	BYTE 			(*BlockOutputSizeB)(void);
	void 			(*BlockReset)(void);

	BYTE 			*NextDataController;

	WORD 			fifoSizeB;
 	WORD			fifoFillLevelB;
	BYTE 			*fifoStartAddress;
	BYTE 			*fifoEndAddress;	
	BYTE			*fifoPushPtr;
	BYTE			*fifoPopPtr;
} DATA_CONTROLLER_STRUCT;



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
BOOL DataControllerInit( DATA_CONTROLLER_STRUCT *ControllerSelect, 
							BYTE *nameString,
							BYTE (*BlockFire)(BYTE *inBuffer, BYTE *inSizeB, BYTE *outBuffer, BYTE *outSizeB),
							BYTE (*BlockInputSizeB)(void), 
							BYTE (*BlockOutputSizeB)(void) ,
							void (*BlockReset)(void),							
							BYTE *fifoBuffer,
							WORD fifoSizeB );
BOOL DataControllerPipelineInit( DATA_CONTROLLER_STRUCT *ControllerSelect,
							PIPELINE_CONTROLLER_TYPE controllerType,
							DATA_CONTROLLER_STRUCT * NextDataController );
BOOL DataControllerFire(DATA_CONTROLLER_STRUCT *ControllerSelect);
void DataControllerResetPipeline(DATA_CONTROLLER_STRUCT *ControllerSelect);

BOOL DataControllerPushBuffer(DATA_CONTROLLER_STRUCT *ControllerSelect, BYTE *buffer, BYTE bufferSizeB);
WORD DataControllerRemainingSizeB(DATA_CONTROLLER_STRUCT *ControllerSelect);



#endif
