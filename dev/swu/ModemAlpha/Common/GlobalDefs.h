#ifndef __GLOBALDEFS_H__
	#define __GLOBALDEFS_H__


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

//******************************************************************************
//  D E F I N E S
//******************************************************************************
//
// Common data type cast
//

#define NUM_OF_BITS_IN_BBYTE 			8
#define MOST_SIG_DIGIT_IN_BBYTE 		BIT7

typedef  char   BYTE;
typedef  char   BBYTE;
typedef  char   LETTER;
	
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef signed   char   SBYTE;
typedef signed   short  SWORD;
typedef signed   long   SDWORD;


//
// Common Boolean logic
//
typedef enum bool_enum
{
	FALSE,
	TRUE
} BOOL;

#define NULL_PTR	0

#define ROM const far


//
// General bit definition
//
#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
#define BIT12       0x1000
#define BIT13       0x2000
#define BIT14       0x4000
#define BIT15       0x8000


typedef struct
{
	BYTE 	*StringPtr;
	WORD	StringSize;
} BYTE_STRING_STRUCT;



#define ASCII_NULL				0x00
#define ASCII_SOH				0x01
#define ASCII_STX				0x02
#define ASCII_BS					0x08 	//BackSpace
#define ASCII_CR					0x0D	//Carriage Return
#define ASCII_ESC				0x1B
#define ASCII_SPACE				0x20
#define ASCII_TAB				0x09
#define ASCII_LF					0x0A

//******************************************************************************
//  E N U M S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************



//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************




#endif
