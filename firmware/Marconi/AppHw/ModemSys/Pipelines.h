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


#ifndef __PIPELINES_H__
	#define __PIPELINES_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define MAX_SIZE_OF_TEST_PIPELINE		15

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef enum
{
	TEST_PIPELINE_ID,
	DATAGEN_TX_PIPELINE_ID,	
	CHAT_TX_PIPELINE_ID,
	DATACHECK_RX_PIPELINE_ID,
	RX_PIPELINE_ID,
	NUM_OF_PIPELINES
} PIPELINES_LIST;

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern PIPELINE HwTestPipeline;

extern DATA_CONTROLLER* HwTestPipelineControllersList[MAX_SIZE_OF_TEST_PIPELINE + 1] ;
extern ROM PIPELINE_INIT_STRUCT PipelinesInstantiationList[];


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
