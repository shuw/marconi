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


#ifndef __PACKETFORMAT_H__
	#define __PACKETFORMAT_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************
#define DEFAULT_PACKET_DATA_SIZE_B						8
#define MAX_PACKET_HEADERS_SIZE_W						2

#define DEFAULT_SIZE_OF_PACKET_IN_WORDS		(DEFAULT_PACKET_DATA_SIZE_B / BYTES_IN_WORD) \
													+ MAX_PACKET_HEADERS_SIZE_W
//#define INSERT_DUMMY_WORDS



//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


#ifdef INSERT_DUMMY_WORDS
	#define PACKET_CREATER_OUTPUT_SIZE				(DEFAULT_SIZE_OF_PACKET_IN_WORDS + 2)
#else
	#define PACKET_CREATER_OUTPUT_SIZE				DEFAULT_SIZE_OF_PACKET_IN_WORDS
#endif


#define PACKET_CREATOR_INPUT_SIZE					DEFAULT_PACKET_DATA_SIZE_B


//******************************************************************************
//				PACKET STRUCTURE
//     0			      11       15//  0bXXXX XXXX XXX XXXX  X (xxxx)
//    Sync		      ID	  Acknowledgement bit  (ACK information only exists when ACK bit is 1)
//******************************************************************************
#define PACKET_HEADER_SYNC				0xACA

#define PACKET_HEADER_SYNC_POSITION		0
#define PACKET_HEADER_SYNC_MASK			0x7FF

#define PACKET_HEADER_ID_POSITION			11
#define PACKET_HEADER_ID_MASK				0xF

#define PACKET_HEADER_AUX_BIT				15
#define PACKET_HEADER_AUX_BIT_MASK		0x1


//******************************************************************************
//				PACKET STRUCTURE
//     0			1			 5
//    Ack_FLAG	Ack_ID		Data Size
//******************************************************************************
#define PACKET_AUX_HEADER_ACK_FLAG_MASK				0x1
#define PACKET_AUX_HEADER_ACK_FLAG_POSITION			0

#define PACKET_AUX_HEADER_ACK_ID_MASK				0xF
#define PACKET_AUX_HEADER_ACK_ID_POSITION			1

//
//
// WARNING: Make sure maximum packet size dosen't exceed input buffer size of packeter
//
#define PACKET_AUX_HEADER_DATA_SIZE_MASK			0x0F
#define PACKET_AUX_HEADER_DATA_SIZE_POSITION		5


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
