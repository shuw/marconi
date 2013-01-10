/////////////////////////////////////////////
// Test vectors for test: dspMultTest
/////////////////////////////////////////////

#define DSPMULTTEST_COUNT 1

const WORD dspMultTestXLengths[] = {
          21, 
};

const SWORD dspMultTestXVectors[] = {
         -10,       -9,       -8,       -7,       -6,       -5,       -4,       -3, 
          -2,       -1,        0,        1,        2,        3,        4,        5, 
           6,        7,        8,        9,       10, 
};

const WORD dspMultTestYLengths[] = {
          21, 
};

const SWORD dspMultTestYVectors[] = {
         -10,       -9,       -8,       -7,       -6,       -5,       -4,       -3, 
          -2,       -1,        0,        1,        2,        3,        4,        5, 
           6,        7,        8,        9,       10, 
};

const WORD dspMultTestZLengths[] = {
          21, 
};

const SWORD dspMultTestZVectors[] = {
         100,       81,       64,       49,       36,       25,       16,        9, 
           4,        1,        0,        1,        4,        9,       16,       25, 
          36,       49,       64,       81,      100, 
};

const WORD dspMultTestShiftLengths[] = {
          21, 
};

const WORD dspMultTestShiftVectors[] = {
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0, 
};

/////////////////////////////////////////////
// Test vectors for test: dspConvTest
/////////////////////////////////////////////

#define DSPCONVTEST_COUNT 1

const WORD dspConvTestXLengths[] = {
         120, 
};

const SWORD dspConvTestXVectors[] = {
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
       26214,    25892,    24931,    23357,    21208,    18536,    15408,    11901, 
        8101,     4101,        0,    -4101,    -8101,   -11901,   -15408,   -18536, 
      -21208,   -23357,   -24931,   -25892,   -26214,   -25892,   -24931,   -23357, 
      -21208,   -18536,   -15408,   -11901,    -8101,    -4101,        0,     4101, 
        8101,    11901,    15408,    18536,    21208,    23357,    24931,    25892, 
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
           0,        0,        0,        0,        0,        0,        0,        0, 
};

const WORD dspConvTestHLengths[] = {
          40, 
};

const SWORD dspConvTestHVectors[] = {
         -84,     -256,     -421,     -563,     -665,     -710,     -688,     -590, 
        -412,     -155,      172,      557,      983,     1430,     1873,     2289, 
        2655,     2950,     3157,     3263,     3263,     3157,     2950,     2655, 
        2289,     1873,     1430,      983,      557,      172,     -155,     -412, 
        -590,     -688,     -710,     -665,     -563,     -421,     -256,      -84, 
};

const WORD dspConvTestZLengths[] = {
          81, 
};

const SWORD dspConvTestZVectors[] = {
           0,      -68,     -272,     -604,    -1039,    -1536,    -2038,    -2479, 
       -2788,    -2890,    -2720,    -2225,    -1363,     -122,     1486,     3430, 
        5639,     8024,    10473,    12859,    15045,    16892,    18271,    19061, 
       19176,    18556,    17166,    15016,    12155,     8660,     4651,      269, 
       -4330,    -8969,   -13469,   -17666,   -21398,   -24527,   -26946,   -28576, 
      -29365,   -29297,   -28371,   -26609,   -24076,   -20866,   -17098,   -12918, 
       -8497,    -4000,      393,     4514,     8215,    11369,    13873,    15668, 
       16725,    17053,    16702,    15746,    14282,    12435,    10334,     8114, 
        5901,     3808,     1932,      343,     -908,    -1808,    -2362,    -2596, 
       -2560,    -2316,    -1928,    -1470,    -1004,     -588,     -267,      -67, 
           0, 
};

const WORD dspConvTestShiftLengths[] = {
           1, 
};

const WORD dspConvTestShiftVectors[] = {
          15, 
};

/////////////////////////////////////////////
// Test vectors for test: dspMagSqTest
/////////////////////////////////////////////

#define DSPMAGSQTEST_COUNT 1

const WORD dspMagSqTestXiLengths[] = {
         128, 
};

const SWORD dspMagSqTestXiVectors[] = {
         928,     -704,     -178,      -25,       44,     -505,      686,      884, 
         419,      685,      404,     -612,     -737,      669,     -543,     -857, 
          20,     -632,      619,      808,      966,     -167,     -656,     -511, 
         772,     -275,      796,     -989,     -158,      742,      218,     -604, 
         243,      219,     -180,      614,     -948,      548,      578,     -698, 
        -465,     -804,      -20,       96,     -761,      461,     -812,      742, 
         243,      156,      -98,     -544,      921,      303,     -173,     -927, 
         309,     -364,      977,     -648,     -408,    -1004,       36,     -182, 
        -881,      736,      729,     -449,     -571,      649,     -584,      971, 
         190,     -832,      347,     -859,     -388,     -927,     -734,     -659, 
          46,      232,      526,     -203,     -698,     -329,     -315,     -506, 
         830,      829,     -839,     -568,      717,      791,      570,     -438, 
         323,      545,     -316,      489,      103,     -309,      651,       68, 
         -67,     -238,      186,      182,     -366,     -882,      933,     -867, 
         174,     1018,      335,      566,     -162,     -571,      424,      567, 
         -99,     -745,       89,     -435,      349,      173,      634,     -678, 
};

const WORD dspMagSqTestXqLengths[] = {
         128, 
};

const SWORD dspMagSqTestXqVectors[] = {
        -755,     -241,      786,     -222,      130,      138,     -273,     -197, 
         336,      592,      121,      474,     -140,      768,     -488,     -714, 
        -988,     -247,     -112,     -197,     -793,     -902,     -651,      725, 
        -471,     -969,     -575,      566,     -401,     -439,      718,      347, 
         617,      954,      257,     -470,      272,     -993,        2,       87, 
        -152,     -903,      515,     -391,      109,      287,      -17,      634, 
         171,     -882,       19,     -440,    -1008,     -274,      990,      -32, 
         194,      515,      296,     1007,      579,      210,     -967,     -306, 
         202,       13,     -898,      -55,      101,     -623,      -81,     -388, 
       -1001,     -527,       30,      365,     -683,      850,     -705,     -526, 
         405,     -551,     -315,      378,     -311,      236,      337,     1010, 
          50,     -612,       78,      -59,     1021,      537,      861,    -1015, 
        -896,     -998,     -269,      830,     -982,      747,      991,     -991, 
        -353,      482,      713,       58,     -101,     -757,      271,      178, 
         199,      -62,     1004,     -738,      249,      625,     -279,      429, 
        -897,     -131,      410,      311,     -602,      765,     -729,     -153, 
};

const WORD dspMagSqTestZLengths[] = {
         128, 
};

const SWORD dspMagSqTestZVectors[] = {
        5590,     2162,     2537,      194,       73,     1070,     2129,     3204, 
        1126,     3201,      694,     2340,     2198,     4052,     2082,     4860, 
        3814,     1798,     1545,     2701,     6101,     3287,     3336,     3073, 
        3194,     3963,     3766,     5072,      725,     2903,     2199,     1895, 
        1717,     3742,      384,     2335,     3799,     5024,     1305,     1932, 
         934,     5710,     1037,      633,     2308,     1151,     2576,     3720, 
         344,     3133,       38,     1912,     7282,      651,     3945,     3360, 
         519,     1553,     4070,     5601,     1959,     4109,     3657,      495, 
        3191,     2116,     5225,      799,     1313,     3161,     1357,     4271, 
        4055,     3788,      473,     3402,     2410,     6179,     4046,     2777, 
         648,     1396,     1468,      719,     2280,      640,      831,     4984, 
        2700,     4147,     2773,     1273,     6080,     3570,     4164,     4773, 
        3543,     5050,      672,     3625,     3808,     2552,     5491,     3854, 
         504,     1128,     2120,      142,      563,     5277,     3687,     3060, 
         272,     4063,     4375,     3378,      344,     2799,     1006,     1974, 
        3181,     2235,      687,     1116,     1891,     2402,     3646,     1887, 
};

const WORD dspMagSqTestShiftLengths[] = {
           1, 
};

const WORD dspMagSqTestShiftVectors[] = {
           8, 
};
