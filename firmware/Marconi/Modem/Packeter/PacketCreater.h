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


#ifndef __PACKETCREATER_H__
	#define __PACKETCREATER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************
//
// Retransmit times takes into account propogation delay (software/ hardware)
// + Radio PPT delay for both sender and receiver
//
#define AVERAGE_PACKET_TRANSMISSION_TIME_MS         (640 )
#define ONE_WAY_PACKET_PROP_DELAY                         ( TXGATE_TRANSITION_TO_TRANSMIT_WAIT_MS \
                                                                                         + AVERAGE_PACKET_TRANSMISSION_TIME_MS )
#define TWO_WAY_PACKET_PROP_DELAY                       ( ONE_WAY_PACKET_PROP_DELAY * 2 )

#define PACKET_RETRANSMIT_MIN_WAIT_MS		(1000 + TWO_WAY_PACKET_PROP_DELAY)
#define PACKET_RETRANSMIT_MAX_WAIT_MS		(5000 + PACKET_RETRANSMIT_MIN_WAIT_MS)

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef enum
{
	PC_TRANSMIT_PACKET_STATE,
	PC_WAIT_FOR_ACK_STATE,
	PC_IDLE_PACKET_STATE,
	PC_NEW_PACKET_STATE,
	NUM_OF_PACKET_CREATER_STATES
} PACKET_CREATER_STATES;




typedef struct
{
	WORD currentState;
	WORD currentPacketID;
	DWORD packetsTransmitted;
	DWORD prevTransmitTimeMS;
	WORD randomRetransmitWaitTimeMS;
#ifdef DEBUG_PACKETER
	DWORD numOfPacketsCreated;
#endif	
} PACKET_CREATER_OBJ_STRUCT;
	

//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
//#define PACKET_CREATOR_INPUT_SIZE					DEFAULT_PACKET_DATA_SIZE_B

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void PacketCreaterInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD PacketCreaterReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);
WORD PacketCreater(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);


#endif
