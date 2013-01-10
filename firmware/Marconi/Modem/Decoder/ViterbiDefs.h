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


#ifndef __VITERBIDEFS_H__
	#define __VITERBIDEFS_H__


//******************************************************************************
//  D E F I N E S
//******************************************************************************

//******************************************************************************
//  E N U M S   A N D   S T R U C T U R E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************
#define VITERBI_NUM_OF_CHILDREN_STATES		2
#define _DEBUG_VITERBI
#define VITERBI_ENCODER_START_STATE			0
#define VITERBI_ENCODER_END_STATE				0
#define VITERBI_PACKET_DEPTH_CONSTANT		5


//******************************************************************************
//  V I T E R B I   E N C O D E R   C O N S T A N T S 
//******************************************************************************
//
// VENCODER packet size is proportional to constraint length
//
#define VITERBI_NUM_OF_STATES 						(WORD)(0x1 << (VITERBI_K - 1))
#define VITERBI_STATE_BIT_MASK						(WORD)(0xFFFF >> (17 - VITERBI_K))
#define VITERBI_POLYNOMIAL_BIT_MASK				(WORD)(0xFFFF >> (16 - VITERBI_K))

#define VDECODER_NUM_OF_INPUT_BITS				(WORD)(((VITERBI_K * VITERBI_PACKET_DEPTH_CONSTANT) + 16) / 16) *16
//#define 		VDECODER_NUM_OF_INPUT_BITS			32





//******************************************************************************
//  V I T E R B I   D E C O D E R   C O N S T A N T S 
//******************************************************************************
#define VDECODER_INPUT_SIZE				(WORD)(((((VDECODER_NUM_OF_INPUT_BITS * VITERBI_N) - 1) / BITS_IN_WORD) + 1)* VITERBI_NUM_SOFT_BITS)
#define VDECODER_OUTPUT_SIZE				(WORD)(VDECODER_INPUT_SIZE / (VITERBI_N * VITERBI_NUM_SOFT_BITS))

#define VDECODER_COLUMNS_IN_TRELLIS 			VDECODER_NUM_OF_INPUT_BITS / NUM_OF_BITS_PER_TRELLIS_STEP
#define VDECODER_ROWS_IN_TRELLIS				VITERBI_NUM_OF_STATES / NUM_OF_NODES_PACKED_PER_WORD
#define NUM_OF_NODES_PACKED_PER_WORD		2
#define NUM_OF_BITS_PER_TRELLIS_STEP			2
#define NUM_OF_BRANCHES_FOR_EACH_NODE		0x1 << NUM_OF_BITS_PER_TRELLIS_STEP

//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
