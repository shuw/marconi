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


#define __RXGATE_C__


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


//******************************************************************************
//
//	Rx Gate Initialization Block
//
//******************************************************************************
void RxGateInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
#ifdef PLATFORM_SIM
	TXRX_GATES_OBJ_STRUCT * TxGateObjStruct = (TXRX_GATES_OBJ_STRUCT *)BlockObjStruct;
	RadioSimStartRx(TxGateObjStruct->modemID);
#endif
#ifdef PLATFORM_HW
	HwGpioSetPTT(FALSE);
#endif
}


//******************************************************************************
//
//	Rx Gate Execution Block
//
//******************************************************************************
WORD RxGate(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	TXRX_GATES_OBJ_STRUCT * TxGateObjStruct = (TXRX_GATES_OBJ_STRUCT *)BlockObjStruct;

	switch (TxGateObjStruct->currentState)
	{

            case (GATE_WAIT_TO_TURN_RECEIVE_ON_STATE):
              {
			if ((GetCurrentTimeMS() - TxGateObjStruct->transitionStartTimeMS ) > 
				RXGATE_TURN_RECEIVE_ON_WAIT_MS )
			{
                
#ifdef PLATFORM_SIM
                		RadioSimStartRx(TxGateObjStruct->modemID);
#endif
#ifdef PLATFORM_HW
                		HwGpioSetPTT(FALSE);
#endif
				TxGateObjStruct->currentState = GATE_TRANS_TO_RECEIVE_STATE;
			}
               break;
                }
		case (GATE_TRANS_TO_RECEIVE_STATE):
		{
			if ((GetCurrentTimeMS() - TxGateObjStruct->transitionStartTimeMS ) > 
				RXGATE_TRANSITION_TO_RECEIVE_WAIT_MS )
			{
				TxGateObjStruct->currentState = GATE_RECEIVE_STATE;

				//
				//TODO: dump the input buffer before
				//
				*outSize = 0;
//				return RETURN_OK;
			}
			break;
		}
		case (GATE_RECEIVE_STATE):
		{
			if (*inSize > 0)
			{
				*inSize = MIN(*inSize, *outSize);
				*outSize = *inSize;
				
				memcpy(outBuffer, inBuffer, *inSize * sizeof(WORD));

				return RETURN_OK;
			}			
			break;
		}
	}
	return RETURN_CONTROLLER_BLOCK_IDLE;
}

//******************************************************************************
//
//	Rx Gate Ready Block
//
//******************************************************************************
WORD RxGateReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	return TRUE;
}


/***********************************  END  ************************************/
