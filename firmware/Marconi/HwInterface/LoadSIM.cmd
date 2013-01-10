/* Options */

-q              /* Quiet (no banner) */
-c              /* Autoinitialize variables at run time */
-heap 0x2000    /* Heap size for C lib: large heap necessary for file I/O in simulation */
-stack 0x400    /* Stack size */
-w              /* Warn if there are undefined output sections */
-x              /* Exhaustively read libraries */

/* Memory map and Sections */

MEMORY
{
    PAGE 0 :
        VSTUBS          : origin = 0x000042, length = 0x000040      /* For .reset: not actually used */
        XINTF0          : origin = 0x080000, length = 0x080000      /* External simulation RAM */
    PAGE 1 : 
        XINTF1          : origin = 0x100000, length = 0x080000      /* External simulation RAM */
        MSARAM          : origin = 0x000000, length = 0x000800      /* RAM */
}

SECTIONS
{
    .reset              : > VSTUBS,   PAGE = 0, type = DSECT  /* Dummy section not in image */
    .text               : > XINTF0,   PAGE = 0
    .cinit              : > XINTF0,   PAGE = 0
    .pinit              : > XINTF0,   PAGE = 0
    .econst             : > XINTF0,   PAGE = 0
    .switch             : > XINTF0,   PAGE = 0
    .stack              : > MSARAM,   PAGE = 1
    .esysmem            : > XINTF1,   PAGE = 1
    .cio                : > XINTF1,   PAGE = 1
    .ebss               : > XINTF1,   PAGE = 1
 }
