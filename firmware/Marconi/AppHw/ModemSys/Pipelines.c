//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		Creates the modem pipelines
//
//
//
//******************************************************************************


#define __PIPELINES_C__


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


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************

PIPELINE HwTestPipeline;
PIPELINE DataGenTxPipeline;
PIPELINE ChatTxPipeline;
PIPELINE DataCheckRxPipeline;
PIPELINE RxPipeline;

//******************************************************************************
//  C O D E
//******************************************************************************


DATA_CONTROLLER* HwTestPipelineControllersList[MAX_SIZE_OF_TEST_PIPELINE + 1] = 
{
	//&DataGeneratorController,
	//&WordToByteController,
	//&ErrorCheckSourceController,
	//&PacketCreaterController,
	//&VEncoderController,
	//&TestModulatorController,
	//&FSKModulatorController,
	//&FSKDemodulatorController,
	//&VDecoderController,
	//&PacketParserController,
	//&ErrorCheckDestinationController,
	//&SerialOutputController,
    &DataGeneratorController,
    &WordToByteController,
    &BurstTxController,
    &VEncoderController,
    &FSKModulatorController,
    &McBspOutputController,
    &McBspInputController,
    &FSKDemodulatorController,
    &VDecoderController,
    &BurstRxController,
    &SerialOutputController, 
	NULL
};


DATA_CONTROLLER * ROM ChatTxControllers[] = 
{
	//&ChatController,
//	&WordToByteController,
	//&PacketCreaterController,
    &SerialInputController,
    &BurstTxController,
	&TxGateController,
	&VEncoderController,
	&FSKModulatorController,
	&McBspOutputController,
	NULL
};

DATA_CONTROLLER * ROM DataGenTxControllers[] = 
{
	&DataGeneratorController,
    &WordToByteController,
    &PacketCreaterController,
	&TxGateController,
	&VEncoderController,
	&FSKModulatorController,
	&McBspOutputController,
	NULL
};

DATA_CONTROLLER * ROM DataCheckRxControllers[] =
{
	&McBspInputController,
	&RxGateController,
	&FSKDemodulatorController,
	&VDecoderController,
    &PacketParserController,
    &ByteToWordController,
	&DataCheckController,
	&SerialOutputController,
	NULL
};

DATA_CONTROLLER * ROM RxControllers[] =
{
	&McBspInputController,
	&RxGateController,
	&FSKDemodulatorController,
	&VDecoderController,
	//&PacketParserController,
    &BurstRxController,
	&SerialOutputController,
	NULL
};


ROM PIPELINE_INIT_STRUCT PipelinesInstantiationList[NUM_OF_PIPELINES] =
{
	{
		&HwTestPipeline,
		"Hw test pipeline",
		HwTestPipelineControllersList,
		0
	},
	{
		&DataGenTxPipeline,
		"DataGen Tx Pipeline",
		DataGenTxControllers,
		0
	},
	{
		&ChatTxPipeline,
		"Chat Tx Pipeline",
		ChatTxControllers,
	    PIPELINE_OPTIONS_RUN_IN_MAIN_LOOP	
	},
	{
		&DataCheckRxPipeline,
		"DataCheck Rx Pipeline",
		DataCheckRxControllers,
		0
	},
	{
		&RxPipeline,
		"Rx Pipeline",
		RxControllers,
	    PIPELINE_OPTIONS_RUN_IN_MAIN_LOOP	
	}
};


/***********************************  END  ************************************/
