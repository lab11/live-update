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
STATIC BOOL U_b_Rosc = 0;
#define Read_U_b_Rosc() U_b_Rosc
#define Write_U_b_Rosc(x) U_b_Rosc = x
STATIC SWORD U_i_Tosc_on = 0;
STATIC BOOL I_b_Tosc_off_antiglitch = 0;
#define Read_I_b_Tosc_off_antiglitch() I_b_Tosc_off_antiglitch
#define Write_I_b_Tosc_off_antiglitch(x) I_b_Tosc_off_antiglitch = x
STATIC SWORD U_i_Tosc_off = 0;

/* You provide this function. */
PROTO(extern BOOL Read_U_b_Xbutton(void);)

STATIC BOOL I_b_parOut_0000 = 0;
#define Read_I_b_parOut_0000() I_b_parOut_0000
#define Write_I_b_parOut_0000(x) I_b_parOut_0000 = x
STATIC BOOL I_b_parThis_0000 = 0;
#define Read_I_b_parThis_0000() I_b_parThis_0000
#define Write_I_b_parThis_0000(x) I_b_parThis_0000 = x
STATIC BOOL I_b_oneShot_0000 = 0;
#define Read_I_b_oneShot_0000() I_b_oneShot_0000
#define Write_I_b_oneShot_0000(x) I_b_oneShot_0000 = x
STATIC SWORD U_i_Cstate = 0;
STATIC BOOL U_b_Rbon = 0;
#define Read_U_b_Rbon() U_b_Rbon
#define Write_U_b_Rbon(x) U_b_Rbon = x
STATIC SWORD U_i_Tkill_delay = 0;
STATIC BOOL U_b_Rkill = 0;
#define Read_U_b_Rkill() U_b_Rkill
#define Write_U_b_Rkill(x) U_b_Rkill = x
STATIC BOOL I_b_parOut_0001 = 0;
#define Read_I_b_parOut_0001() I_b_parOut_0001
#define Write_I_b_parOut_0001(x) I_b_parOut_0001 = x
STATIC BOOL I_b_parThis_0001 = 0;
#define Read_I_b_parThis_0001() I_b_parThis_0001
#define Write_I_b_parThis_0001(x) I_b_parThis_0001 = x
STATIC BOOL U_b_Rhigh = 0;
#define Read_U_b_Rhigh() U_b_Rhigh
#define Write_U_b_Rhigh(x) U_b_Rhigh = x
STATIC BOOL I_b_parOut_0002 = 0;
#define Read_I_b_parOut_0002() I_b_parOut_0002
#define Write_I_b_parOut_0002(x) I_b_parOut_0002 = x
STATIC BOOL I_b_parThis_0002 = 0;
#define Read_I_b_parThis_0002() I_b_parThis_0002
#define Write_I_b_parThis_0002(x) I_b_parThis_0002 = x

/* You provide these functions. */
PROTO(BOOL Read_U_b_Ytx(void);)
PROTO(void Write_U_b_Ytx(BOOL v);)

STATIC BOOL I_b_parOut_0003 = 0;
#define Read_I_b_parOut_0003() I_b_parOut_0003
#define Write_I_b_parOut_0003(x) I_b_parOut_0003 = x
STATIC BOOL I_b_parThis_0003 = 0;
#define Read_I_b_parThis_0003() I_b_parThis_0003
#define Write_I_b_parThis_0003(x) I_b_parThis_0003 = x
STATIC SWORD I_i_scratch2 = 0;

/* You provide these functions. */
PROTO(BOOL Read_U_b_Yled(void);)
PROTO(void Write_U_b_Yled(BOOL v);)



/* Call this function once per PLC cycle. You are responsible for calling
   it at the interval that you specified in the MCU configuration when you
   generated this code. */
void PlcCycle(void)
{
    Write_I_b_mcr(1);
    
    /* start rung 1 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_Rosc()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_I_b_rung_top()) {
        if(U_i_Tosc_on < 24) {
            U_i_Tosc_on++;
            Write_I_b_rung_top(0);
        }
    } else {
        U_i_Tosc_on = 0;
    }
    
    if(!Read_I_b_Tosc_off_antiglitch()) {
        U_i_Tosc_off = 24;
    }
    Write_I_b_Tosc_off_antiglitch(1);
    if(!Read_I_b_rung_top()) {
        if(U_i_Tosc_off < 24) {
            U_i_Tosc_off++;
            Write_I_b_rung_top(1);
        }
    } else {
        U_i_Tosc_off = 0;
    }
    
    if(Read_I_b_rung_top()) {
        Write_U_b_Rosc(0);
    } else {
        Write_U_b_Rosc(1);
    }
    
    /* ] finish series */
    
    /* start rung 2 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(!Read_U_b_Xbutton()) {
        Write_I_b_rung_top(0);
    }
    
    /* start parallel [ */
    Write_I_b_parOut_0000(0);
    Write_I_b_parThis_0000(Read_I_b_rung_top());
    if(Read_I_b_parThis_0000()) {
        if(!Read_I_b_oneShot_0000()) {
            U_i_Cstate++;
            if(U_i_Cstate < 3) {
            } else {
                U_i_Cstate = 0;
            }
        }
    }
    Write_I_b_oneShot_0000(Read_I_b_parThis_0000());
    
    if(Read_I_b_parThis_0000()) {
        Write_I_b_parOut_0000(1);
    }
    Write_I_b_parThis_0000(Read_I_b_rung_top());
    Write_U_b_Rbon(Read_I_b_parThis_0000());
    
    if(Read_I_b_parThis_0000()) {
        Write_I_b_parOut_0000(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0000());
    /* ] finish parallel */
    /* ] finish series */
    
    /* start rung 3 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    if(Read_U_b_Rbon()) {
        Write_I_b_rung_top(0);
    }
    
    if(Read_I_b_rung_top()) {
        if(U_i_Tkill_delay < 49) {
            U_i_Tkill_delay++;
            Write_I_b_rung_top(0);
        }
    } else {
        U_i_Tkill_delay = 0;
    }
    
    Write_U_b_Rkill(Read_I_b_rung_top());
    
    /* ] finish series */
    
    /* start rung 4 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    /* start parallel [ */
    Write_I_b_parOut_0001(0);
    Write_I_b_parThis_0001(Read_I_b_rung_top());
    if(!Read_U_b_Rbon()) {
        Write_I_b_parThis_0001(0);
    }
    
    if(Read_I_b_parThis_0001()) {
        Write_I_b_parOut_0001(1);
    }
    Write_I_b_parThis_0001(Read_I_b_rung_top());
    if(!Read_U_b_Rhigh()) {
        Write_I_b_parThis_0001(0);
    }
    
    if(Read_I_b_parThis_0001()) {
        Write_I_b_parOut_0001(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0001());
    /* ] finish parallel */
    if(Read_U_b_Rkill()) {
        Write_I_b_rung_top(0);
    }
    
    /* start parallel [ */
    Write_I_b_parOut_0002(0);
    Write_I_b_parThis_0002(Read_I_b_rung_top());
    Write_U_b_Rhigh(Read_I_b_parThis_0002());
    
    if(Read_I_b_parThis_0002()) {
        Write_I_b_parOut_0002(1);
    }
    Write_I_b_parThis_0002(Read_I_b_rung_top());
    Write_U_b_Ytx(Read_I_b_parThis_0002());
    
    if(Read_I_b_parThis_0002()) {
        Write_I_b_parOut_0002(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0002());
    /* ] finish parallel */
    /* ] finish series */
    
    /* start rung 5 */
    Write_I_b_rung_top(Read_I_b_mcr());
    
    /* start series [ */
    /* start parallel [ */
    Write_I_b_parOut_0003(0);
    Write_I_b_parThis_0003(Read_I_b_rung_top());
    I_i_scratch2 = 1;
    if(U_i_Cstate == I_i_scratch2) {
    } else {
        Write_I_b_parThis_0003(0);
    }
    
    if(Read_I_b_parThis_0003()) {
        Write_I_b_parOut_0003(1);
    }
    Write_I_b_parThis_0003(Read_I_b_rung_top());
    /* start series [ */
    I_i_scratch2 = 2;
    if(U_i_Cstate == I_i_scratch2) {
    } else {
        Write_I_b_parThis_0003(0);
    }
    
    if(!Read_U_b_Rosc()) {
        Write_I_b_parThis_0003(0);
    }
    
    /* ] finish series */
    if(Read_I_b_parThis_0003()) {
        Write_I_b_parOut_0003(1);
    }
    Write_I_b_rung_top(Read_I_b_parOut_0003());
    /* ] finish parallel */
    Write_U_b_Yled(Read_I_b_rung_top());
    
    /* ] finish series */
}
