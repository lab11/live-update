LDmicro0.1
MICRO=ANSI C Code
CYCLE=10000
CRYSTAL=4000000
BAUD=2400
COMPILED=C:\Users\Ryan Tsang\Documents\LDMicro\LEDtutorial.c

IO LIST
    Xbutton at 0
    Yled at 0
    Ytx at 0
END

PROGRAM
RUNG
    CONTACTS Rosc 0
    TON Tosc_on 250000
    TOF Tosc_off 250000
    COIL Rosc 1 0 0
END
RUNG
    CONTACTS Xbutton 0
    PARALLEL
        CTC Cstate 2
        COIL Rbon 0 0 0
    END
END
RUNG
    CONTACTS Rbon 1
    TON Tkill_delay 1000000
    COIL Rkill 0 0 0
END
RUNG
    PARALLEL
        CONTACTS Rbon 0
        CONTACTS Rhigh 0
    END
    CONTACTS Rkill 1
    PARALLEL
        COIL Rhigh 0 0 0
        COIL Ytx 0 0 0
    END
END
RUNG
    PARALLEL
        EQU Cstate 1
        SERIES
            EQU Cstate 2
            CONTACTS Rosc 0
        END
    END
    COIL Yled 0 0 0
END
