#ifndef __MODULATION_PARAMETERS_H__
#define __MODULATION_PARAMETERS_H__

#define FSK_SYMBOL_LENGTH           40
#define FSK_NUM_TONES               4
#define FSK_BITS_PER_SYMBOL         2
#define FSK_BITS_PER_SYMBOL_MASK    0x03
#define FSK_RECEIVER_FILTER_LENGTH  79
#define FSK_FIXED_POINT             15
static const SWORD FSK_TONE_SET[FSK_NUM_TONES][FSK_SYMBOL_LENGTH] = {
{
           0,    14876,    26510,    32365,    31164,    23170,    10126,    -5126, 
      -19261,   -29197,   -32768,   -29197,   -19261,    -5126,    10126,    23170, 
       31164,    32365,    26510,    14876,        0,   -14876,   -26510,   -32365, 
      -31164,   -23170,   -10126,     5126,    19261,    29197,    32767,    29197, 
       19261,     5126,   -10126,   -23170,   -31164,   -32365,   -26510,   -14876, 
},
{
           0,    19261,    31164,    31164,    19261,        0,   -19261,   -31164, 
      -31164,   -19261,        0,    19261,    31164,    31164,    19261,        0, 
      -19261,   -31164,   -31164,   -19261,        0,    19261,    31164,    31164, 
       19261,        0,   -19261,   -31164,   -31164,   -19261,        0,    19261, 
       31164,    31164,    19261,        0,   -19261,   -31164,   -31164,   -19261, 
},
{
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
},
{
           0,    26510,    31164,    10126,   -19261,   -32768,   -19261,    10126, 
       31164,    26510,        0,   -26510,   -31164,   -10126,    19261,    32767, 
       19261,   -10126,   -31164,   -26510,        0,    26510,    31164,    10126, 
      -19261,   -32768,   -19261,    10126,    31164,    26510,        0,   -26510, 
      -31164,   -10126,    19261,    32767,    19261,   -10126,   -31164,   -26510, 
},
};
static const WORD FSK_DIBITS_TO_SYMBOL[] = {
           0,        1,        3,        2, 
};
static const SWORD FSK_VCO_I_SET[FSK_NUM_TONES][FSK_SYMBOL_LENGTH] = {
{
           0,    14876,    26510,    32365,    31164,    23170,    10126,    -5126, 
      -19261,   -29197,   -32768,   -29197,   -19261,    -5126,    10126,    23170, 
       31164,    32365,    26510,    14876,        0,   -14876,   -26510,   -32365, 
      -31164,   -23170,   -10126,     5126,    19261,    29197,    32767,    29197, 
       19261,     5126,   -10126,   -23170,   -31164,   -32365,   -26510,   -14876, 
},
{
           0,    19261,    31164,    31164,    19261,        0,   -19261,   -31164, 
      -31164,   -19261,        0,    19261,    31164,    31164,    19261,        0, 
      -19261,   -31164,   -31164,   -19261,        0,    19261,    31164,    31164, 
       19261,        0,   -19261,   -31164,   -31164,   -19261,        0,    19261, 
       31164,    31164,    19261,        0,   -19261,   -31164,   -31164,   -19261, 
},
{
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
           0,    23170,    32767,    23170,        0,   -23170,   -32768,   -23170, 
},
{
           0,    26510,    31164,    10126,   -19261,   -32768,   -19261,    10126, 
       31164,    26510,        0,   -26510,   -31164,   -10126,    19261,    32767, 
       19261,   -10126,   -31164,   -26510,        0,    26510,    31164,    10126, 
      -19261,   -32768,   -19261,    10126,    31164,    26510,        0,   -26510, 
      -31164,   -10126,    19261,    32767,    19261,   -10126,   -31164,   -26510, 
},
};
static const SWORD FSK_VCO_Q_SET[FSK_NUM_TONES][FSK_SYMBOL_LENGTH] = {
{
       32767,    29197,    19261,     5126,   -10126,   -23170,   -31164,   -32365, 
      -26510,   -14876,        0,    14876,    26510,    32365,    31164,    23170, 
       10126,    -5126,   -19261,   -29197,   -32768,   -29197,   -19261,    -5126, 
       10126,    23170,    31164,    32365,    26510,    14876,        0,   -14876, 
      -26510,   -32365,   -31164,   -23170,   -10126,     5126,    19261,    29197, 
},
{
       32767,    26510,    10126,   -10126,   -26510,   -32768,   -26510,   -10126, 
       10126,    26510,    32767,    26510,    10126,   -10126,   -26510,   -32768, 
      -26510,   -10126,    10126,    26510,    32767,    26510,    10126,   -10126, 
      -26510,   -32768,   -26510,   -10126,    10126,    26510,    32767,    26510, 
       10126,   -10126,   -26510,   -32768,   -26510,   -10126,    10126,    26510, 
},
{
       32767,    23170,        0,   -23170,   -32768,   -23170,        0,    23170, 
       32767,    23170,        0,   -23170,   -32768,   -23170,        0,    23170, 
       32767,    23170,        0,   -23170,   -32768,   -23170,        0,    23170, 
       32767,    23170,        0,   -23170,   -32768,   -23170,        0,    23170, 
       32767,    23170,        0,   -23170,   -32768,   -23170,        0,    23170, 
},
{
       32767,    19261,   -10126,   -31164,   -26510,        0,    26510,    31164, 
       10126,   -19261,   -32768,   -19261,    10126,    31164,    26510,        0, 
      -26510,   -31164,   -10126,    19261,    32767,    19261,   -10126,   -31164, 
      -26510,        0,    26510,    31164,    10126,   -19261,   -32768,   -19261, 
       10126,    31164,    26510,        0,   -26510,   -31164,   -10126,    19261, 
},
};

#if defined(PLATFORM_HW)
    #pragma DATA_SECTION(FSK_RECEIVER_FILTER, RAM_CODE_SECTION)
#endif
static const SWORD FSK_RECEIVER_FILTER[FSK_RECEIVER_FILTER_LENGTH] = {
           3,       12,       26,       42,       56,       66,       67,       59, 
          42,       18,       -9,      -32,      -46,      -42,      -18,       32, 
         106,      202,      315,      437,      559,      672,      768,      842, 
         892,      916,      920,      906,      883,      856,      832,      815, 
         807,      808,      818,      832,      848,      862,      871,      874, 
         871,      862,      848,      832,      818,      808,      807,      815, 
         832,      856,      883,      906,      920,      916,      892,      842, 
         768,      672,      559,      437,      315,      202,      106,       32, 
         -18,      -42,      -46,      -32,       -9,       18,       42,       59, 
          67,       66,       56,       42,       26,       12,        3, 
};
#endif
