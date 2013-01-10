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


#define __PACKETRETRIEVER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"
#include <string.h>

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
typedef enum
{
	PACKET_PARSER_SEARCH_FOR_SYNC,
	PACKET_PARSER_GET_AUX_HEADER,
	PACKET_PARSER_GET_DATA,
	PACKET_PARSER_CHECKSUM_AND_VERIFY,
	PACKET_PARSER_SEARCH_FOR_EOT,
	NUM_PACKET_PARSER_STATES
} PACKET_PARSER_STATE_ENUM;


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

WORD GetShiftedDataFromBuffer(WORD *inputBuffer, WORD inputSize, 
							WORD inputBitPosition,  WORD *outputData);


//******************************************************************************
//
// FUNCTION:        Packet Parser Block Initialization
//
// USAGE:             Initializes Packet Parser Block Object Variables
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
void PacketParserInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	PACKET_SHARED_OBJ_STRUCT *PacketSharedObjStruct = (PACKET_SHARED_OBJ_STRUCT *)BlockObjStruct;

	PacketSharedObjStruct->ParserStruct.packetsReceived = 0;
	PacketSharedObjStruct->ParserStruct.currentState = PACKET_PARSER_SEARCH_FOR_SYNC;
	PacketSharedObjStruct->ParserStruct.packetBitPosition = 0;
	PacketSharedObjStruct->ParserStruct.prevPacketID = 0xFFFF;
	PacketSharedObjStruct->RCV_In_Progress_Flag = 0;
	PacketSharedObjStruct->RCV_ACK_ID= 0;
	PacketSharedObjStruct->RCV_ACK_Flag = FALSE;

#ifdef DEBUG_PACKETER
	PacketSharedObjStruct->ParserStruct.numOfSyncDetected = 0;
	PacketSharedObjStruct->ParserStruct.numOfPacketsReceived = 0;	
#endif
}

//******************************************************************************
//
// FUNCTION:        Packet Parser Block
//
// USAGE:             Parses packets
//				
//
// INPUT:              N/A
//
// OUTPUT:           N/A
//
// GLOBALS:         N/A
//
//******************************************************************************
WORD PacketParser(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	PACKET_SHARED_OBJ_STRUCT *PacketSharedObjStruct = (PACKET_SHARED_OBJ_STRUCT *)BlockObjStruct;
	PACKET_PARSER_OBJ_STRUCT *PacketParserObjStruct = &(PacketSharedObjStruct->ParserStruct);
	WORD inputAvail = *inSize;
	WORD outputAvail = *outSize;
	WORD inputData;
	WORD inputBitPosition = PacketParserObjStruct->packetBitPosition;
	WORD notEnoughDataBOOL = 0;


	while ( ( GetShiftedDataFromBuffer(inBuffer, inputAvail, inputBitPosition, &inputData) ) && 
			( outputAvail > 0) && !notEnoughDataBOOL)
	{
#if 0
		if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
		{
			DebugMsgW("\r\nPacketParserCurrentState: %d ", PacketParserObjStruct->currentState);
		}
#endif
		switch(PacketParserObjStruct->currentState)
		{
			case (PACKET_PARSER_SEARCH_FOR_SYNC):
				//SYNC FOUND!
				if ( (inputData & PACKET_HEADER_SYNC_MASK) == (PACKET_HEADER_SYNC & PACKET_HEADER_SYNC_MASK))
				{

					PacketParserObjStruct->packetGetDataStateByteCounter = 0;
					PacketParserObjStruct->checkSum = inputData;
					PacketParserObjStruct->rcvPacketID = ReadBitsInWord(inputData, PACKET_HEADER_ID_MASK, PACKET_HEADER_ID_POSITION);

					PacketSharedObjStruct->RCV_In_Progress_Flag = 1;
					if (ReadBitsInWord(inputData, 
						PACKET_HEADER_AUX_BIT_MASK, PACKET_HEADER_AUX_BIT) )
					{
						PacketParserObjStruct->currentState = PACKET_PARSER_GET_AUX_HEADER;
					}
					else
					{
	        				PacketParserObjStruct->packetSizeB = DEFAULT_PACKET_DATA_SIZE_B;
                                          PacketParserObjStruct->receivedAckFlag = 0;
                                          PacketParserObjStruct->receivedAckID = 0;
						PacketParserObjStruct->currentState = PACKET_PARSER_GET_DATA;
					}
					inputAvail--;
					inBuffer++;

#ifdef DEBUG_PACKETER
					PacketParserObjStruct->numOfSyncDetected++;
					if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
					{
						DebugMsgW("\n\rSync Detected: %d - ", PacketParserObjStruct->rcvPacketID);
						DebugMsg(CurrentControllerPtr->nameString);
						DebugMsgDW(" @%lums", GetCurrentTimeMS());	
					}
#endif
				}
				else
				{
					inputBitPosition++;
					//
					// sets bit position for next WORD
					//
					if (inputBitPosition >= BITS_IN_WORD)
					{
						inputBitPosition = inputBitPosition - BITS_IN_WORD;
						inputAvail--;
						inBuffer++;
					}
				}
				break;
			case (PACKET_PARSER_GET_AUX_HEADER):
			{
				PacketParserObjStruct->checkSum += inputData;
				PacketParserObjStruct->receivedAckFlag= ReadBitsInWord(inputData, 
						PACKET_AUX_HEADER_ACK_FLAG_MASK, PACKET_AUX_HEADER_ACK_FLAG_POSITION);
				if (PacketParserObjStruct->receivedAckFlag)
				{
					PacketParserObjStruct->receivedAckID= ReadBitsInWord(inputData, 
							PACKET_AUX_HEADER_ACK_ID_MASK, PACKET_AUX_HEADER_ACK_ID_POSITION);
				}
				PacketParserObjStruct->packetSizeB = ReadBitsInWord(inputData,
							PACKET_AUX_HEADER_DATA_SIZE_MASK, PACKET_AUX_HEADER_DATA_SIZE_POSITION);

#ifdef DEBUG_PACKETER
				if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
				{
					DebugMsgW("\n\rPacketSize Received: %d - ", PacketParserObjStruct->packetSizeB);
					DebugMsg(CurrentControllerPtr->nameString);
				}
#endif
				inputAvail--;
				inBuffer++;
				PacketParserObjStruct->currentState = PACKET_PARSER_GET_DATA;
			}
			break;
			case (PACKET_PARSER_GET_DATA):
				//
				// TODO: rethink this code design, confusing, Shu
				//
				// Extra 2 words are used to accomodate checksum and bit shifted word
				// To make sure that checksum states runs in the same pipeline cycle, so outbuffer data is not lost, SHU
				//
				if ( (inputAvail >= ((PacketParserObjStruct->packetSizeB / BYTES_IN_WORD) + 2)) && 
					(outputAvail >= (PacketParserObjStruct->packetSizeB)) )
				{
					//
					// Retrives byte-packed words from input buffer
					//
					if (PacketParserObjStruct->packetGetDataStateByteCounter++ < PacketParserObjStruct->packetSizeB)
					{
						PacketParserObjStruct->checkSum += inputData;

						*outBuffer++ = inputData & 0x00FF;
#ifdef DEBUG_PACKETER
						if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
						{
	                                           DebugMsgDW("\n\rPacketParser Received: %lx", inputData);
						}
#endif
						//
						// Gets the last odd byte out of the input buffer
						//
						if (PacketParserObjStruct->packetGetDataStateByteCounter++ < PacketParserObjStruct->packetSizeB)
						{
							*outBuffer++ = (inputData & 0xFF00) >> 8;
							inBuffer++;
						}


					}
					//
					// Finished gathering all data from packet
					//
					else
					{
						inputAvail -= (PacketParserObjStruct->packetSizeB + 1) / BYTES_IN_WORD;
						PacketParserObjStruct->currentState = PACKET_PARSER_CHECKSUM_AND_VERIFY;
					}
				}
				else
				{
					notEnoughDataBOOL = TRUE;
				}
				break;

			case (PACKET_PARSER_CHECKSUM_AND_VERIFY):

				//
				// Finished receiving current packet
				//
				PacketSharedObjStruct->RCV_In_Progress_Flag = 0;
#ifdef DEBUG_PACKETER
				if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
				{
	                            DebugMsgW("\n\rPacketParser Checksum: %x", inputData);
	                            DebugMsgW("\n\rPacketParser Checksum Expected: %x", PacketParserObjStruct->checkSum);
				}
#endif
				
				//
				// Checksum Matches
				// New Packet Received!
				//
				if (inputData == PacketParserObjStruct->checkSum)
				{					
					//an acknowledgement is sent only if data is received 
					// (ie. rcved packet that only contained acknowledgent do not need ack.)
					if (PacketParserObjStruct->packetSizeB > 0)
					{
						WORD nextExpectedID = PacketParserObjStruct->prevPacketID + 1;
						if ((unsigned)nextExpectedID > PACKET_HEADER_ID_MASK)
							nextExpectedID = 0;

						//
						// Correct packet ID is received
						//
						if (nextExpectedID == PacketParserObjStruct->rcvPacketID)
						{
#ifdef DEBUG_PACKETER	
							if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)\
							{
								DebugMsgW("\n\rGOOD! Correct Packet ID Received: %d - ", PacketParserObjStruct->rcvPacketID);
								DebugMsg(CurrentControllerPtr->nameString);
								DebugMsgDW(" @%lums", GetCurrentTimeMS());
							}
#endif
							outputAvail -= PacketParserObjStruct->packetSizeB;
						}
						//
						// Same packet has been received again
						// Packet is discarded, and an repeat acknoledgement is sent
						//
						else if (PacketParserObjStruct->prevPacketID == PacketParserObjStruct->rcvPacketID)
						{
#ifdef DEBUG_PACKETER							
							if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)\
							{
								DebugMsgW("\n\rRepeat Packet ID Received: %d - ", PacketParserObjStruct->rcvPacketID);
								DebugMsg(CurrentControllerPtr->nameString);
								DebugMsgDW(" @%lums", GetCurrentTimeMS());								
							}
#endif
						}
						else
						{
							outputAvail -= PacketParserObjStruct->packetSizeB;

#ifdef DEBUG_PACKETER								
							if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)\
							{
								DebugMsgW("\n\rBAD! Incorrect Packet ID Received: %d - ", PacketParserObjStruct->rcvPacketID);
								DebugMsg(CurrentControllerPtr->nameString);
								DebugMsgDW(" @%lums", GetCurrentTimeMS());								
							}
#endif
						}

							
						PacketSharedObjStruct->SEND_ACK_Flag = 1;
						PacketSharedObjStruct->SEND_ACK_ID = PacketParserObjStruct->rcvPacketID;
					}
					
					PacketSharedObjStruct->RCV_ACK_Flag = PacketParserObjStruct->receivedAckFlag;
					PacketSharedObjStruct->RCV_ACK_ID = PacketParserObjStruct->receivedAckID;

					PacketParserObjStruct->prevPacketID = PacketParserObjStruct->rcvPacketID;
					

				}
				else
				{
#ifdef DEBUG_PACKETER
					PacketParserObjStruct->numOfPacketsReceived++;
					if (CurrentControllerPtr->optionFlags & CONTROLLER_OPTIONS_PRINT_DEBUG)
					{
						DebugMsg("\n\rBAD! Incorrect Checksum Received - ");
						DebugMsg(CurrentControllerPtr->nameString);
						DebugMsgDW(" @%lums", GetCurrentTimeMS());
					}
#endif								
					PacketParserObjStruct->rcvPacketID = PacketParserObjStruct->prevPacketID;
				}
				inputAvail--;

				PacketParserObjStruct->currentState = PACKET_PARSER_SEARCH_FOR_SYNC;
				break;
				
			/* TO BE ADDED
			case (PACKET_PARSER_SEARCH_FOR_EOT):
				break;
			*/
			default:
				break;

		}
	}

	*inSize = *inSize -inputAvail;
	*outSize = *outSize - outputAvail;
	PacketParserObjStruct->packetBitPosition = inputBitPosition;
	return 0;
}

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
WORD PacketParserReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if ( (*inputSize > 0)
		&& *outputSize > 0)
	{
		return TRUE;
	}
	return FALSE; 
		
}

//******************************************************************************
//
// FUNCTION:        Get Input Bits from Buffer
//
// USAGE:             Gets a specified number of input bits from a buffer
//
// INPUT:              inputBuffer - location of input buffer
//				inputBitPosition - bit position of the input buffer
//				sizeOfInput - number of WORDs in input buffer
//				outputData - location of where to put output data (one WORD)
//
// OUTPUT:           TRUE/ FALSE - whether there is enough space on the input to get the data
//
// GLOBALS:         N/A
//
//******************************************************************************
WORD GetShiftedDataFromBuffer(WORD *inputBuffer, WORD inputSize, 
							WORD inputBitPosition,  WORD *outputData)
{
	if (inputSize > 0)
	{
		*outputData = *inputBuffer++ >> inputBitPosition;

		if (inputBitPosition != 0 )
		{
			if (inputSize >= 2)
			{
				*outputData |= *inputBuffer << (BITS_IN_WORD - inputBitPosition);
			}
			else
			{
				//not enough data left to retrieve anything
				return FALSE;
			}
		}
		return TRUE;

	}
	return FALSE;
}


/***********************************  END  ************************************/
