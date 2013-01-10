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


#ifndef __VENCODER_H__
	#define __VENCODER_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define VITERBI_CONSTRAINT_LENGTH 			4
#define VITERBI_NUM_OF_CHILDREN_STATES		2
	
#if (VITERBI_CONSTRAINT_LENGTH == 2)
	#define FIRST_POLYNOMIAL					(BIT0 | BIT1 | BIT2)
	#define SECOND_POLYNOMIAL					(BIT0 | BIT2)
	//this number must be 2 ^ VITERBI_CONSTRAINT_LENGTH
	#define VITERBI_NUM_OF_STATES 				4
	#define VITERBI_STATES_BIT_MASK			(BIT0 | BIT1)

#endif

#if (VITERBI_CONSTRAINT_LENGTH == 3)
	#define FIRST_POLYNOMIAL					(BIT0 | BIT2 | BIT3)
	#define SECOND_POLYNOMIAL					(BIT0 | BIT1 | BIT2 | BIT3)
	//this number must be 2 ^ VITERBI_CONSTRAINT_LENGTH
	#define VITERBI_NUM_OF_STATES 				8
	#define VITERBI_STATES_BIT_MASK			(BIT0 | BIT1 | BIT2)

#endif

#if (VITERBI_CONSTRAINT_LENGTH == 4)
	#define FIRST_POLYNOMIAL					(BIT0 | BIT1 | BIT3 | BIT4)
	#define SECOND_POLYNOMIAL					(BIT0 | BIT2 | BIT3 | BIT4)
	//this number must be 2 ^ VITERBI_CONSTRAINT_LENGTH
	#define VITERBI_NUM_OF_STATES 				16
	#define VITERBI_STATES_BIT_MASK			(BIT0 | BIT1 | BIT2 | BIT3)

#endif


//
// For this to work, non-even (NUM_OF_BITS_IN_BBYTE % VITERBI_CONSTRAINT_LENGTH) != 0
// packing must be implemented by both encoder and decoder 
//
#if (VITERBI_CONSTRAINT_LENGTH == 6)
	#define FIRST_POLYNOMIAL					(BIT0 | BIT1 | BIT3 | BIT5 | BIT6)
	#define SECOND_POLYNOMIAL					(BIT0 | BIT1 | BIT2 | BIT4 | BIT5 | BIT6)
	//this number must be 2 ^ VITERBI_CONSTRAINT_LENGTH
	#define VITERBI_NUM_OF_STATES 				64
	#define VITERBI_STATES_BIT_MASK			(BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5)

#endif


//
// Must move to 16bit BBYTE for this to work, number of states 256 > than that allowed by 8bit char
//
#if (VITERBI_CONSTRAINT_LENGTH == 8)
	#define FIRST_POLYNOMIAL					(BIT0 | BIT1 | BIT2 | BIT3 | BIT5 | BIT6)
	#define SECOND_POLYNOMIAL					(BIT0 | BIT1 | BIT2 | BIT5 | BIT6 | BIT7)
	//this number must be 2 ^ VITERBI_CONSTRAINT_LENGTH
	#define VITERBI_NUM_OF_STATES 				256
	#define VITERBI_STATES_BIT_MASK			(0xFF)

#endif


//
// VENCODER packet size is proportional to constraint length
//
#define VENCODER_PACKET_SIZE					((((VITERBI_CONSTRAINT_LENGTH * 5)  * 2) / NUM_OF_BITS_IN_BBYTE) + 1)

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
BYTE VEncoder(BYTE *inBuffer, BYTE *inSize, BYTE *outBuffer, BYTE *outSize);
BBYTE ViterbiEncodingAlgorithm(BBYTE inputBits);
BYTE VEncoderInputSizeB(void);
BYTE VEncoderOutputSizeB(void);

#endif
