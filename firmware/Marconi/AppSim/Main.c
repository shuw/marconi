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


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************
extern FILE_READER_OBJ_STRUCT		FileReaderObjStructA;
extern PACKET_SHARED_OBJ_STRUCT 		PacketSharedObjStructA;
extern PACKET_SHARED_OBJ_STRUCT 		PacketSharedObjStructB;
//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
static void PrintPacketDebugInfo(void);

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
	WORD counter = 0;
	debugFileCaptureStream = fopen(DEBUG_OUT_FILE_NAME , "w+");
	
	ModemInit();
#ifdef USE_TESTER_MAIN
		TesterMain();
#else
		while ( TRUE )
		{
			PipelineFire(&ModemATxPipeline);
			PipelineFire(&RadioSimAtoBPipeline);
			PipelineFire(&ModemBRxPipeline);

			PipelineFire(&ModemBTxPipeline);
			PipelineFire(&RadioSimBtoAPipeline);
			PipelineFire(&ModemARxPipeline);
//			PrintPacketDebugInfo();
		}

		ErrorCheckPrintStatistics(ErrorCheckDestinationControllerA.BlockObjStruct);

	#endif
	
	DebugMsg("\n\n\nProgram terminated\n");
	return 0;
}

static void PrintPacketDebugInfo(void)
{
#ifdef DEBUG_PACKETER
	static DWORD prevModemAPacketsTransmitted;
	static DWORD prevModemBPacketsTransmitted;

	if ( (prevModemAPacketsTransmitted != PacketSharedObjStructA.CreaterStruct.packetsTransmitted) ||
		(prevModemBPacketsTransmitted != PacketSharedObjStructB.CreaterStruct.packetsTransmitted)  )
	{
		DebugMsg("\n\rModem A: ");
		DebugMsgDW("Tx = %lu ", PacketSharedObjStructA.CreaterStruct.packetsTransmitted);
		DebugMsgDW("Rx = %lu ", PacketSharedObjStructA.ParserStruct.numOfPacketsReceived);
		DebugMsgDW("S = %lu ", PacketSharedObjStructA.ParserStruct.numOfSyncDetected);
		DebugMsg(" Modem B: ");
		DebugMsgDW("Tx = %lu ", PacketSharedObjStructB.CreaterStruct.packetsTransmitted);
		DebugMsgDW("Rx = %lu ", PacketSharedObjStructB.ParserStruct.numOfPacketsReceived);
		DebugMsgDW("S = %lu ", PacketSharedObjStructB.ParserStruct.numOfSyncDetected);	

		prevModemAPacketsTransmitted = PacketSharedObjStructA.CreaterStruct.packetsTransmitted;
		prevModemBPacketsTransmitted = PacketSharedObjStructB.CreaterStruct.packetsTransmitted;
	}
#endif

}

/***********************************  END  ************************************/
