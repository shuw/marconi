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


#define __PACKETCREATER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"
#include <string.h>
#include <stdlib.h>


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static WORD CalculateChecksum(WORD * Bp_Buffer, WORD B_Size);

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
// FUNCTION:        Packet Creater Initialization
//
// USAGE:             Initializes Packet Creater static variables
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void PacketCreaterInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	PACKET_SHARED_OBJ_STRUCT *PacketSharedObjStruct = (PACKET_SHARED_OBJ_STRUCT *)BlockObjStruct;
	PACKET_SHARED_INIT_STRUCT *PacketSharedInitStruct = (PACKET_SHARED_INIT_STRUCT *)BlockInitStruct;
	
	PacketSharedObjStruct->CreaterStruct.currentState = PC_TRANSMIT_PACKET_STATE;
	PacketSharedObjStruct->CreaterStruct.currentPacketID = 0;
	PacketSharedObjStruct->CreaterStruct.packetsTransmitted = 0;
	PacketSharedObjStruct->seekAckEnabled = PacketSharedInitStruct->seekAckEnabled;
		
	PacketSharedObjStruct->SEND_ACK_ID= 0;
	PacketSharedObjStruct->SEND_ACK_Flag = FALSE;
#ifdef DEBUG_PACKETER
	PacketSharedObjStruct->CreaterStruct.numOfPacketsCreated = 0;
#endif
	
}


//******************************************************************************
//
// FUNCTION:        Packet Creater Block
//
// USAGE:             To create packeting for transmitted data
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
// TODO: packet creater state machine to loop to allow multiple packet creation
//
// TODO: Packet Creater must always be ready even when no input data,
//  may need to resend packet
//  create new IDLE_STATE -> waiting for input
//

WORD PacketCreater(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	PACKET_SHARED_OBJ_STRUCT *PacketSharedObjStruct = (PACKET_SHARED_OBJ_STRUCT *)BlockObjStruct;
	PACKET_CREATER_OBJ_STRUCT *PacketCreaterObjStruct = &(PacketSharedObjStruct->CreaterStruct);
	WORD *packetPtr = outBuffer;
	WORD *packetHeadPtr = outBuffer;
	WORD packetSize = 0;
	WORD counter;
	WORD inputSize = *inSize;

	//
	// This block does not take in any data until packet is successfully transmitted
	//
	*inSize = 0;
#ifdef INSERT_DUMMY_WORDS
	*packetPtr++ = 0;
	packetHeadPtr++;
#endif

	switch (PacketCreaterObjStruct->currentState)
	{
		case (PC_NEW_PACKET_STATE):
		{
			*inSize = DEFAULT_PACKET_DATA_SIZE_B;
			*outSize = 0;
			PacketCreaterObjStruct->currentPacketID++;
			//
			// Wrap around packet ID
			//
			if ((unsigned)PacketCreaterObjStruct->currentPacketID > PACKET_HEADER_ID_MASK)
				PacketCreaterObjStruct->currentPacketID = 0;


			PacketCreaterObjStruct->currentState = PC_TRANSMIT_PACKET_STATE;
			break;
		}
		case (PC_TRANSMIT_PACKET_STATE):
		{
			PacketCreaterObjStruct->packetsTransmitted++;


			//packet ID, used to find dropped packets
			*packetPtr = 0;

			//add sync to packet header
			SetBitsInWord(*packetPtr, 
				PACKET_HEADER_SYNC, PACKET_HEADER_SYNC_MASK, PACKET_HEADER_SYNC_POSITION);

			//add packet ID to packet header
			SetBitsInWord(*packetPtr, PacketCreaterObjStruct->currentPacketID, 
				PACKET_HEADER_ID_MASK, PACKET_HEADER_ID_POSITION);




			
			// 
			// Create secondary header for acknlowedgement
			//
			if (PacketSharedObjStruct->SEND_ACK_Flag && PacketSharedObjStruct->seekAckEnabled)
			{
				PacketSharedObjStruct->SEND_ACK_Flag = 0;
				
				//sets the aux header flag
				SetBitsInWord(*packetPtr, 1, 
					PACKET_HEADER_AUX_BIT_MASK, PACKET_HEADER_AUX_BIT);

				//move packet pointer forward
				packetPtr++;
				packetSize++;
				*packetPtr = 0;
				
				SetBitsInWord(*packetPtr, 1, 
						PACKET_AUX_HEADER_ACK_FLAG_MASK, PACKET_AUX_HEADER_ACK_FLAG_POSITION);
				
				SetBitsInWord(*packetPtr, PacketSharedObjStruct->SEND_ACK_ID, 
						PACKET_AUX_HEADER_ACK_ID_MASK, PACKET_AUX_HEADER_ACK_ID_POSITION);

				SetBitsInWord(*packetPtr, inputSize, 
						PACKET_AUX_HEADER_DATA_SIZE_MASK, PACKET_AUX_HEADER_DATA_SIZE_POSITION);

			}
				


			//move packet pointer forward
			packetPtr++;
			packetSize++;
			


			//fills in data input is in bytes
			for (counter = 0; counter < (inputSize / BYTES_IN_WORD);  counter++)
			{
				packetSize++;
				*(packetPtr) = *inBuffer++ & 0x00FF;
				*(packetPtr++) |= (*inBuffer++ & 0x00FF) << 8;
			}

			//Calculate checksum
			*packetPtr++ = CalculateChecksum(packetHeadPtr, packetSize);

			packetSize++;
#ifdef INSERT_DUMMY_WORDS
                    *packetPtr = 0;
			*outSize = packetSize + 2;
#else
			*outSize = packetSize;
#endif
			PacketCreaterObjStruct->prevTransmitTimeMS = GetCurrentTimeMS();

			//
			// TODO: This delay should include the size of the packet
			// Sets random retransmit wait time to avoid infinite conflict loop
			//
			PacketCreaterObjStruct->randomRetransmitWaitTimeMS = 
					RandWord(PACKET_RETRANSMIT_MIN_WAIT_MS, PACKET_RETRANSMIT_MAX_WAIT_MS);
			
			if (inputSize > 0)
			{
				if (PacketSharedObjStruct->seekAckEnabled)
				{
					PacketCreaterObjStruct->currentState = PC_WAIT_FOR_ACK_STATE;					
				}
				else
				{
					PacketCreaterObjStruct->currentState = PC_NEW_PACKET_STATE;
				}
			}
				
		}
		break;
		case (PC_WAIT_FOR_ACK_STATE):
		{
			if (PacketSharedObjStruct->RCV_ACK_Flag)
			{
#ifdef PLATFORM_HW
				HwGpioClear(HW_GPIO_ID_A, GPIO_RETRANSMIT_LED);
#endif
				//Correct Ack Received
				if (PacketSharedObjStruct->RCV_ACK_ID == PacketCreaterObjStruct->currentPacketID)
				{
#ifdef DEBUG_PACKETER
					if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
					{
						DebugMsgW("\n\rGOOD! Correct Ack Received = %d - ", PacketSharedObjStruct->RCV_ACK_ID);
						DebugMsg(CurrentControllerPtr->nameString);
						DebugMsgDW(" @%lums", GetCurrentTimeMS());
					}
#endif
					PacketCreaterObjStruct->currentState = PC_NEW_PACKET_STATE;
				}
				else //Incorrect Ack Received
				{
#ifdef DEBUG_PACKETER					
					if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
					{
						DebugMsgW("\n\rBAD! Incorrect Ack Received = %d - ", PacketSharedObjStruct->RCV_ACK_ID);
						DebugMsg(CurrentControllerPtr->nameString);
						DebugMsgDW(" @%lums", GetCurrentTimeMS());
					}
#endif
						PacketCreaterObjStruct->currentState = PC_TRANSMIT_PACKET_STATE;
				}

				PacketSharedObjStruct->RCV_ACK_Flag = 0;
				
			}
			//TODO: Unhandled Case: New Packet Received, without Receiving Acknledgement
			// To Previous Packet
			else if (PacketSharedObjStruct->SEND_ACK_Flag)
			{
#ifdef DEBUG_PACKETER				
				if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
				{
					DebugMsgW("\n\rpacket retransmitted with ack = %d - ", PacketCreaterObjStruct->currentPacketID);
					DebugMsg(CurrentControllerPtr->nameString);
					DebugMsgDW(" @%lums", GetCurrentTimeMS());
				}
#endif
				PacketCreaterObjStruct->currentState = PC_TRANSMIT_PACKET_STATE;
			}
			else if ( (GetCurrentTimeMS() - PacketCreaterObjStruct->prevTransmitTimeMS)
				> PacketCreaterObjStruct->randomRetransmitWaitTimeMS)
			{
#ifdef DEBUG_PACKETER				
				if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
				{
					DebugMsgW("\n\rTimeout: packet retransmited = %d - ", PacketCreaterObjStruct->currentPacketID);
					DebugMsg(CurrentControllerPtr->nameString);
					DebugMsgDW(" @%lums", GetCurrentTimeMS());
				}
#endif
#ifdef PLATFORM_HW
				HwGpioSet(HW_GPIO_ID_A, GPIO_RETRANSMIT_LED);
#endif
				PacketCreaterObjStruct->currentState = PC_TRANSMIT_PACKET_STATE;

			}
			
			return RETURN_CONTROLLER_BLOCK_IDLE;
		}
	}





	return RETURN_OK;
}

//******************************************************************************
//
// FUNCTION:        Packet Creater Ready
//
// USAGE:             To check if packet creater is ready
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
WORD PacketCreaterReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	PACKET_SHARED_OBJ_STRUCT *PacketSharedObjStruct = (PACKET_SHARED_OBJ_STRUCT *)BlockObjStruct;

	if (!PacketSharedObjStruct->RCV_In_Progress_Flag)
	{
		if ( PacketSharedObjStruct->seekAckEnabled && 
			(PacketSharedObjStruct->SEND_ACK_Flag && *outputSize >= PACKET_CREATER_OUTPUT_SIZE) )
		{
			if (*inputSize > PACKET_CREATOR_INPUT_SIZE)
			{
				*inputSize = PACKET_CREATOR_INPUT_SIZE;
			}
			*outputSize = PACKET_CREATER_OUTPUT_SIZE;

			
			return TRUE;
		}
		if ( (*inputSize >= PACKET_CREATOR_INPUT_SIZE)
			&& *outputSize >= PACKET_CREATER_OUTPUT_SIZE)
		{
			*inputSize = PACKET_CREATOR_INPUT_SIZE;
			*outputSize = PACKET_CREATER_OUTPUT_SIZE;
			return TRUE;
		}
	}
	return FALSE; 
		
}


//******************************************************************************
//
// FUNCTION:        Packet Creater Ready
//
// USAGE:             To check if packet creater is ready
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
static WORD CalculateChecksum(WORD * Bp_Buffer, WORD B_Size)
{
	WORD	B_Checksum = 0;

	for ( ; B_Size > 0; B_Size--)
	{
		B_Checksum += (WORD)*Bp_Buffer;			// Summation for checksum value
		Bp_Buffer++;                            // next WORD
	}
	return B_Checksum;
}

/***********************************  END  ************************************/
