/* Options */

-q              /* Quiet (no banner) */
-c              /* Autoinitialize variables at run time */
-heap 0x200     /* Heap size for C lib */
-stack 0x400    /* Stack size */
-w              /* Warn if there are undefined output sections */
-x              /* Exhaustively read libraries */

/* Memory map and Sections */

MEMORY
{
    PAGE 0 :
        VSTUBS          : origin = 0x000042, length = 0x000040      /* For .reset: not actually used */
        HSARAM          : origin = 0x3f8000, length = 0x002000      /* RAM */
        FLASH           : origin = 0x3D8000, length = 0x010000      /* FLASH memory (first half only)*/
        FLASH_ENTRY     : origin = 0x3F7FF6, length = 0x000002      /* Entry point when booting from flash */
    PAGE 1 : 
        MSARAM          : origin = 0x000000, length = 0x000800      /* RAM */
        LSARAM          : origin = 0x008000, length = 0x002000      /* RAM */
        DEV_EMU         : origin = 0x000880, length = 0x000180      /* device emulation registers */
        PIE_VECT        : origin = 0x000D00, length = 0x000100      /* PIE Vector Table */
        FLASH_REGS      : origin = 0x000A80, length = 0x000060      /* FLASH registers */
        CSM             : origin = 0x000AE0, length = 0x000010      /* code security module registers */
        XINTF           : origin = 0x000B20, length = 0x000020      /* external interface registers */
        CPU_TIMER0      : origin = 0x000C00, length = 0x000008      /* CPU Timer0 registers */
        PIE_CTRL        : origin = 0x000CE0, length = 0x000020      /* PIE control registers */
        ECANA           : origin = 0x006000, length = 0x000040      /* eCAN control and status registers */ 
        ECANA_LAM       : origin = 0x006040, length = 0x000040      /* eCAN local acceptance masks */
        ECANA_MOTS      : origin = 0x006080, length = 0x000040      /* eCAN message object time stamps */
        ECANA_MOTO      : origin = 0x0060C0, length = 0x000040      /* eCAN object time-out registers */
        ECANA_MBOX      : origin = 0x006100, length = 0x000100      /* eCAN mailboxes */
        SYSTEM          : origin = 0x007010, length = 0x000020      /* System control registers */
        SPIA            : origin = 0x007040, length = 0x000010      /* SPI registers */
        SCIA            : origin = 0x007050, length = 0x000010      /* SCI-A registers */
        XINTRUPT        : origin = 0x007070, length = 0x000010      /* external interrupt registers */
        GPIOMUX         : origin = 0x0070C0, length = 0x000020      /* GPIO mux registers */
        GPIODAT         : origin = 0x0070E0, length = 0x000020      /* GPIO data registers */
        ADC             : origin = 0x007100, length = 0x000020      /* ADC registers */
        EVA             : origin = 0x007400, length = 0x000040      /* Event Manager A registers */
        EVB             : origin = 0x007500, length = 0x000040      /* Event Manager B registers */
        SCIB            : origin = 0x007750, length = 0x000010      /* SCI-B registers */
        MCBSPA          : origin = 0x007800, length = 0x000040      /* McBSP registers */
        CSM_PWL         : origin = 0x3F7FF8, length = 0x000008      /* Part of FLASHA.  CSM password locations. */
}

SECTIONS
{
    .reset              : > VSTUBS,         PAGE = 0, type = DSECT  /* Dummy section not in image */
    .text               : > HSARAM,         PAGE = 0
    .cinit              : > HSARAM,         PAGE = 0
    .pinit              : > HSARAM,         PAGE = 0
    .econst             : > HSARAM,         PAGE = 0
    .switch             : > HSARAM,         PAGE = 0
    .stack              : > MSARAM,         PAGE = 1
    .sysmem             : > MSARAM,         PAGE = 1
    .esysmem            : > MSARAM,         PAGE = 1 /* TODO: TEMP */
    .cio                : > LSARAM,         PAGE = 1
    .ebss               : > LSARAM,         PAGE = 1

    CodeEntry           : > HSARAM,         PAGE = 0
    RamCode             : LOAD = HSARAM,
                          RUN = HSARAM,
                          LOAD_START(_RamCodeLoadStart),
                          LOAD_END(_RamCodeLoadEnd),
                          RUN_START(_RamCodeRunStart),
                          PAGE = 0
    
    PieVectTableFile    : > PIE_VECT,   PAGE = 1

    /*** Peripheral Frame 0 Register Structures ***/
    DevEmuRegsFile      : > DEV_EMU,     PAGE = 1
    FlashRegsFile       : > FLASH_REGS,  PAGE = 1
    CsmRegsFile         : > CSM,         PAGE = 1
    XintfRegsFile       : > XINTF,       PAGE = 1
    CpuTimer0RegsFile   : > CPU_TIMER0,  PAGE = 1  
    PieCtrlRegsFile     : > PIE_CTRL,    PAGE = 1      

    /*** Peripheral Frame 1 Register Structures ***/
    SysCtrlRegsFile     : > SYSTEM,      PAGE = 1
    SpiaRegsFile        : > SPIA,        PAGE = 1
    SciaRegsFile        : > SCIA,        PAGE = 1
    XIntruptRegsFile    : > XINTRUPT,    PAGE = 1
    GpioMuxRegsFile     : > GPIOMUX,     PAGE = 1
    GpioDataRegsFile    : > GPIODAT      PAGE = 1
    AdcRegsFile         : > ADC,         PAGE = 1
    EvaRegsFile         : > EVA,         PAGE = 1
    EvbRegsFile         : > EVB,         PAGE = 1
    ScibRegsFile        : > SCIB,        PAGE = 1
    McbspaRegsFile      : > MCBSPA,      PAGE = 1
    
    /*** Peripheral Frame 2 Register Structures ***/
    ECanaRegsFile       : > ECANA,       PAGE = 1
    ECanaLAMRegsFile    : > ECANA_LAM    PAGE = 1   
    ECanaMboxesFile     : > ECANA_MBOX   PAGE = 1
    ECanaMOTSRegsFile   : > ECANA_MOTS   PAGE = 1
    ECanaMOTORegsFile   : > ECANA_MOTO   PAGE = 1

    /*** Code Security Module Register Structures ***/
    CsmPwlFile          : > CSM_PWL,     PAGE = 1   
}
