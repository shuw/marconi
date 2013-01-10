#ifndef __BURST_COMMON_H__
#define __BURST_COMMON_H__


#define BURST_PAYLOAD_LENGTH    8


// Generated in Matlab:
// sync = randint(1, 20, [0 1]); disp(sprintf('%d ', sync)); disp(sprintf('%x', packBits(sync, 20, 'msbToLsb')));
#define BURST_SYNC              0x000f4c91UL
#define BURST_SYNC_BITS         20 
#define BURST_BYTE_BITS         8
#define BURST_LENGTH_BITS       4
#define BURST_CHECKSUM_BITS     8

#define BURST_TOTAL_BITS        ( BURST_PAYLOAD_LENGTH * BURST_BYTE_BITS + \
                                  BURST_SYNC_BITS + BURST_LENGTH_BITS + BURST_CHECKSUM_BITS )

#endif
