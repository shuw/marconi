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


#define __TESTERMAIN_C__


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
extern FILE_READER_OBJ_STRUCT		FileReaderObjStructA;

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
void TesterMain(void)
{
	BYTE testIterationCounter = 0;
	AWGN_CHANNEL_INIT_STRUCT *ChannelInitStruct = 
			(AWGN_CHANNEL_INIT_STRUCT *)AWGNChannelController.BlockInitStruct;
	
	for (ChannelInitStruct->SignalToNoiseDB= TESTER_INITIAL_SNR_DB;
		ChannelInitStruct->SignalToNoiseDB <= TESTER_FINAL_SNR_DB;
		ChannelInitStruct->SignalToNoiseDB += ( (TESTER_FINAL_SNR_DB - TESTER_INITIAL_SNR_DB) / TESTER_NUMBER_OF_STEPS_SNR_DB))
	{
		WORD counter = 0;


		
		DebugMsgW("\n\n*** Tester Iteration %d ***\n", testIterationCounter);
		DebugMsgFloat("\nSNR in dB = %f\n", ChannelInitStruct->SignalToNoiseDB);



		while (!(FileReaderObjStruct.EOF_reached && (counter++ > PIPELINE_FLUSH_CYCLES)))
		{
<<<<<<< .mine
			PipelineFire(&ModemATxPipeline);
=======
			DataControllerFirePipeline(&ModemATxPipeline);
>>>>>>> .r366
		}

		ErrorCheckPrintStatistics(ErrorCheckDestinationController.BlockObjStruct);
		testIterationCounter++;		

		InitializePipelines(); //re-initializes all pipelines for next run
	}


}

/***********************************  END  ************************************/
