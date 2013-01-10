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


#ifndef __MODEMINIT_H__
	#define __MODEMINIT_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define	SIZEOF_ENCODER_BUFFER			32
#define	SIZEOF_VENCODER_BUFFER			32
#define	SIZEOF_DECODER_BUFFER			32
#define	SIZEOF_VDECODER_BUFFER			32
#define	SIZEOF_MODULATOR_BUFFER			128
#define	SIZEOF_NOISEMEDIUMA_BUFFER		4
#define	SIZEOF_FILE_WRITER_BUFFER		16






//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************
extern 		DATA_CONTROLLER_STRUCT	FileReaderController;
extern 		DATA_CONTROLLER_STRUCT	FileWriterController;
extern 		DATA_CONTROLLER_STRUCT	EncoderController;
extern 		DATA_CONTROLLER_STRUCT	VEncoderController;
extern 		DATA_CONTROLLER_STRUCT	DecoderController;
extern 		DATA_CONTROLLER_STRUCT	VDecoderController;
extern 		DATA_CONTROLLER_STRUCT	ModulatorController;
extern 		DATA_CONTROLLER_STRUCT	NoiseMediumAController;
extern 		DATA_CONTROLLER_STRUCT	 *ModemTransmitPipeline[];

//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void ModemControllersInit(void);
void ModemPipelineInit(DATA_CONTROLLER_STRUCT *ModemTransmitPipeline[]);

#endif
