
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define ONE_PLUS_EPS (1 + (1e+36)) // REAL(4), PARAMETER           :: OnePlusEps    = 1.0 + EPSILON(OnePlusEps)       ! The number slighty greater than unity in single precision.
#define PC_DT 0.000125 // REAL(4), PARAMETER           :: PC_DT         = 0.000125  !JASON:THIS CHANGED FOR ITI BARGE:      0.0001                    ! Communication interval for pitch  controller, sec.
#define PC_KI 0.008068634 // REAL(4), PARAMETER           :: PC_KI         =       0.008068634               ! Integral gain for pitch controller at rated pitch (zero), (-).
#define PC_KK 0.1099965 // REAL(4), PARAMETER           :: PC_KK         =       0.1099965                 ! Pitch angle where the the derivative of the aerodynamic power w.r.t. pitch has increased by a factor of two relative to the derivative at rated pitch (zero), rad.
#define PC_KP 0.01882681 // REAL(4), PARAMETER           :: PC_KP         =       0.01882681                ! Proportional gain for pitch controller at rated pitch (zero), sec.
#define PC_MAX_PIT 1.570796  // REAL(4), PARAMETER           :: PC_MaxPit     =       1.570796                  ! Maximum pitch setting in pitch controller, rad.
#define PC_MAX_RAT 0.1396263 // REAL(4), PARAMETER           :: PC_MaxRat     =       0.1396263                 ! Maximum pitch  rate (in absolute value) in pitch  controller, rad/s.
#define PC_MIN_PIT 0.0 // REAL(4), PARAMETER           :: PC_MinPit     =       0.0                       ! Minimum pitch setting in pitch controller, rad.
#define PC_REF_SPEED 122.9096 //REAL(4), PARAMETER           :: PC_RefSpd     =     122.9096                    ! Desired (reference) HSS speed for pitch controller, rad/s.
#define R2D 57.295780 // REAL(4), PARAMETER           :: R2D           =      57.295780                  ! Factor to convert radians to degrees.
#define RPS2RPM 9.5492966 // REAL(4), PARAMETER           :: RPS2RPM       =       9.5492966                 ! Factor to convert radians per second to revolutions per minute.
#define CORNER_FREQ 1.570796 ; //REAL(4), PARAMETER           :: CornerFreq    =       1.570796                  ! Corner frequency (-3dB point) in the recursive, single-pole, low-pass filter, rad/s. -- chosen to be 1/4 the blade edgewise natural frequency ( 1/4 of approx. 1Hz = 0.25Hz = 1.570796rad/s)

#define VS_CTINSP 70.16224 // REAL(4), PARAMETER           :: VS_CtInSp     =      70.16224                   ! Transitional generator speed (HSS side) between regions 1 and 1 1/2, rad/s.
#define VS_DT 0.000125 // REAL(4), PARAMETER           :: VS_DT         = 0.000125  !JASON:THIS CHANGED FOR ITI BARGE:      0.0001                    ! Communication interval for torque controller, sec.
#define VS_MAX_RAT 15000.0 // REAL(4), PARAMETER           :: VS_MaxRat     =   15000.0                       ! Maximum torque rate (in absolute value) in torque controller, N-m/s.
#define VS_MAX_TQ 47402.91 // REAL(4), PARAMETER           :: VS_MaxTq      =   47402.91                      ! Maximum generator torque in Region 3 (HSS side), N-m. -- chosen to be 10% above VS_RtTq = 43.09355kNm
#define VS_RGN2K 2.332287 // REAL(4), PARAMETER           :: VS_Rgn2K      =       2.332287                  ! Generator torque constant in Region 2 (HSS side), N-m/(rad/s)^2.
#define VS_RGN2SP 91.21091 // REAL(4), PARAMETER           :: VS_Rgn2Sp     =      91.21091                   ! Transitional generator speed (HSS side) between regions 1 1/2 and 2, rad/s.
#define VS_RGN3MP 0.01745329 // REAL(4), PARAMETER           :: VS_Rgn3MP     =       0.01745329                ! Minimum pitch angle at which the torque is computed as if we are in region 3 regardless of the generator speed, rad. -- chosen to be 1.0 degree above PC_MinPit
#define VS_RTGNSP 121.6805 // REAL(4), PARAMETER           :: VS_RtGnSp     =     121.6805                    ! Rated generator speed (HSS side), rad/s. -- chosen to be 99% of PC_RefSpd
#define VS_RTPWR 5296610.0 //  REAL(4), PARAMETER           :: VS_RtPwr      = 5296610.0                       ! Rated generator generator power in Region 3, Watts. -- chosen to be 5MW divided by the electrical generator efficiency of 94.4%

#define VS_SLPC 10.0 // REAL(4), PARAMETER           :: VS_SlPc       =      10.0                       ! Rated generator slip percentage in Region 2 1/2, %.
#define UN_DB 85 // INTEGER(4), PARAMETER        :: UnDb          = 85                              ! I/O unit for the debugging information
#define UN_DB2 85 // INTEGER(4), PARAMETER        :: UnDb2         = 86                              ! I/O unit for the debugging information
#define UN 85 // INTEGER(4), PARAMETER        :: Un            = 87                              ! I/O unit for pack/unpack (checkpoint & restart)
#define PC_DBG_OUT 0 // LOGICAL(1), PARAMETER        :: PC_DbgOut     = .FALSE.                         ! Flag to indicate whether to output debugging information

#define TAB '\t' // CHARACTER(   1), PARAMETER   :: Tab           = CHAR( 9 )                       ! The tab character.
#define FMT_DAT "(F8.3,99('\"//Tab//\"',ES10.3E2,:))" // CHARACTER(  25), PARAMETER   :: FmtDat    = "(F8.3,99('"//Tab//"',ES10.3E2,:))" ! The format of the debugging data

float from_sc(void) { // !REAL(C_FLOAT),          INTENT(IN   ) :: from_SC   (*)  ! DATA from the supercontroller
  return 0.0 // TODO: Return data from controller
}

void to_sc(int index, float input) { // !REAL(C_FLOAT),          INTENT(INOUT) :: to_SC     (*)  ! DATA to the supercontroller
  // TODO: Send data to controller
}

float avr_swap (int sent_index) { // REAL(C_FLOAT),          INTENT(INOUT) :: avrSWAP   (*)                  ! The swap array, used to pass data to, and receive data from, the DLL controller.
  // to_sc(sent_data, );
  return from_sc(sent_data);
}

int AVI_FAIL = 0; // INTEGER(C_INT),         INTENT(INOUT) :: aviFAIL                        ! A flag used to indicate the success of this DLL call set as follows: 0 if the DLL call was successful, >0 if the DLL call was successful but cMessage should be issued as a warning messsage, <0 if the DLL call was unsuccessful or for any other reason the simulation is to be stopped at this point with cMessage as the error message. // aviFAIL      = 0

/*
CHARACTER(KIND=C_CHAR), INTENT(IN)    :: accINFILE (NINT(avrSWAP(50)))  ! The name of the parameter input file, 'DISCON.IN'.
CHARACTER(KIND=C_CHAR), INTENT(IN)    :: avcOUTNAME(NINT(avrSWAP(51)))  ! OUTNAME (Simulation RootName)
CHARACTER(KIND=C_CHAR), INTENT(INOUT) :: avcMSG    (NINT(avrSWAP(49)))  ! MESSAGE (Message from DLL to simulation code [ErrMsg])  The message which will be displayed by the calling program if aviFAIL <> 0.
*/

float alpha; // REAL(4)                      :: Alpha                                           ! Current coefficient in the recursive, single-pole, low-pass filter, (-).
float bl_pitch[3]; // REAL(4)                      :: BlPitch   (3)                                   ! Current values of the blade pitch angles, rad.
float elap_time; // REAL(4)                      :: ElapTime                                        ! Elapsed time since the last call to the controller, sec.
float gen_speed = avr_swap(20); // REAL(4)                      :: GenSpeed                                        ! Current  HSS (generator) speed, rad/s. // GenSpeed     =       avrSWAP(20)
static float gen_speed_f; // REAL(4), SAVE                :: GenSpeedF                                       ! Filtered HSS (generator) speed, rad/s.
float gen_trq; // REAL(4)                      :: GenTrq                                          ! Electrical generator torque, N-m.
float gain_correction; // REAL(4)                      :: GK                                              ! Current value of the gain correction factor, used in the gain scheduling law of the pitch controller, (-).
float hor_wind_v = avr_swap(27); //REAL(4)                      :: HorWindV                                        ! Horizontal hub-heigh wind speed, m/s. // HorWindV     =       avrSWAP(27)
float int_spd_err; // REAL(4), SAVE                :: IntSpdErr                                       ! Current integral of speed error w.r.t. time, rad.
static float last_gen_torque; // REAL(4), SAVE                :: LastGenTrq                                      ! Commanded electrical generator torque the last time the controller was called, N-m.
static float last_time; // REAL(4), SAVE                :: LastTime                                        ! Last time this DLL was called, sec.
static float last_time_pc; // REAL(4), SAVE                :: LastTimePC                                      ! Last time the pitch  controller was called, sec.
static float last_time_vs; // REAL(4), SAVE                :: LastTimeVS                                      ! Last time the torque controller was called, sec.
static float pit_com = 3; //REAL(4), SAVE                :: PitCom    (3)                                   ! Commanded pitch of each blade the last time the controller was called, rad.
float pit_com_i; // REAL(4)                      :: PitComI                                         ! Integral term of command pitch, rad.
float pit_com_p; // REAL(4)                      :: PitComP                                         ! Proportional term of command pitch, rad.
float pit_com_t; // REAL(4)                      :: PitComT                                         ! Total command pitch based on the sum of the proportional and integral terms, rad.
float pit_rate; // REAL(4)                      :: PitRate   (3)                                   ! Pitch rates of each blade based on the current pitch angles and current pitch command, rad/s.

float spd_err; // REAL(4)                      :: SpdErr                                          ! Current speed error, rad/s.
float time = avr_swap(2); // REAL(4)                      :: Time                                            ! Current simulation time, sec. // Time         =       avrSWAP( 2)
float trq_rate; // REAL(4)                      :: TrqRate                                         ! Torque rate based on the current and last torque commands, N-m/s.
float vs_slope15; // REAL(4), SAVE                :: VS_Slope15                                      ! Torque/speed slope of region 1 1/2 cut-in torque ramp , N-m/(rad/s).
float vs_slope25; // REAL(4), SAVE                :: VS_Slope25                                      ! Torque/speed slope of region 2 1/2 induction generator, N-m/(rad/s).
float vs_sy_sp; // REAL(4), SAVE                :: VS_SySp                                         ! Synchronous speed of region 2 1/2 induction generator, rad/s.
static float vs_tr_gn_sp; // REAL(4), SAVE                :: VS_TrGnSp                                       ! Transitional generator speed (HSS side) between regions 2 and 2 1/2, rad/s.

int i_status = (int)(avr_swap(1) + 0.5); // INTEGER(4)                   :: iStatus                                         ! A status flag set by the simulation as follows: 0 if this is the first call, 1 for all subsequent time steps, -1 if this is the final call at the end of the simulation. // iStatus      = NINT( avrSWAP( 1) )
int k; // INTEGER(4)                   :: K                                               ! Loops through blades.
int num_bl = (int)(avr_swap(61) + 0.5); // INTEGER(4)                   :: NumBl                                           ! Number of blades, (-). // NumBl        = NINT( avrSWAP(61) )
int err_stat; // INTEGER(4)                   :: ErrStat

char *in_file; // CHARACTER(SIZE(accINFILE)-1) :: InFile                                          ! a Fortran version of the input C string (not considered an array here)    [subtract 1 for the C null-character]
char *root_name; // CHARACTER(SIZE(avcOUTNAME)-1):: RootName                                        ! a Fortran version of the input C string (not considered an array here)    [subtract 1 for the C null-character]
char *err_msg; // CHARACTER(SIZE(avcMSG)-1)    :: ErrMsg                                          ! a Fortran version of the C string argument (not considered an array here) [subtract 1 for the C null-character]

// ! Load variables from calling program (See Appendix A of Bladed User's Guide):

printf("from_sc: %f, %f, %f", from_sc(), from_sc(), from_sc()); // !print *, 'from_sc: ', from_sc(1:4)
to_sc(1, 5.0); // !to_sc(1) = 5.0;
to_sc(2, 2.0); // !to_sc(2) = 2.0;

bl_pitch[0] = MIN(MAX(avr_swap(4), PC_MIN_PIT), PC_MAX_PIT); // !BlPitch  (1) =       MIN( MAX( avrSWAP( 4), PC_MinPit ), PC_MaxPit )    ! assume that blade pitch can't exceed limits // BlPitch  (1) =       avrSWAP( 4)
bl_pitch[0] = MIN(MAX(avr_swap(33), PC_MIN_PIT), PC_MAX_PIT); // !BlPitch  (2) =       MIN( MAX( avrSWAP(33), PC_MinPit ), PC_MaxPit )    ! assume that blade pitch can't exceed limits // BlPitch  (2) =       avrSWAP(33)
bl_pitch[0] = MIN(MAX(avr_swap(34), PC_MIN_PIT), PC_MAX_PIT); // !BlPitch  (3) =       MIN( MAX( avrSWAP(34), PC_MinPit ), PC_MaxPit )    ! assume that blade pitch can't exceed limits // BlPitch  (3) =       avrSWAP(34)
