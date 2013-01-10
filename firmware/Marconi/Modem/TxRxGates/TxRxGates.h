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


#ifndef __TXRXGATES_H__
	#define __TXRXGATES_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
#define TXGATE_TRANSITION_TO_TRANSMIT_WAIT_MS	    300	
#define TXGATE_TRANSMIT_PIPELINE_IDLE_HYSTERISIS	10

#define RXGATE_TRANSITION_TO_RECEIVE_WAIT_MS		100
#define RXGATE_TURN_RECEIVE_ON_WAIT_MS              50

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************

typedef enum
{
	GATE_RECEIVE_STATE,
	GATE_TRANS_TO_TRANSMIT_STATE,
	GATE_TRANSMIT_STATE,
	GATE_TRANSMIT_END_STATE,
	GATE_WAIT_TO_TURN_RECEIVE_ON_STATE,
	GATE_TRANS_TO_RECEIVE_STATE,
	NUM_OF_GATE_STATES
} GATE_STATES;

typedef struct
{
	MODEM_ID modemID;
} TXRX_GATES_INIT_STRUCT;

typedef struct
{
	MODEM_ID 		modemID;
	WORD 			transmitPipelineIdleHysterisis;
	GATE_STATES		currentState;
	WORD		prevState;	
	DWORD 			transitionStartTimeMS;
} TXRX_GATES_OBJ_STRUCT;


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void TxGateInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD TxGate(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD TxGateReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);

void RxGateInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD RxGate(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD RxGateReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);

#endif
