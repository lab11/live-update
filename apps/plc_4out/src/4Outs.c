/* This is auto-generated code from LDmicro. Do not edit this file! Go
   back to the ladder diagram source for changes in the logic, and make
   any C additions either in ladder.h or in additional .c files linked
   against this one. */

/* You must provide ladder.h; there you must provide:
      * a typedef for SWORD and BOOL, signed 16 bit and boolean types
        (probably typedef signed short SWORD; typedef unsigned char BOOL;)

   You must also provide implementations of all the I/O read/write
   either as inlines in the header file or in another source file. (The
   I/O functions are all declared extern.)

   See the generated source code (below) for function names. */
#include "ladder.h"

/* Define EXTERN_EVERYTHING in ladder.h if you want all symbols extern.
   This could be useful to implement `magic variables,' so that for
   example when you write to the ladder variable duty_cycle, your PLC
   runtime can look at the C variable U_duty_cycle and use that to set
   the PWM duty cycle on the micro. That way you can add support for
   peripherals that LDmicro doesn't know about. */
#ifdef EXTERN_EVERYTHING
#define STATIC 
#else
#define STATIC static
#endif

/* Define NO_PROTOTYPES if you don't want LDmicro to provide prototypes for
   all the I/O functions (Read_U_xxx, Write_U_xxx) that you must provide.
   If you define this then you must provide your own prototypes for these
   functions in ladder.h, or provide definitions (e.g. as inlines or macros)
   for them in ladder.h. */
#ifdef NO_PROTOTYPES
#define PROTO(x)
#else
#define PROTO(x) x
#endif

/* U_xxx symbols correspond to user-defined names. There is such a symbol
   for every internal relay, variable, timer, and so on in the ladder
   program. I_xxx symbols are internally generated. */
STATIC BOOL I_b_mcr = 0;
#define Read_I_b_mcr() I_b_mcr
#define Write_I_b_mcr(x) I_b_mcr = x
STATIC BOOL I_b_rung_top = 0;
#define Read_I_b_rung_top() I_b_rung_top
#define Write_I_b_rung_top(x) I_b_rung_top = x
STATIC BOOL I_b_parOut_0000 = 0;
#define Read_I_b_parOut_0000() I_b_parOut_0000
#define Write_I_b_parOut_0000(x) I_b_parOut_0000 = x
STATIC BOOL I_b_parThis_0000 = 0;
#define Read_I_b_parThis_0000() I_b_parThis_0000
#define Write_I_b_parThis_0000(x) I_b_parThis_0000 = x

/* You provide this function. */
PROTO(extern BOOL Read_U_b_XMStart(void);)

STATIC BOOL U_b_RMaster = 0;
#define Read_U_b_RMaster() U_b_RMaster
#define Write_U_b_RMaster(x) U_b_RMaster = x

/* You provide this function. */
PROTO(extern BOOL Read_U_b_XStop(void);)

STATIC BOOL U_b_RDone3 = 0;
#define Read_U_b_RDone3() U_b_RDone3
#define Write_U_b_RDone3(x) U_b_RDone3 = x
STATIC SWORD U_i_T0 = 0;
STATIC BOOL I_b_parOut_0001 = 0;
#define Read_I_b_parOut_0001() I_b_parOut_0001
#define Write_I_b_parOut_0001(x) I_b_parOut_0001 = x
STATIC BOOL I_b_parThis_0001 = 0;
#define Read_I_b_parThis_0001() I_b_parThis_0001
#define Write_I_b_parThis_0001(x) I_b_parThis_0001 = x
STATIC BOOL U_b_RDone0 = 0;
#define Read_U_b_RDone0() U_b_RDone0
#define Write_U_b_RDone0(x) U_b_RDone0 = x
STATIC SWORD U_i_T1 = 0;
STATIC BOOL I_b_parOut_0002 = 0;
#define Read_I_b_parOut_0002() I_b_parOut_0002
#define Write_I_b_parOut_0002(x) I_b_parOut_0002 = x
STATIC BOOL I_b_parThis_0002 = 0;
#define Read_I_b_parThis_0002() I_b_parThis_0002
#define Write_I_b_parThis_0002(x) I_b_parThis_0002 = x
STATIC BOOL U_b_RDone1 = 0;
#define Read_U_b_RDone1() U_b_RDone1
#define Write_U_b_RDone1(x) U_b_RDone1 = x
STATIC SWORD U_i_T2 = 0;
STATIC BOOL I_b_parOut_0003 = 0;
#define Read_I_b_parOut_0003() I_b_parOut_0003
#define Write_I_b_parOut_0003(x) I_b_parOut_0003 = x
STATIC BOOL I_b_parThis_0003 = 0;
#define Read_I_b_parThis_0003() I_b_parThis_0003
#define Write_I_b_parThis_0003(x) I_b_parThis_0003 = x
STATIC BOOL U_b_RDone2 = 0;
#define Read_U_b_RDone2() U_b_RDone2
#define Write_U_b_RDone2(x) U_b_RDone2 = x
STATIC SWORD U_i_T3 = 0;

/* You provide these functions. */
PROTO(BOOL Read_U_b_YOut0(void);)
PROTO(void Write_U_b_YOut0(BOOL v);)


/* You provide these functions. */
PROTO(BOOL Read_U_b_YOut1(void);)
PROTO(void Write_U_b_YOut1(BOOL v);)


/* You provide these functions. */
PROTO(BOOL Read_U_b_YOut2(void);)
PROTO(void Write_U_b_YOut2(BOOL v);)


/* You provide these functions. */
PROTO(BOOL Read_U_b_YOut3(void);)
PROTO(void Write_U_b_YOut3(BOOL v);)



/* Call this function once per PLC cycle. You are responsible for calling
   it at the interval that you specified in the MCU configuration when you
   generated this code. */
void PlcCycle(void)
{
    Write_I_b_mcr(1);
    
    /* start rung 1 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    /* start parallel [ */
    Write_I_b_parOut_0000(0);
    Write_I_b_parThis_0000(Read_I_b_rung_top());
    if(!Read_U_b_XMStart()) {
        Write_I_b_parThis_0000(0);
    }
    
    if(Read_I_b_parThis_0000()) {
        Write_I_b_parOut_0000(1);
    }
    Write_I_b_parThis_0000(Read_I_b_rung_top());
    if(!Read_U_b_RMaster()) {
        Write_I_b_parThis_0000(0);
    }
    
    if(Read_I_b_parThis_0000()) {
        Write_I_b_parOut_0000(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0000());
    /* ] finish parallel */
    if(Read_U_b_XStop()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_U_b_RDone3()) {
        Write_I_b_rung_top(0);
    }
    
    Write_U_b_RMaster(Read_I_b_rung_top());
    
    /* ] finish series */
    
    /* start rung 2 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_RMaster()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_I_b_rung_top()) {
        if(U_i_T0 < 500) {
            U_i_T0++;
            Write_I_b_rung_top(0);
        }
    } else {
        U_i_T0 = 0;
    }
    
    /* start parallel [ */
    Write_I_b_parOut_0001(0);
    Write_I_b_parThis_0001(Read_I_b_rung_top());
    Write_U_b_RDone0(Read_I_b_parThis_0001());
    
    if(Read_I_b_parThis_0001()) {
        Write_I_b_parOut_0001(1);
    }
    Write_I_b_parThis_0001(Read_I_b_rung_top());
    /* start series [ */
    if(Read_I_b_parThis_0001()) {
        if(U_i_T1 < 500) {
            U_i_T1++;
            Write_I_b_parThis_0001(0);
        }
    } else {
        U_i_T1 = 0;
    }
    
    /* start parallel [ */
    Write_I_b_parOut_0002(0);
    Write_I_b_parThis_0002(Read_I_b_parThis_0001());
    Write_U_b_RDone1(Read_I_b_parThis_0002());
    
    if(Read_I_b_parThis_0002()) {
        Write_I_b_parOut_0002(1);
    }
    Write_I_b_parThis_0002(Read_I_b_parThis_0001());
    /* start series [ */
    if(Read_I_b_parThis_0002()) {
        if(U_i_T2 < 500) {
            U_i_T2++;
            Write_I_b_parThis_0002(0);
        }
    } else {
        U_i_T2 = 0;
    }
    
    /* start parallel [ */
    Write_I_b_parOut_0003(0);
    Write_I_b_parThis_0003(Read_I_b_parThis_0002());
    Write_U_b_RDone2(Read_I_b_parThis_0003());
    
    if(Read_I_b_parThis_0003()) {
        Write_I_b_parOut_0003(1);
    }
    Write_I_b_parThis_0003(Read_I_b_parThis_0002());
    /* start series [ */
    if(Read_I_b_parThis_0003()) {
        if(U_i_T3 < 500) {
            U_i_T3++;
            Write_I_b_parThis_0003(0);
        }
    } else {
        U_i_T3 = 0;
    }
    
    Write_U_b_RDone3(Read_I_b_parThis_0003());
    
    /* ] finish series */
    if(Read_I_b_parThis_0003()) {
        Write_I_b_parOut_0003(1);
    }
    Write_I_b_parThis_0002(Read_I_b_parOut_0003());
    /* ] finish parallel */
    /* ] finish series */
    if(Read_I_b_parThis_0002()) {
        Write_I_b_parOut_0002(1);
    }
    Write_I_b_parThis_0001(Read_I_b_parOut_0002());
    /* ] finish parallel */
    /* ] finish series */
    if(Read_I_b_parThis_0001()) {
        Write_I_b_parOut_0001(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0001());
    /* ] finish parallel */
    /* ] finish series */
    
    /* start rung 3 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_RMaster()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_U_b_RDone0()) {
        Write_I_b_rung_top(0);
    }
    
    Write_U_b_YOut0(Read_I_b_rung_top());
    
    /* ] finish series */
    
    /* start rung 4 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_RMaster()) {
        Write_I_b_rung_top(0);
    }
    
    if(!Read_U_b_RDone0()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_U_b_RDone1()) {
        Write_I_b_rung_top(0);
    }
    
    Write_U_b_YOut1(Read_I_b_rung_top());
    
    /* ] finish series */
    
    /* start rung 5 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_RMaster()) {
        Write_I_b_rung_top(0);
    }
    
    if(!Read_U_b_RDone1()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_U_b_RDone2()) {
        Write_I_b_rung_top(0);
    }
    
    Write_U_b_YOut2(Read_I_b_rung_top());
    
    /* ] finish series */
    
    /* start rung 6 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_RMaster()) {
        Write_I_b_rung_top(0);
    }
    
    if(!Read_U_b_RDone2()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_U_b_RDone3()) {
        Write_I_b_rung_top(0);
    }
    
    Write_U_b_YOut3(Read_I_b_rung_top());
    
    /* ] finish series */
}
