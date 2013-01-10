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


#ifndef __CONFIG_H__
	#define __CONFIG_H__


//******************************************************************************
//  P R O G R A M    P A R A M E T E R S
//******************************************************************************
//#define USE_TESTER_MAIN	
#define VERSION_STRING			"\n\rPrototype 0.7 Beta - Camillo Testboard" \
								"\n\rUniversity of Waterloo - FYDP 2005" \
								"\n\rA. Cheung G. Liu" \
								"\n\rS. Janhunen S. Wu"

//******************************************************************************
//  D E B U G   P A R A M E T E R S
//******************************************************************************
#define DEBUG_PACKETER
#define DEBUG_TXGATE
//#define RADIOSIM_ENABLE_RATE_CONTROL

//******************************************************************************
//  M O D E M    P A R A M E T E R S
//******************************************************************************
//#define PIPELINE_PRINT_STATS

//#define ERROR_CHECK_DISABLE_COMPARISON
//#define VDECODER_SCAN_FOR_BIT_SHIFTS
#define USE_SOFT_DECISION

#define VITERBI_K 										6
#define VITERBI_N										2
#define VITERBI_NUM_SOFT_BITS							4
#define DEFAULT_SNR_DB									0


//******************************************************************************
//  D E T A I L E D   C O N F I G U R A T I O N
//******************************************************************************


#if (VITERBI_NUM_SOFT_BITS == 4)
	#define SOFT_LEVEL_BINARY_BIT_MAX_LEVEL	15
	#define SOFT_LEVEL_BINARY_BIT_MASK	0xF
	#define SOFT_LEVEL_2BINARY_BIT_MASK	0xFF
#endif


#endif
