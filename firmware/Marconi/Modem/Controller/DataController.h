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


#define		SIZEOF_CONTROLLER_INPUT_BUFFER			256
#define		SIZEOF_CONTROLLER_OUTPUT_BUFFER		256

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

typedef enum
{
	RETURN_OK = 0,
	RETURN_CONTROLLER_BLOCK_IDLE,
	RETURN_RESET_PIPELINE
} CONTROLLER_BLOCK_RETURN_TYPE;




//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
BOOL DataControllerInit(ROM DATA_CONTROLLER_INIT_STRUCT *ControllerInit);
BOOL DataControllerFire(DATA_CONTROLLER *ControllerSelect, DATA_CONTROLLER *NextController);

BOOL DataControllerPushBuffer(DATA_CONTROLLER *ControllerSelect, WORD *buffer, WORD bufferSizeB);
WORD DataControllerRemainingSizeB(DATA_CONTROLLER *ControllerSelect);

#endif
