//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		Modem Skeleton Main
//
//
//
//******************************************************************************


#define __MODEM_MAIN_C__


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
static void FirePipeline(DATA_CONTROLLER_STRUCT *Pipeline[]);

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
BOOL globalEndProgram = FALSE;


DATA_CONTROLLER_STRUCT *ModemTransmitPipeline[] =
{
	&FileReaderController, 
//	&EncoderController, 
	&VEncoderController, 
//	&ModulatorController, 
//	&NoiseMediumAController,
//	&DecoderController,
	&VDecoderController,
	&FileWriterController,
	NULL
};

//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


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
int main(void)
{
	outputCaptureStream = fopen("debugOut.txt" , "w+");
	PrintMsg("Marconi Modem Alpha\n\n\n");	

	ModemControllersInit();
	ModemPipelineInit(ModemTransmitPipeline);
	VDecoderInit();
	
	FileReaderInit("encodertest2.txt", HEX_FILE);
	FileWriterInit("encoderOutput.txt", HEX_FILE);

//	FileReaderInit("Vtest.txt", BINARYSTRING_FILE);
//	FileWriterInit("Vout.txt", BINARYSTRING_FILE);


	while (TRUE && !globalEndProgram)
	{
		
		FirePipeline(ModemTransmitPipeline);
	}

	PrintMsg("\n\n\nProgram terminated\n");
	return 0;
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
static void FirePipeline(DATA_CONTROLLER_STRUCT *Pipeline[])
{
	BYTE counter;
	for (counter = 0; Pipeline[counter] != NULL; counter++)
	{
		DataControllerFire(Pipeline[counter]);
	}
}


/***********************************  END  ************************************/
