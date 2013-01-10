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


#ifndef __RADIOSIM_H__
	#define __RADIOSIM_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define RADIO_SYMBOL_RATE_PER_S                                8000


#define RADIO_TRANS_TO_TRANSMIT_MAX_WAIT_MS	200
#define RADIO_TRANS_TO_TRANSMIT_MIN_WAIT_MS      50

#define RADIO_TRANS_TO_RECEIVE_MAX_WAIT_MS		200
#define RADIO_TRANS_TO_RECEIVE_MIN_WAIT_MS		50



#define RADIO_SYMBOL_RATE_PER_MS                                ((double)RADIO_SYMBOL_RATE_PER_S / (double)1000)
#define RADIO_RATE_CONTROL_SYMBOLS_PER_INTERVAL            40

//
// Modem is half duplex so receive and transmit states are mutually exclusive
//
typedef enum
{
	MODEM_TRANSMIT_STATE,
	MODEM_RECEIVE_STATE,
	MODEM_TRANSITION_TO_RECEIVE_STATE,
	MODEM_TRANSITION_TO_TRANSMIT_STATE,
	NUM_OF_MODEM_STATES
} MODEM_STATES;

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


typedef struct
{
	WORD ModemState[NUM_OF_MODEM_ID];
	WORD ModemTransitionRandomDelayMS[NUM_OF_MODEM_ID];
	DWORD ModemTransitionStartTimeMS[NUM_OF_MODEM_ID];
} RADIO_SYSTEM;


typedef struct
{
    MODEM_ID                modemIDofInput;
    DWORD                    rateControlStartTimeMs;
    
    RADIO_SYSTEM	*RadioSystem;
} RADIO_SIM_OBJ_STRUCT;

typedef struct
{
	MODEM_ID modemIDofInput;
	RADIO_SYSTEM	*RadioSystem;
} RADIO_INIT_OBJ_STRUCT;


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void RadioSimInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD RadioSim(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD RadioSimReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);
WORD RadioSimStartTx(MODEM_ID	modemID);
WORD RadioSimStartRx(MODEM_ID	modemID);




#endif
