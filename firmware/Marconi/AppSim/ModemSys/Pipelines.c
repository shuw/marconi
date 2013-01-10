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

PIPELINE ModemATxPipeline;
PIPELINE ModemBTxPipeline;
PIPELINE ModemARxPipeline;
PIPELINE ModemBRxPipeline;
PIPELINE RadioSimAtoBPipeline;
PIPELINE RadioSimBtoAPipeline;
PIPELINE ViterbiModulatorPipeline;
PIPELINE DemodulatorViterbiPipeline;
PIPELINE ModulatorPipeline;
PIPELINE DemodulatorPipeline;
PIPELINE BurstTxPipeline;
PIPELINE BurstRxPipeline;
PIPELINE TestTxPipeline;
PIPELINE TestRxPipeline;

//******************************************************************************
//  C O D E
//******************************************************************************
DATA_CONTROLLER* ROM ModemATxControllersList[] = 
{
//	&FileReaderControllerA,
	&DataGeneratorController, 
//	&WordToByteControllerA,
//	&ErrorCheckSourceControllerA,
//	&PacketCreaterControllerA,
	&TxGateControllerA,
	&VEncoderControllerA,
	&FSKModulatorControllerA,
//	&TestModulatorControllerA,
	&PassThruAtoRadioSimController,
	NULL
};

DATA_CONTROLLER* ROM ModemBTxControllersList[] = 
{
//	&DataGeneratorController,
	&WordToByteControllerB,
	&ErrorCheckSourceControllerB,
	&PacketCreaterControllerB,
//	&VEncoderControllerB,
	&TxGateControllerB,
//	&TestModulatorControllerB,
	&FSKModulatorControllerB,
	&PassThruBtoRadioSimController,
	NULL
};

DATA_CONTROLLER* ROM ModemARxControllersList[] = 
{
	&PassThruRadioSimToAController,
	&RxGateControllerA,
	&FSKDemodulatorControllerA,
	&VDecoderControllerA,
//    &TestDecoderControllerA,
//	&SyncRxController,
	&PacketParserControllerA,
	&ErrorCheckDestinationControllerB,
	&FileWriterControllerA,
	NULL
};

DATA_CONTROLLER* ROM ModemBRxControllersList[] = 
{
	&PassThruRadioSimToBController,
	&RxGateControllerB,
	&FSKDemodulatorControllerB,
//	&TestDecoderControllerB,
	&VDecoderControllerB,
//	&PacketParserControllerB,
	&DataCheckController,
//	&ErrorCheckDestinationControllerA,
	&FileWriterControllerB,
	NULL
};



DATA_CONTROLLER* ROM RadioSimAtoB[] = 
{
	&PassThruAtoRadioSimController,
	&AWGNChannelControllerAtoB,
	&RadioSimControllerAtoB,
	&PassThruRadioSimToBController,
	NULL
};

DATA_CONTROLLER* ROM RadioSimBtoA[] = 
{
	&PassThruBtoRadioSimController,
//	&AWGNChannelControllerBtoA,
	&RadioSimControllerBtoA,
	&PassThruRadioSimToAController,
	NULL
};


DATA_CONTROLLER *ViterbiModulatorControllers[] = {
    &FileReaderControllerA,
    &VEncoderControllerB,
    &FSKModulatorControllerB,
    &FileWriterControllerA,
    NULL
};


DATA_CONTROLLER *DemodulatorViterbiControllers[] = {
    &FileReaderControllerA,
    &FSKDemodulatorControllerA,
    &VDecoderControllerA,
    &FileWriterControllerA,
    NULL
};

DATA_CONTROLLER *ModulatorControllers[] = {
    &FileReaderControllerA,
    &FSKModulatorControllerB,
    &FileWriterControllerA,
    NULL
};


DATA_CONTROLLER *DemodulatorControllers[] = {
    &FileReaderControllerA,
    &FSKDemodulatorControllerA,
    &TestDecoderControllerA,
    &FileWriterControllerA,
    NULL
};

DATA_CONTROLLER *BurstTxControllers[] = {
    &FileReaderControllerA,
    &BurstTxControllerA,
    &FileWriterControllerA,
    NULL
};

DATA_CONTROLLER *BurstRxControllers[] = {
    &FileReaderControllerA,
    &BurstRxControllerA,
    &FileWriterControllerA,
    NULL
};

DATA_CONTROLLER *TestTxControllers[] = {
    &FileReaderControllerA,
    &BurstTxControllerA,
    &VEncoderControllerA,
    &FSKModulatorControllerA,
    &FileWriterControllerA,
    NULL
};

DATA_CONTROLLER *TestRxControllers[] = {
    &FileReaderControllerA,
    &FSKDemodulatorControllerA,
    &VDecoderControllerA,
    &BurstRxControllerA,
    &FileWriterControllerA,
    NULL
};


//
//
ROM PIPELINE_INIT_STRUCT PipelinesInstantiationList[] =
{
	{
		&ModemATxPipeline,
		"Modem A Tx",
		ModemATxControllersList,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
	{
		&ModemBTxPipeline,
		"Modem B Tx",
		ModemBTxControllersList,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
	{
		&ModemARxPipeline,
		"Modem A Rx",
		ModemARxControllersList,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
	{
		&ModemBRxPipeline,
		"Modem B Rx",
		ModemBRxControllersList,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
	{
		&RadioSimAtoBPipeline,
		"Radiosim A to B",
		RadioSimAtoB,
		0
	},
	{
		&RadioSimBtoAPipeline,
		"Radiosim B to A",
		RadioSimBtoA,
		0
	},
	{
		&ViterbiModulatorPipeline,
		"Viterbi+Modulator Test Pipeline",
		ViterbiModulatorControllers,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
    {
		&DemodulatorViterbiPipeline,
		"Demodulator+Viterbi Test Pipeline",
		DemodulatorViterbiControllers,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
    {
		&ModulatorPipeline,
		"Modulator Test Pipeline",
		ModulatorControllers,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
    {
		&DemodulatorPipeline,
		"Demodulator Test Pipeline",
		DemodulatorControllers,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
    {
		&BurstTxPipeline,
		"BurstTx Test Pipeline",
		BurstTxControllers,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
    {
		&BurstRxPipeline,
		"BurstRx Test Pipeline",
		BurstRxControllers,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
    {
		&TestTxPipeline,
		"TestTx Test Pipeline",
		TestTxControllers,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},
    {
		&TestRxPipeline,
		"TestRx Test Pipeline",
		TestRxControllers,
		0
//		| PIPELINE_OPTIONS_PRINT_DEBUG
	},

};



/***********************************  END  ************************************/
