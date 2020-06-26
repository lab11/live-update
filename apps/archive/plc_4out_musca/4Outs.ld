LDmicro0.1
MICRO=ANSI C Code
CYCLE=10000
CRYSTAL=4000000
BAUD=2400
COMPILED=C:\Users\Ryan Tsang\Documents\LDMicro\4Outs.c

IO LIST
    XMStart at 0
    XStop at 0
    YOut0 at 0
    YOut1 at 0
    YOut2 at 0
    YOut3 at 0
END

PROGRAM
RUNG
    PARALLEL
        CONTACTS XMStart 0
        CONTACTS RMaster 0
    END
    CONTACTS XStop 1
    CONTACTS RDone3 1
    COIL RMaster 0 0 0
END
RUNG
    CONTACTS RMaster 0
    TON T0 5000000
    PARALLEL
        COIL RDone0 0 0 0
        SERIES
            TON T1 5000000
            PARALLEL
                COIL RDone1 0 0 0
                SERIES
                    TON T2 5000000
                    PARALLEL
                        COIL RDone2 0 0 0
                        SERIES
                            TON T3 5000000
                            COIL RDone3 0 0 0
                        END
                    END
                END
            END
        END
    END
END
RUNG
    CONTACTS RMaster 0
    CONTACTS RDone0 1
    COIL YOut0 0 0 0
END
RUNG
    CONTACTS RMaster 0
    CONTACTS RDone0 0
    CONTACTS RDone1 1
    COIL YOut1 0 0 0
END
RUNG
    CONTACTS RMaster 0
    CONTACTS RDone1 0
    CONTACTS RDone2 1
    COIL YOut2 0 0 0
END
RUNG
    CONTACTS RMaster 0
    CONTACTS RDone2 0
    CONTACTS RDone3 1
    COIL YOut3 0 0 0
END
