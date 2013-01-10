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


#ifndef __PACKETRETRIEVER_H__
	#define __PACKETRETRIEVER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
typedef struct
{
	DWORD packetsReceived;
	WORD currentState;
	WORD packetBitPosition;
	WORD rcvPacketID;
	WORD prevPacketID;	
	WORD packetSizeB;
	WORD packetGetDataStateByteCounter;
	WORD checkSum;
	WORD receivedAckFlag;
	WORD receivedAckID;
#ifdef DEBUG_PACKETER
	DWORD numOfSyncDetected;
	DWORD numOfPacketsReceived;
#endif
} PACKET_PARSER_OBJ_STRUCT;


typedef struct
{
	PACKET_CREATER_OBJ_STRUCT	CreaterStruct;
	PACKET_PARSER_OBJ_STRUCT ParserStruct;
	WORD seekAckEnabled;
	WORD SEND_ACK_Flag;
	WORD SEND_ACK_ID;
	WORD RCV_ACK_Flag;
	WORD RCV_ACK_ID;
	WORD RCV_In_Progress_Flag;
} PACKET_SHARED_OBJ_STRUCT;

typedef struct
{
	WORD seekAckEnabled;
} PACKET_SHARED_INIT_STRUCT;


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
void PacketParserInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct);
WORD PacketParserReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct);
WORD PacketParser(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct);
WORD GetShiftedDataFromBuffer(WORD *inputBuffer, WORD inputSize, 
							WORD inputBitPosition,  WORD *outputData);


#endif
