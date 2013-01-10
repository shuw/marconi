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


#define __AWGN_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"

/* BPSK BINARY SYMMETRIC CHANNEL SIMULATOR                          */
/* Copyright (c) 1999, Spectrum Applications, Derwood, MD, USA      */
/* All rights reserved                                              */
/* Version 2.0 Last Modified 1999.02.17                             */
 

#include <math.h>
#include <stdlib.h>
 



//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************
#define K 3              /* constraint length */
#define TWOTOTHEM 4      /* 2^(K - 1) -- change as required */
#define PI 3.141592654   /* circumference of circle divided by diameter */

#define MSG_LEN 100000l  /* how many bits in each test message */
#define DOENC            /* test with convolutional encoding/Viterbi decoding */
#undef  DONOENC          /* test with no coding */
#define LOESN0 0.0       /* minimum Es/No at which to test */
#define HIESN0 3.5       /* maximum Es/No at which to test */
#define ESN0STEP 0.5     /* Es/No increment for test driver */



//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************
static float GNGauss(float mean, float sigma);
static void AWGNAddNoise(float es_ovr_n0, long channel_len, int *in_array, float *out_array);

//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************



//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************
typedef enum
{
	AWGN_ADD_BIT_SHIFT_STATE,
	AWGN_NORMAL_STATE,
	NUM_OF_AWGN_CHANNEL_STATES
} AWGN_CHANNEL_STATE_MACHINE;

//******************************************************************************
//  C O D E
//******************************************************************************



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
void AWGNChannelInit(BYTE *BlockInitStruct, BYTE *BlockObjStruct)
{
	AWGN_CHANNEL_INIT_STRUCT *AWGNChannelInitStruct = (AWGN_CHANNEL_INIT_STRUCT *)BlockInitStruct;
	AWGN_CHANNEL_OBJ_STRUCT *AWGNChannelObjStruct = (AWGN_CHANNEL_OBJ_STRUCT *)BlockObjStruct;

	AWGNChannelObjStruct->SignalToNoiseDB = 	AWGNChannelInitStruct->SignalToNoiseDB;
	AWGNChannelObjStruct->currentState = AWGN_NORMAL_STATE;
	AWGNChannelObjStruct->outputBitPosition = 0;
	AWGNChannelObjStruct->outputWord = 0;
	AWGNChannelObjStruct->numOfDroppedBits = 0;
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
WORD AWGNChannel(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *BlockObjStruct)
{
	WORD inputSize = *inSize;
	WORD inputBitPosition = 0;
	SWORD inputSoftBit;
	WORD inputByte;
	WORD outputByte;
	int inputInteger;
	float outputFloat;
	AWGN_CHANNEL_OBJ_STRUCT *AWGNChannelObjStruct = (AWGN_CHANNEL_OBJ_STRUCT *)BlockObjStruct;
	double SNRdB = AWGNChannelObjStruct->SignalToNoiseDB;
	WORD outputSize = 0;
	WORD outputWord = AWGNChannelObjStruct->outputWord;
	WORD outputBitPosition = AWGNChannelObjStruct->outputBitPosition;
	
	switch (AWGNChannelObjStruct->currentState)
	{
		case (AWGN_NORMAL_STATE):
			while (inputSize-- > 0)
			{
				inputByte = *inBuffer;

				outputByte = 0;
				inputBitPosition = 0;
				while (inputBitPosition <  BITS_IN_WORD)
				{
					//Once in a blue moon, bits are dropped based on average period
					if (rand() >= (INV_AVERAGE_PERIOD_OF_BIT_DROPS))
					{
						inputSoftBit = (inputByte >> inputBitPosition) & SOFT_LEVEL_BINARY_BIT_MASK;

						if (inputSoftBit >= 0x08)
						{
							inputInteger = 1;
						}
						else
						{		
							inputInteger = 0;
						}

						AWGNAddNoise((float)(SNRdB), 1,  &inputInteger, &outputFloat);
						outputFloat = (outputFloat -1) / (-2);

						inputSoftBit = (WORD)(outputFloat * 0x0F);
						

						if (inputSoftBit < 0)
						{
							inputSoftBit = 0;
						}
						else if (inputSoftBit > SOFT_LEVEL_BINARY_BIT_MAX_LEVEL)
						{
							inputSoftBit = SOFT_LEVEL_BINARY_BIT_MAX_LEVEL;
						}

						outputByte |= inputSoftBit << outputBitPosition;
						outputBitPosition += VITERBI_NUM_SOFT_BITS;
					}
					else
					{
						AWGNChannelObjStruct->numOfDroppedBits++;
						outputBitPosition = outputBitPosition;
					}

					if (outputBitPosition >= BITS_IN_WORD)
					{
						*outBuffer++ = outputByte;
						outputSize++;
						outputByte = 0;
						outputBitPosition = 0;
					}
					
					inputBitPosition += VITERBI_NUM_SOFT_BITS;
				}
				inBuffer++;
			}
			break;
		default:
			AWGNChannelObjStruct->currentState = AWGN_NORMAL_STATE;
			break;
	}


	AWGNChannelObjStruct->outputWord = outputWord;
	AWGNChannelObjStruct->outputBitPosition = outputBitPosition;
	*outSize = outputSize;
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
WORD AWGNChannelReady(WORD *inputSize, WORD *outputSize, BYTE *BlockObjStruct)
{
	if (*inputSize >= AWGN_READY_SIZE && *outputSize >= AWGN_READY_SIZE)
	{
		*inputSize = AWGN_READY_SIZE;
		*outputSize = AWGN_READY_SIZE;
		return TRUE;
	}
	return FALSE;
}



//******************************************************************************
//
// FUNCTION:        AWGN Noise Channel for modulated signal
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
static void AWGNAddNoise(float es_ovr_n0, long channel_len, int *in_array, float *out_array) {

	long t;
	float mean, es, sn_ratio, sigma, signal;
	float gauss_offset;

	// given the desired Es/No (for BPSK, = Eb/No - 3 dB), calculate the
	//standard deviation of the additive white gaussian noise (AWGN). The
	//standard deviation of the AWGN will be used to generate Gaussian random
	//variables simulating the noise that is added to the signal. 

	mean = 0;
	es = 1;
	sn_ratio = (float) pow(10, ( es_ovr_n0 / 10) );
	sigma =  (float) sqrt (es / ( 2 * sn_ratio ) );

	// now transform the data from 0/1 to +1/-1 and add noise 
	for (t = 0; t < channel_len; t++) {

		//if the binary data value is 1, the channel symbol is -1; if the
		//binary data value is 0, the channel symbol is +1.
		signal = (float)(1 - 2 * *( in_array + t ));

		//  now generate the gaussian noise point, add it to the channel symbol,
		//    and output the noisy channel symbol 

		gauss_offset = GNGauss(mean,sigma);
		*( out_array + t ) = signal + gauss_offset;
	}

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
static float GNGauss(float mean, float sigma) {

	// This uses the fact that a Rayleigh-distributed random variable R, with
	// the probability distribution F(R) = 0 if R < 0 and F(R) =
	// 1 - exp(-R^2/2*sigma^2) if R >= 0, is related to a pair of Gaussian
	// variables C and D through the transformation C = R * cos(theta) and
	// D = R * sin(theta), where theta is a uniformly distributed variable
	// in the interval (0, 2*pi()). From Contemporary Communication Systems
	// USING MATLAB(R), by John G. Proakis and Masoud Salehi, published by
	// PWS Publishing Company, 1998, pp 49-50. This is a pretty good book. 

	double u, r;            //  uniform and Rayleigh random variables 
	DWORD randomDouble;

	randomDouble = 0;
	randomDouble = ((WORD)rand() << 1) & 0xFFFF;
	randomDouble |= ((DWORD)rand() << 16);

	//  generate a uniformly distributed random number u between 0 and 1 - 1E-6
	u = (double)randomDouble / (double)LRAND_MAX;
	if (u == 1.0) u = 0.999999999;

	//  generate a Rayleigh-distributed random number r using u 
	r = sigma * sqrt( 2.0 * log( 1.0 / (1.0 - u) ) );

	randomDouble = 0;
	randomDouble = ((WORD)rand() << 1) & 0xFFFF;
	randomDouble |= ((DWORD)rand() << 16);
	
	//  generate another uniformly-distributed random number u as before
	u = (double)randomDouble / (double)LRAND_MAX;
	if (u == 1.0) u = 0.999999999;

	//  generate and return a Gaussian-distributed random number using r and u 
	return( (float) ( mean + r * cos(2 * PI * u) ) );
}

/***********************************  END  ************************************/
