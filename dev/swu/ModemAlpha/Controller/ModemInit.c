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


#define __MODEMINIT_C__


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
DATA_CONTROLLER_STRUCT	FileReaderController;
DATA_CONTROLLER_STRUCT	FileWriterController;
DATA_CONTROLLER_STRUCT	EncoderController;
DATA_CONTROLLER_STRUCT	VEncoderController;
DATA_CONTROLLER_STRUCT	DecoderController;
DATA_CONTROLLER_STRUCT	VDecoderController;

DATA_CONTROLLER_STRUCT	ModulatorController;
DATA_CONTROLLER_STRUCT	NoiseMediumAController;

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************

BYTE			encoderBuffer[SIZEOF_ENCODER_BUFFER];
BYTE			VEncoderBuffer[SIZEOF_VENCODER_BUFFER];
BYTE			decoderBuffer[SIZEOF_DECODER_BUFFER];
BYTE			VDecoderBuffer[SIZEOF_VDECODER_BUFFER];
BYTE			modulatorBuffer[SIZEOF_MODULATOR_BUFFER];
BYTE			noiseMediumABuffer[SIZEOF_NOISEMEDIUMA_BUFFER];
BYTE			fileWriterBuffer[SIZEOF_FILE_WRITER_BUFFER];




//******************************************************************************
//  C O D E
//******************************************************************************




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
void ModemControllersInit(void)
{
	//
	// File Reader Controller
	//
	DataControllerInit
	(
		&FileReaderController,
		"File Reader",
		&FileReader,
		FileReaderInputSizeB,
		FileReaderOutputSizeB,
		NULL,
		NULL,
		0
	);
	
	//
	// File Writer Controller
	//
	DataControllerInit
	(
		&FileWriterController,
		"File Writer",
		&FileWriter,
		FileWriterInputSizeB,
		FileWriterOutputSizeB,
		NULL,
		fileWriterBuffer,
		SIZEOF_FILE_WRITER_BUFFER
	);	

	
	//
	// Encoder Controller
	//
	DataControllerInit
	(
		&EncoderController,
		"Encoder",
		&Encoder,
		EncoderInputSizeB,
		EncoderOutputSizeB,
		NULL,
		encoderBuffer,
		SIZEOF_ENCODER_BUFFER		
	);

	//
	// Viterbi Encoder Controller
	//
	DataControllerInit
	(
		&VEncoderController,
		"Viterbi Encoder",
		&VEncoder,
		VEncoderInputSizeB,
		VEncoderOutputSizeB,
		NULL,
		VEncoderBuffer,
		SIZEOF_VENCODER_BUFFER		
	);
	
	//
	// Decoder Controller
	//
	DataControllerInit
	(
		&DecoderController,
		"Decoder",
		&Decoder,
		DecoderInputSizeB,
		DecoderOutputSizeB,
		NULL,
		decoderBuffer,
		SIZEOF_DECODER_BUFFER		
	);

	//
	// Viterbi Decoder Controller
	//
	DataControllerInit
	(
		&VDecoderController,
		"Viterbi Decoder",
		&VDecoder,
		VDecoderInputSizeB,
		VDecoderOutputSizeB,
		NULL,
		VDecoderBuffer,
		SIZEOF_VDECODER_BUFFER		
	);

	
	//
	// Modulator Controller
	//
	DataControllerInit
	(
		&ModulatorController,
		"Modulator",
		&Modulator,
		ModulatorInputSizeB,
		ModulatorOutputSizeB,
		NULL,
		modulatorBuffer,
		SIZEOF_MODULATOR_BUFFER
	);

	//
	// Noise Medium Type A
	//
	DataControllerInit
	(
		&NoiseMediumAController,
		"Noise Medium Type A",
		&NoiseMediumTypeA,
		&NoiseMediumTypeAInputSizeB,
		&NoiseMediumTypeAOutputSizeB,
		NULL,
		noiseMediumABuffer,
		SIZEOF_NOISEMEDIUMA_BUFFER
	);
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
void ModemPipelineInit(DATA_CONTROLLER_STRUCT *Pipeline[])
{
	BYTE counter;

	
	DataControllerPipelineInit(Pipeline[0], CONTROLLER_PIPELINE_HEAD, Pipeline[1]);

	for (counter = 1; Pipeline[counter] != NULL; counter++)
	{
		if (Pipeline[counter + 1] != NULL)
		{
			DataControllerPipelineInit(Pipeline[counter], CONTROLLER_PIPELINE_MIDDLE, Pipeline[counter + 1]);
		}
		else
		{
			DataControllerPipelineInit(Pipeline[counter], CONTROLLER_PIPELINE_TAIL, NULL);
		}
	}
}

/***********************************  END  ************************************/
