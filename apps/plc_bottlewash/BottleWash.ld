LDmicro0.1
MICRO=ANSI C Code
CYCLE=10000
CRYSTAL=4000000
BAUD=2400
COMPILED=C:\Users\Ryan Tsang\Documents\LDMicro\BottleWash.c

IO LIST
    XBottle_IN at 0
    XBottle_UP at 0
    XPosition_DOWN at 0
    XStart at 0
    XStop at 0
    YBottle_HOLD at 0
    YBottle_RELEASE at 0
    YMotor_FORWARD at 0
    YMotor_REVERSE at 0
    YWater_ON at 0
END

PROGRAM
RUNG
    PARALLEL
        CONTACTS XStart 0
        CONTACTS RMaster 0
    END
    CONTACTS XStop 1
    COIL RMaster 0 0 0
END
RUNG
    CONTACTS RMaster 0
    PARALLEL
        CONTACTS XBottle_IN 0
        CONTACTS RBottle_HOLD 0
    END
    CONTACTS RBottle_RELEASE 1
    PARALLEL
        COIL YBottle_HOLD 0 0 0
        COIL RBottle_HOLD 0 0 0
    END
END
RUNG
    CONTACTS RMaster 0
    CONTACTS RBottle_HOLD 0
    TON T0 1000000
    COIL RHOLD_DN 0 0 0
END
RUNG
    CONTACTS RMaster 0
    CONTACTS RHOLD_DN 0
    CONTACTS RWater_ON 1
    CONTACTS RMotor_REVERSE 1
    PARALLEL
        COIL YMotor_FORWARD 0 0 0
        COIL RMotor_FORWARD 0 0 0
    END
END
RUNG
    CONTACTS RMaster 0
    PARALLEL
        CONTACTS XPosition_DOWN 0
        CONTACTS RWater_ON 0
    END
    CONTACTS RWater_OFF 1
    PARALLEL
        COIL YWater_ON 0 0 0
        COIL RWater_ON 0 0 0
    END
END
RUNG
    CONTACTS RMaster 0
    CONTACTS RWater_ON 0
    TON T1 5000000
    COIL RWater_OFF 0 0 0
END
RUNG
    CONTACTS RMaster 0
    PARALLEL
        CONTACTS RWater_OFF 0
        CONTACTS RMotor_REVERSE 0
    END
    CONTACTS XBottle_UP 1
    PARALLEL
        COIL YMotor_REVERSE 0 0 0
        COIL RMotor_REVERSE 0 0 0
    END
END
RUNG
    CONTACTS RMaster 0
    PARALLEL
        CONTACTS XBottle_UP 0
        CONTACTS RBottle_RELEASE 0
    END
    CONTACTS XBottle_IN 1
    TON T2 1000000
    PARALLEL
        COIL YBottle_RELEASE 0 0 0
        COIL RBottle_RELEASE 0 0 0
    END
END
