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


#define __RADIOSIM_C__


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


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  C O D E
//******************************************************************************
extern RADIO_SYSTEM			TestRadioSystem;

//******************************************************************************
//
//	Radio Simulator Initialization Block
//
//******************************************************************************
void RadioSimInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	RADIO_SIM_OBJ_STRUCT *RadioSimObjStruct = (RADIO_SIM_OBJ_STRUCT *)BlockObjStruct;
	RADIO_SIM_OBJ_STRUCT *RadioSimInitStruct = (RADIO_SIM_OBJ_STRUCT *)BlockInitStruct;


	RadioSimObjStruct->modemIDofInput = RadioSimInitStruct->modemIDofInput;
	RadioSimObjStruct->RadioSystem = RadioSimInitStruct->RadioSystem;
	
	//
	// Reset modem to receive mode in radio system
	//
	(RadioSimObjStruct->RadioSystem)->ModemState[RadioSimInitStruct->modemIDofInput] = MODEM_RECEIVE_STATE;
}


//******************************************************************************
//
//	Radio Simulator Execution Block
//
//******************************************************************************
WORD RadioSim(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	RADIO_SIM_OBJ_STRUCT *RadioSimObjStruct = (RADIO_SIM_OBJ_STRUCT *)BlockObjStruct;
	RADIO_SYSTEM *RadioSystem;
	WORD ModemIDOfInput, ModemIDOfOutput;
	WORD modemIDCounter;
	RadioSystem = RadioSimObjStruct->RadioSystem;


	if (RadioSimObjStruct->modemIDofInput == MODEM_A)
	{
		ModemIDOfInput = MODEM_A;
		ModemIDOfOutput = MODEM_B;
	}
	else
	{
		ModemIDOfOutput = MODEM_A;
		ModemIDOfInput = MODEM_B;
	}

	//
	// Checks if transition time has elapsed
	//
 	for (modemIDCounter = 0; modemIDCounter < NUM_OF_MODEM_ID; modemIDCounter++)
 	{
		if ( (GetCurrentTimeMS() - RadioSystem->ModemTransitionStartTimeMS[modemIDCounter] ) >
			RadioSystem->ModemTransitionRandomDelayMS[modemIDCounter] )
		{
			if (RadioSystem->ModemState[modemIDCounter] == MODEM_TRANSITION_TO_RECEIVE_STATE)
			{
				RadioSystem->ModemState[modemIDCounter] = MODEM_RECEIVE_STATE;
			}
			else if (RadioSystem->ModemState[modemIDCounter] == MODEM_TRANSITION_TO_TRANSMIT_STATE)
			{
				RadioSystem->ModemState[modemIDCounter] = MODEM_TRANSMIT_STATE;				
			}
			
		}
			
 	}


	if (RadioSystem->ModemState[ModemIDOfInput] ==MODEM_TRANSMIT_STATE)
	{
		//
		// Channel Open, Point-To-Point Transmission open
		//
		if (RadioSystem->ModemState[ModemIDOfOutput] == MODEM_RECEIVE_STATE)
		{
			*inSize = MIN(*inSize, *outSize);
			*outSize = *inSize;
			
			//Sets transmission for  maximum amount of data available at input and output
			memcpy(outBuffer, inBuffer, *inSize * sizeof(WORD));
		}
		//
		// Modem at Input is Transmitting, but Modem at output is not in receive mode
		//
		else
		{
			// No output from block
			*outSize = 0;
		}
	}
	else //if Modem at input is in receive state
	{
		*inSize = 0; //no data is sent from input modem
		
		//
		// Modem at output is in receive mode, but no signal is being sent by input modem
		//
		if (RadioSystem->ModemState[ModemIDOfOutput] == MODEM_RECEIVE_STATE)
		{
			if (*outSize > 0)
			{
				*outBuffer = 0;
				*outSize = 1; //one 0 word is sent to output modem
			}
		}
		//
		// Modem at output is in transmit mode
		//
		else
		{
			*outSize = 0;
		}
			
	}

	
	return 0;
}

//******************************************************************************
//
//	Radio Simulator Ready Block
//
//******************************************************************************
WORD RadioSimReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*inputSize > 0 || *outputSize > 0)
	{
		return TRUE;
	}
	return FALSE;
}

//******************************************************************************
//
//	Radio Hold To Talk Simulation
//
//******************************************************************************
WORD RadioSimStartTx(MODEM_ID	modemID)
{
	TestRadioSystem.ModemState[modemID] = MODEM_TRANSITION_TO_TRANSMIT_STATE;
	TestRadioSystem.ModemTransitionRandomDelayMS[modemID] = 
		RandWord(RADIO_TRANS_TO_TRANSMIT_MIN_WAIT_MS, RADIO_TRANS_TO_TRANSMIT_MAX_WAIT_MS);
	TestRadioSystem.ModemTransitionStartTimeMS[modemID] = GetCurrentTimeMS();
	return TRUE;
}

//******************************************************************************
//
//	Radio Hold To Talk Simulation
//
//******************************************************************************
WORD RadioSimStartRx(MODEM_ID	modemID)
{
	TestRadioSystem.ModemState[modemID] = MODEM_TRANSITION_TO_RECEIVE_STATE;
	TestRadioSystem.ModemTransitionRandomDelayMS[modemID] = 
		RandWord(RADIO_TRANS_TO_RECEIVE_MIN_WAIT_MS, RADIO_TRANS_TO_RECEIVE_MAX_WAIT_MS);
	TestRadioSystem.ModemTransitionStartTimeMS[modemID] = GetCurrentTimeMS();
	return TRUE;
}


/***********************************  END  ************************************/
