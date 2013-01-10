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


#define __TXGATE_C__


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
//	Tx Gate Initialization Block
//
//******************************************************************************
void TxGateInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	TXRX_GATES_OBJ_STRUCT * TxGateObjStruct = (TXRX_GATES_OBJ_STRUCT *)BlockObjStruct;
	TXRX_GATES_INIT_STRUCT * TxGateInitStruct = (TXRX_GATES_INIT_STRUCT *)BlockInitStruct;

	TxGateObjStruct->modemID = TxGateInitStruct->modemID;
	TxGateObjStruct->transmitPipelineIdleHysterisis = 0;
	TxGateObjStruct->currentState = GATE_RECEIVE_STATE;
	TxGateObjStruct->prevState = 0xFFFF;
}


static void TxLEDStatus(GATE_STATES state)
{
#ifdef PLATFORM_HW
    HwGpioClear(HW_GPIO_ID_A, 0xFC);
    if(state <= 5) {
        HwGpioSet(HW_GPIO_ID_A, 1 << (state + 2));
    }
#endif
}


//******************************************************************************
//
//	Tx Gate Execution Block
//
//******************************************************************************
WORD TxGate(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	TXRX_GATES_OBJ_STRUCT * GateObjStruct = (TXRX_GATES_OBJ_STRUCT *)BlockObjStruct;

//#ifdef DEBUG_TXGATE
       if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
       {
        	if (GateObjStruct->prevState != GateObjStruct->currentState)
        	{
//                    if (GateObjStruct->currentState == GATE_TRANSMIT_STATE)
//                    {
                        DebugMsgW("\n\rGate State = %d - ", (WORD)GateObjStruct->currentState);
                        DebugMsg(CurrentControllerPtr->nameString);
                        DebugMsgDW(" @%lu", GetCurrentTimeMS() );
//                    }
                    GateObjStruct->prevState = GateObjStruct->currentState;
        	}

       }

//#endif

    TxLEDStatus(GateObjStruct->currentState);

	switch (GateObjStruct->currentState)
	{
		case (GATE_TRANS_TO_RECEIVE_STATE):
		case (GATE_RECEIVE_STATE):
		{			
			//Input detected, switch to transmit
			if (*inSize >0)
			{
#ifdef PLATFORM_SIM				
				RadioSimStartTx(GateObjStruct->modemID);
#endif
#ifdef PLATFORM_HW
				HwGpioSetPTT(TRUE);
#endif
				GateObjStruct->currentState = GATE_TRANS_TO_TRANSMIT_STATE;
				GateObjStruct->transitionStartTimeMS = GetCurrentTimeMS();
			}


			break;
		}
		case (GATE_TRANS_TO_TRANSMIT_STATE):
		{
			if ((GetCurrentTimeMS() - GateObjStruct->transitionStartTimeMS) > 
				TXGATE_TRANSITION_TO_TRANSMIT_WAIT_MS && *outSize >= 8 )
			{
				GateObjStruct->currentState = GATE_TRANSMIT_STATE;
				//
				// Inserts empty word at beginning of transmission
				//
                *inSize = 0;
				*outSize = 8;
                *outBuffer++ = 45295;
                *outBuffer++ = 60807;
                *outBuffer++ = 26305;       
                *outBuffer++ = 46268;
                *outBuffer++ = 45295;
                *outBuffer++ = 60807;
                *outBuffer++ = 45295;
                *outBuffer++ = 60807;

                return RETURN_OK;
			}

			break;
		}			
		case (GATE_TRANSMIT_STATE):
		{
			
			if (*inSize > 0)
			{
				*inSize = MIN(*inSize, *outSize);
				*outSize = *inSize;
				
				memcpy(outBuffer, inBuffer, *inSize * sizeof(WORD));
				
				return RETURN_OK;
			}
			//
			// No data at input
			//
			else
			{
				if (IsCurrentPipelineIdle() && IsCurrentPipelineEmptyDownStream())
				{
					if ( (GateObjStruct->transmitPipelineIdleHysterisis++) > TXGATE_TRANSMIT_PIPELINE_IDLE_HYSTERISIS )
					{
						//
						// Inserts empty word at end of transmission
						//
						*outSize = 1;
						*outBuffer = 0;
						
						GateObjStruct->transmitPipelineIdleHysterisis = 0;
						GateObjStruct->currentState = GATE_TRANSMIT_END_STATE;

						return RETURN_OK;
					}
				}

			}
			break;
		}
		case (GATE_TRANSMIT_END_STATE):
		{
			if (IsCurrentPipelineEmptyDownStream())
			{
				GateObjStruct->transitionStartTimeMS = GetCurrentTimeMS();
				GateObjStruct->currentState = GATE_WAIT_TO_TURN_RECEIVE_ON_STATE;
			}
			break;
		}

	}

	return RETURN_CONTROLLER_BLOCK_IDLE;
}

//******************************************************************************
//
//	Tx Gate Ready Block
//
//******************************************************************************
WORD TxGateReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*outputSize > 0)
	{
		return TRUE;
	}
	
	return FALSE;
}


/***********************************  END  ************************************/
