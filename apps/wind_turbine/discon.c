
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

#define FMT_DAT "(F8.3,99(\t,ES10.3E2,:))" // CHARACTER(  25), PARAMETER   :: FmtDat    = "(F8.3,99('"//Tab//"',ES10.3E2,:))" ! The format of the debugging data

static float gen_speed_f; // REAL(4), SAVE                :: GenSpeedF                                       ! Filtered HSS (generator) speed, rad/s.
static float last_gen_torque; // REAL(4), SAVE                :: LastGenTrq                                      ! Commanded electrical generator torque the last time the controller was called, N-m.
static float last_time; // REAL(4), SAVE                :: LastTime                                        ! Last time this DLL was called, sec.
static float last_time_pc; // REAL(4), SAVE                :: LastTimePC                                      ! Last time the pitch  controller was called, sec.
static float last_time_vs; // REAL(4), SAVE                :: LastTimeVS                                      ! Last time the torque controller was called, sec.
static float pit_com[3]; //REAL(4), SAVE                :: PitCom    (3)                                   ! Commanded pitch of each blade the last time the controller was called, rad.
static float vs_tr_gn_sp; // REAL(4), SAVE                :: VS_TrGnSp                                       ! Transitional generator speed (HSS side) between regions 2 and 2 1/2, rad/s.
static float vs_slope15; // REAL(4), SAVE                :: VS_Slope15                                      ! Torque/speed slope of region 1 1/2 cut-in torque ramp , N-m/(rad/s).
static float vs_slope25; // REAL(4), SAVE                :: VS_Slope25                                      ! Torque/speed slope of region 2 1/2 induction generator, N-m/(rad/s).
static float vs_sy_sp; // REAL(4), SAVE                :: VS_SySp                                         ! Synchronous speed of region 2 1/2 induction generator, rad/s.

void discon(float *avrSWAP, int *aviFAIL, char *accINFILE, char *avcOUTNAME, char *avcMSG ) {

  *aviFAIL = 0; // INTEGER(C_INT),         INTENT(INOUT) :: aviFAIL                        ! A flag used to indicate the success of this DLL call set as follows: 0 if the DLL call was successful, >0 if the DLL call was successful but cMessage should be issued as a warning messsage, <0 if the DLL call was unsuccessful or for any other reason the simulation is to be stopped at this point with cMessage as the error message. // aviFAIL      = 0

  /*
  CHARACTER(KIND=C_CHAR), INTENT(IN)    :: accINFILE (NINT(avrSWAP(50)))  ! The name of the parameter input file, 'DISCON.IN'.
  CHARACTER(KIND=C_CHAR), INTENT(IN)    :: avcOUTNAME(NINT(avrSWAP(51)))  ! OUTNAME (Simulation RootName)
  CHARACTER(KIND=C_CHAR), INTENT(INOUT) :: avcMSG    (NINT(avrSWAP(49)))  ! MESSAGE (Message from DLL to simulation code [ErrMsg])  The message which will be displayed by the calling program if aviFAIL <> 0.
  */

  float alpha; // REAL(4)                      :: Alpha                                           ! Current coefficient in the recursive, single-pole, low-pass filter, (-).
  float bl_pitch[3]; // REAL(4)                      :: BlPitch   (3)                                   ! Current values of the blade pitch angles, rad.
  float elap_time; // REAL(4)                      :: ElapTime                                        ! Elapsed time since the last call to the controller, sec.
  //float gen_speed = avr_swap(20); // REAL(4)                      :: GenSpeed                                        ! Current  HSS (generator) speed, rad/s. // GenSpeed     =       avrSWAP(20)
  float gen_speed = avrSWAP[20];
  float gen_trq; // REAL(4)                      :: GenTrq                                          ! Electrical generator torque, N-m.
  float gain_correction; // REAL(4)                      :: GK                                              ! Current value of the gain correction factor, used in the gain scheduling law of the pitch controller, (-).
  //float hor_wind_v = avr_swap(27); //REAL(4)                      :: HorWindV                                        ! Horizontal hub-heigh wind speed, m/s. // HorWindV     =       avrSWAP(27)
  float hor_wind_v = avrSWAP[27];
  float int_spd_err; // REAL(4), SAVE                :: IntSpdErr                                       ! Current integral of speed error w.r.t. time, rad.
  float pit_com_i; // REAL(4)                      :: PitComI                                         ! Integral term of command pitch, rad.
  float pit_com_p; // REAL(4)                      :: PitComP                                         ! Proportional term of command pitch, rad.
  float pit_com_totalotal; // REAL(4)                      :: PitComT                                         ! Total command pitch based on the sum of the proportional and integral terms, rad.
  float pit_rate[3]; // REAL(4)                      :: PitRate   (3)                                   ! Pitch rates of each blade based on the current pitch angles and current pitch command, rad/s.

  float spd_err; // REAL(4)                      :: SpdErr                                          ! Current speed error, rad/s.
  float curr_time = avrSWAP[1]; // REAL(4)                      :: Time                                            ! Current simulation time, sec. // Time         =       avrSWAP( 2)
  float trq_rate; // REAL(4)                      :: TrqRate                                         ! Torque rate based on the current and last torque commands, N-m/s.

  int i_status = (int)(avrSWAP[0] + 0.5); // INTEGER(4)                   :: iStatus                                         ! A status flag set by the simulation as follows: 0 if this is the first call, 1 for all subsequent time steps, -1 if this is the final call at the end of the simulation. // iStatus      = NINT( avrSWAP( 1) )
  int k; // INTEGER(4)                   :: K                                               ! Loops through blades.
  int num_bl = (int)(avrSWAP[60] + 0.5); // INTEGER(4)                   :: NumBl                                           ! Number of blades, (-). // NumBl        = NINT( avrSWAP(61) )
  int err_stat; // INTEGER(4)                   :: ErrStat

  // ! Load variables from calling program (See Appendix A of Bladed User's Guide):

  bl_pitch[0] = avrSWAP[3]; // BlPitch  (1) =       avrSWAP( 4)
  bl_pitch[1] = avrSWAP[32]; // BlPitch  (2) =       avrSWAP(33)
  bl_pitch[2] = avrSWAP[33]; // BlPitch  (3) =       avrSWAP(34)

  // ! Read any External Controller Parameters specified in the User Interface
  // !   and initialize variables:

  if (i_status == 0) {// IF ( iStatus == 0 )  THEN  ! .TRUE. if we're on the first call to the DLL

    //! Inform users that we are using this user-defined routine:
    *aviFAIL = 1; // aviFAIL  = 1
    err_msg = "Running with torque and pitch control of the NREL offshore 5MW baseline wind turbine from DISCON.dll as written by J. Jonkman of NREL/NWTC for use in the IEA Annex XXIII OC3 studies."; //ErrMsg   = 'Running with torque and pitch control of the NREL offshore '// &
             // '5MW baseline wind turbine from DISCON.dll as written by J. '// &
             // 'Jonkman of NREL/NWTC for use in the IEA Annex XXIII OC3 '   // &
             // 'studies.'

    //! Determine some torque control parameters not specified directly:

    vs_sy_sp = VS_RTGNSP / (1.0 + 0.01 * VS_SLPC); // VS_SySp    = VS_RtGnSp/( 1.0 +  0.01*VS_SlPc )
    vs_slope15 = (VS_RGN2K * VS_RGN2SP * VS_RGN2SP) / (VS_RGN2SP - VS_CTINSP); // VS_Slope15 = ( VS_Rgn2K*VS_Rgn2Sp*VS_Rgn2Sp )/( VS_Rgn2Sp - VS_CtInSp )
    vs_slope25 = (VS_RTPWR / VS_RTGNSP) /(VS_RTGNSP - vs_sy_sp); // VS_Slope25 = ( VS_RtPwr/VS_RtGnSp           )/( VS_RtGnSp - VS_SySp   )
    if (VS_RGN2K == 0){ //IF ( VS_Rgn2K == 0.0 )  THEN  ! .TRUE. if the Region 2 torque is flat, and thus, the denominator in the ELSE condition is zero
       vs_tr_gn_sp = vs_sy_sp; // VS_TrGnSp = VS_SySp
    } else { // ELSE                          ! .TRUE. if the Region 2 torque is quadratic with speed
       vs_tr_gn_sp = (vs_slope15 - sqrt(vs_slope25 * (vs_slope25 - 4.0 * VS_RGN2K * vs_sy_sp))) / (2.0 * VS_RGN2K); //VS_TrGnSp = ( VS_Slope25 - SQRT( VS_Slope25*( VS_Slope25 - 4.0*VS_Rgn2K*VS_SySp ) ) )/( 2.0*VS_Rgn2K )
    } // ENDIF

    // Unnecessary because those param are all "hardcoded" for this particular wind turbine (lines 186 - 269) ! Check validity of input parameters
    // Will not have debug files (lines 272 - 296)

    // ! Initialize the SAVEd variables:
    // ! NOTE: LastGenTrq, though SAVEd, is initialized in the torque controller
    // !       below for simplicity, not here.

    gen_speed_f = gen_speed; // GenSpeedF  = GenSpeed                        ! This will ensure that generator speed filter will use the initial value of the generator speed on the first pass
    pit_com = bl_pitch; // PitCom     = BlPitch                         ! This will ensure that the variable speed controller picks the correct control region and the pitch controller picks the correct gain on the first call
    gain_correction = 1.0 / (1.0 + pit_com[1] / PC_KK); // GK         = 1.0/( 1.0 + PitCom(1)/PC_KK )   ! This will ensure that the pitch angle is unchanged if the initial SpdErr is zero
    int_spd_err = pit_com[1] / (gain_correction * PC_KI); // IntSpdErr  = PitCom(1)/( GK*PC_KI )          ! This will ensure that the pitch angle is unchanged if the initial SpdErr is zero

    last_time = curr_time; // LastTime   = Time                            ! This will ensure that generator speed filter will use the initial value of the generator speed on the first pass
    last_time_pc = curr_time - PC_DT; // LastTimePC = Time - PC_DT                    ! This will ensure that the pitch  controller is called on the first pass
    last_time_vs = curr_time - VS_DT; // LastTimeVS = Time - VS_DT                    ! This will ensure that the torque controller is called on the first pass

  } //ENDIF

  // ! Main control calculations:

  if (i_status >= 0 && *aviFAIL >= 0 ) { // IF ( ( iStatus >= 0 ) .AND. ( aviFAIL >= 0 ) )  THEN  ! Only compute control calculations if no error has occured and we are not on the last time step
    // ! Abort if the user has not requested a pitch angle actuator (See Appendix A
    // !   of Bladed User's Guide):
    if ( (int)(avrSWAP[10] + 0.5) != 0 ) { // IF ( NINT(avrSWAP(10)) /= 0 )  THEN ! .TRUE. if a pitch angle actuator hasn't been requested
      *aviFAIL = -1;
      err_msg = "Pitch angle actuator not requested"; // ErrMsg  = 'Pitch angle actuator not requested.'
    } // ENDIF

    // ! Set unused outputs to zero (See Appendix A of Bladed User's Guide):

    avrSWAP[35] = 0; // avrSWAP(36) = 0.0 ! Shaft brake status: 0=off
    avrSWAP[40] = 0; // avrSWAP(41) = 0.0 ! Demanded yaw actuator torque
    avrSWAP[45] = 0; // avrSWAP(46) = 0.0 ! Demanded pitch rate (Collective pitch)
    avrSWAP[47] = 0; // avrSWAP(48) = 0.0 ! Demanded nacelle yaw rate
    avrSWAP[64] = 0; // avrSWAP(65) = 0.0 ! Number of variables returned for logging
    avrSWAP[71] = 0; // avrSWAP(72) = 0.0 ! Generator start-up resistance
    avrSWAP[78] = 0; // avrSWAP(79) = 0.0 ! Request for loads: 0=none
    avrSWAP[79] = 0; // avrSWAP(80) = 0.0 ! Variable slip current status
    avrSWAP[80] = 0; // avrSWAP(81) = 0.0 ! Variable slip current demand

    // !=======================================================================
    // ! Filter the HSS (generator) speed measurement:
    // ! NOTE: This is a very simple recursive, single-pole, low-pass filter with
    // !       exponential smoothing.

    // ! Update the coefficient in the recursive formula based on the elapsed time
    // !   since the last call to the controller:
    alpha = exp((last_time - curr_time) * CORNER_FREQ); // Alpha     = EXP( ( LastTime - Time )*CornerFreq )

    // ! Apply the filter:
    gen_speed_f = (1.0 - alpha) * gen_speed + alpha * gen_speed_f; // GenSpeedF = ( 1.0 - Alpha )*GenSpeed + Alpha*GenSpeedF
    // !=======================================================================

    // ! Variable-speed torque control:

    // ! Compute the elapsed time since the last call to the controller:
    elap_time = curr_time - last_time_vs; // ElapTime = Time - LastTimeVS

    // ! Only perform the control calculations if the elapsed time is greater than
    // !   or equal to the communication interval of the torque controller:
    // ! NOTE: Time is scaled by OnePlusEps to ensure that the contoller is called
    // !       at every time step when VS_DT = DT, even in the presence of
    // !       numerical precision errors.
    if (curr_time * ONE_PLUS_EPS - last_time_vs >= VS_DT) { // IF ( ( Time*OnePlusEps - LastTimeVS ) >= VS_DT )  THEN
      // ! Compute the generator torque, which depends on which region we are in:
      if (gen_speed_f >= VS_RTGNSP || pit_com[0] >= VS_RGN3MP) { // IF ( (   GenSpeedF >= VS_RtGnSp ) .OR. (  PitCom(1) >= VS_Rgn3MP ) )  THEN ! We are in region 3 - power is constant
        gen_trq = VS_RTPWR / gen_speed_f; // GenTrq = VS_RtPwr/GenSpeedF
      } else if (gen_speed_f <= VS_CTINSP) { // ELSEIF ( GenSpeedF <= VS_CtInSp )  THEN                                    ! We are in region 1 - torque is zero
        gen_trq = 0.0f; // GenTrq = 0.0
      } else if (gen_speed_f < VS_RGN2SP) { // ELSEIF ( GenSpeedF <  VS_Rgn2Sp )  THEN                                    ! We are in region 1 1/2 - linear ramp in torque from zero to optimal
        gen_trq = vs_slope15 * (gen_speed_f - VS_CTINSP); // GenTrq = VS_Slope15*( GenSpeedF - VS_CtInSp )
      } else if (gen_speed_f < vs_tr_gn_sp) { // ELSEIF ( GenSpeedF <  VS_TrGnSp )  THEN                                    ! We are in region 2 - optimal torque is proportional to the square of the generator speed
        gen_trq = VS_RGN2K * gen_speed_f * gen_speed_f; // GenTrq = VS_Rgn2K*GenSpeedF*GenSpeedF
      } else { // ELSE                                                                       ! We are in region 2 1/2 - simple induction generator transition region
        gen_trq = VS_Slope25 * (gen_speed_f - vs_sy_sp); // GenTrq = VS_Slope25*( GenSpeedF - VS_SySp   )
      } // ENDIF

      // ! Saturate the commanded torque using the maximum torque limit:
      gen_trq = MIN(gen_trq, VS_MAX_TQ); // GenTrq  = MIN( GenTrq                    , VS_MaxTq  )   ! Saturate the command using the maximum torque limit

      // ! Saturate the commanded torque using the torque rate limit:
      if (i_status == 0) { last_gen_torque = gen_trq; } // IF ( iStatus == 0 )  LastGenTrq = GenTrq                 ! Initialize the value of LastGenTrq on the first pass only
      trq_rate = (gen_trq - last_gen_torque) / elap_time; // TrqRate = ( GenTrq - LastGenTrq )/ElapTime               ! Torque rate (unsaturated)
      trq_rate = MIN(MAX(trq_rate, -VS_MAX_RAT), VS_MAX_RAT)// TrqRate = MIN( MAX( TrqRate, -VS_MaxRat ), VS_MaxRat )   ! Saturate the torque rate using its maximum absolute value
      gen_trq = last_gen_torque + trq_rate * elap_time; // GenTrq  = LastGenTrq + TrqRate*ElapTime                  ! Saturate the command using the torque rate limit

      // ! Reset the values of LastTimeVS and LastGenTrq to the current values:
      last_time_vs = curr_time; // LastTimeVS = Time
      last_gen_torque = gen_trq; // LastGenTrq = GenTrq
    } // ENDIF

    // ! Set the generator contactor status, avrSWAP(35), to main (high speed)
    // !   variable-speed generator, the torque override to yes, and command the
    // !   generator torque (See Appendix A of Bladed User's Guide):

    avrSWAP[34] = 1; //  avrSWAP(35) = 1.0          ! Generator contactor status: 1=main (high speed) variable-speed generator
    avrSWAP[55] = 0; //  avrSWAP(56) = 0.0          ! Torque override: 0=yes
    avrSWAP[46] = last_gen_torque; //  avrSWAP(47) = LastGenTrq   ! Demanded generator torque


    // !=======================================================================
    // ! Pitch control:

    // ! Compute the elapsed time since the last call to the controller:
    elap_time = curr_time - last_time_pc; // ElapTime = Time - LastTimePC

    // ! Only perform the control calculations if the elapsed time is greater than
    // !   or equal to the communication interval of the pitch controller:
    // ! NOTE: Time is scaled by OnePlusEps to ensure that the contoller is called
    // !       at every time step when PC_DT = DT, even in the presence of
    // !       numerical precision errors.

    if (curr_time * ONE_PLUS_EPS - last_time_pc >= PC_DT) { // IF ( ( Time*OnePlusEps - LastTimePC ) >= PC_DT )  THEN
      // ! Compute the gain scheduling correction factor based on the previously
      // !   commanded pitch angle for blade 1:
      gain_correction = 1.0f / (1.0 + pit_com[0] / PC_KK); // GK = 1.0/( 1.0 + PitCom(1)/PC_KK )

      // ! Compute the current speed error and its integral w.r.t. time; saturate the
      // !   integral term using the pitch angle limits:
      spd_err = gen_speed_f - PC_REF_SPEED; // SpdErr    = GenSpeedF - PC_RefSpd                                 ! Current speed error
      int_spd_err += spd_err * elap_time; // IntSpdErr = IntSpdErr + SpdErr*ElapTime                           ! Current integral of speed error w.r.t. time
      int_spd_err = MIN(MAX(int_spd_err, PC_MIN_PIT / (gain_correction * PC_KI)), PC_MAX_PIT / (gain_correction * PC_KI)); // IntSpdErr = MIN( MAX( IntSpdErr, PC_MinPit/( GK*PC_KI ) ), PC_MaxPit/( GK*PC_KI )      )    ! Saturate the integral term using the pitch angle limits, converted to integral speed error limits

      // ! Compute the pitch commands associated with the proportional and integral
      // !   gains:
      pit_com_p = gain_correction * PC_KP * spd_err; // PitComP   = GK*PC_KP*   SpdErr                                    ! Proportional term
      pit_com_i = gain_correction * PC_KI * int_spd_err; // PitComI   = GK*PC_KI*IntSpdErr                                    ! Integral term (saturated)

      // ! Superimpose the individual commands to get the total pitch command;
      // !   saturate the overall command using the pitch angle limits:
      pit_com_total = pit_com_p + pit_com_i; // PitComT   = PitComP + PitComI                                     ! Overall command (unsaturated)
      pit_com_total = MIN(MAX(pit_com_total, PC_MIN_PIT), PC_MAX_PIT); // PitComT   = MIN( MAX( PitComT, PC_MinPit ), PC_MaxPit )           ! Saturate the overall command using the pitch angle limits

      // ! Saturate the overall commanded pitch using the pitch rate limit:
      // ! NOTE: Since the current pitch angle may be different for each blade
      // !       (depending on the type of actuator implemented in the structural
      // !       dynamics model), this pitch rate limit calculation and the
      // !       resulting overall pitch angle command may be different for each
      // !       blade.

      for (int k = 0; k < num_bl; k++) { // DO K = 1,NumBl ! Loop through all blades

           pit_rate[k] = (pit_com_total - bl_pitch[k]) / elap_time; // PitRate(K) = ( PitComT - BlPitch(K) )/ElapTime                 ! Pitch rate of blade K (unsaturated)
           pit_rate[k] = MIN(MAX(pit_rate[k], -PC_MAX_RAT), PC_MAX_RAT); // PitRate(K) = MIN( MAX( PitRate(K), -PC_MaxRat ), PC_MaxRat )   ! Saturate the pitch rate of blade K using its maximum absolute value
           pit_com[k] = bl_pitch[k] + pit_rate[k] * elap_time; // PitCom (K) = BlPitch(K) + PitRate(K)*ElapTime                  ! Saturate the overall command of blade K using the pitch rate limit
           pit_com[k] = MIN(MAX(pit_com[k], PC_MIN_PIT), PC_MAX_PIT); // PitCom(K)  = MIN( MAX( PitCom(K), PC_MinPit ), PC_MaxPit )     ! Saturate the overall command using the pitch angle limits

      } //  ENDDO          ! K - all blades

     // ! Reset the value of LastTimePC to the current value:
     last_time_pc = curr_time; // LastTimePC = Time

     // TODO?: ! Output debugging information if requested:
    /*    IF ( PC_DbgOut )  THEN
                          WRITE (UnDb,FmtDat)  Time, ElapTime, HorWindV, GenSpeed*RPS2RPM, GenSpeedF*RPS2RPM,           &
                                               100.0*SpdErr/PC_RefSpd, SpdErr, IntSpdErr, GK, PitComP*R2D, PitComI*R2D, &
                                               PitComT*R2D, PitRate*R2D, PitCom*R2D, BlPitch*R2D

        END IF*/
    } // ENDIF

    // ! Set the pitch override to yes and command the pitch demanded from the last
    // !   call to the controller (See Appendix A of Bladed User's Guide):

    avrSWAP[55] = 0; // avrSWAP(55) = 0.0       ! Pitch override: 0=yes

    avrSWAP[41] = pit_com[0]; // avrSWAP(42) = PitCom(1) ! Use the command angles of all blades if using individual pitch
    avrSWAP[42] = pit_com[1]; // avrSWAP(43) = PitCom(2) ! "
    avrSWAP[43] = pit_com[2]; // avrSWAP(44) = PitCom(3) ! "

    avrSWAP[44] = pit_com[0]; // avrSWAP(45) = PitCom(1) ! Use the command angle of blade 1 if using collective pitch

    // TODO: Debug info -->    IF ( PC_DbgOut )  WRITE (UnDb2,FmtDat) Time, avrSWAP(1:85)

    // !=======================================================================

    // ! Reset the value of LastTime to the current value:
    last_time = curr_time; // LastTime = Time

  } else if (i_status == -8) { // ELSEIF ( iStatus == -8 )  THEN
     // ! pack
     FILE *fpout = fopen(accINFILE, "w+"); // OPEN( Un, FILE=TRIM( InFile ), STATUS='UNKNOWN', FORM='UNFORMATTED' , ACCESS='STREAM', IOSTAT=ErrStat, ACTION='WRITE' )

     if (!fpout) { // IF ( ErrStat /= 0 ) THEN
        err_msg = "Cannot open file"; // ErrMsg  = 'Cannot open file "'//TRIM( InFile )//'". Another program may have locked it for writing.'
        *aviFAIL = -1; // aviFAIL = -1
     } else { // ELSE

        // ! write all static variables to the checkpoint file (inverse of unpack):
        fwrite( &gen_speed_f, 1, sizeof(gen_speed_f), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) GenSpeedF               ! Filtered HSS (generator) speed, rad/s.
        fwrite( &int_spd_err, 1, sizeof(int_spd_err), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) IntSpdErr               ! Current integral of speed error w.r.t. time, rad.
        fwrite( &last_gen_torque, 1, sizeof(last_gen_torque), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) LastGenTrq              ! Commanded electrical generator torque the last time the controller was called, N-m.
        fwrite( &last_time, 1, sizeof(last_time), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) LastTime                ! Last time this DLL was called, sec.
        fwrite( &last_time_pc, 1, sizeof(last_time_pc), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) LastTimePC              ! Last time the pitch  controller was called, sec.
        fwrite( &last_time_vs, 1, sizeof(last_time_vs), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) LastTimeVS              ! Last time the torque controller was called, sec.
        fwrite( pit_com, 3, sizeof(pit_com[0]), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) PitCom                  ! Commanded pitch of each blade the last time the controller was called, rad.
        fwrite( &vs_slope15, 1, sizeof(vs_slope15), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) VS_Slope15              ! Torque/speed slope of region 1 1/2 cut-in torque ramp , N-m/(rad/s).
        fwrite( &vs_slope25, 1, sizeof(vs_slope25), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) VS_Slope25              ! Torque/speed slope of region 2 1/2 induction generator, N-m/(rad/s).
        fwrite( &vs_sy_sp, 1, sizeof(vs_sy_sp), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) VS_SySp                 ! Synchronous speed of region 2 1/2 induction generator, rad/s.
        fwrite( &vs_tr_gn_sp, 1, sizeof(vs_tr_gn_sp), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) VS_TrGnSp               ! Transitional generator speed (HSS side) between regions 2 and 2 1/2, rad/s.

        fclose(fpout); // CLOSE ( Un )

    } // END IF

  } else if (i_status == -9) { // ELSEIF( iStatus == -9 ) THEN
     // !unpack
     FILE *fpout = fopen(accINFILE, "r+"); // OPEN( Un, FILE=TRIM( InFile ), STATUS='OLD', FORM='UNFORMATTED', ACCESS='STREAM', IOSTAT=ErrStat, ACTION='READ' )

     if (!fpout) { // IF ( ErrStat /= 0 ) THEN
        aviFAIL = -1
        ErrMsg  = ' Cannot open file "'//TRIM( InFile )//'" for reading. Another program may have locked.'
     } else { // ELSE

        // ! READ all static variables from the restart file (inverse of pack):
        fread( &int_spd_err, 1, sizeof(int_spd_err), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) IntSpdErr               ! Current integral of speed error w.r.t. time, rad.
        fread( &last_gen_torque, 1, sizeof(last_gen_torque), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) LastGenTrq              ! Commanded electrical generator torque the last time the controller was called, N-m.
        fread( &last_time, 1, sizeof(last_time), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) LastTime                ! Last time this DLL was called, sec.
        fread( &last_time_pc, 1, sizeof(last_time_pc), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) LastTimePC              ! Last time the pitch  controller was called, sec.
        fread( &last_time_vs, 1, sizeof(last_time_vs), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) LastTimeVS              ! Last time the torque controller was called, sec.
        fread( pit_com, 3, sizeof(pit_com[0]), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) PitCom                  ! Commanded pitch of each blade the last time the controller was called, rad.
        fread( &vs_slope15, 1, sizeof(vs_slope15), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) VS_Slope15              ! Torque/speed slope of region 1 1/2 cut-in torque ramp , N-m/(rad/s).
        fread( &vs_slope25, 1, sizeof(vs_slope25), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) VS_Slope25              ! Torque/speed slope of region 2 1/2 induction generator, N-m/(rad/s).
        fread( &vs_sy_sp, 1, sizeof(vs_sy_sp), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) VS_SySp                 ! Synchronous speed of region 2 1/2 induction generator, rad/s.
        fread( &vs_tr_gn_sp, 1, sizeof(vs_tr_gn_sp), fpout ) ; // WRITE( Un, IOSTAT=ErrStat ) VS_TrGnSp               ! Transitional generator speed (HSS side) between regions 2 and 2 1/2, rad/s.

        fclose(fpout); // CLOSE ( Un )
     } // END IF


  } // ENDIF

  *avcMSG = err_msg; // avcMSG = TRANSFER( TRIM(ErrMsg)//C_NULL_CHAR, avcMSG, SIZE(avcMSG) )

  // RETURN
  // END SUBROUTINE DISCON
  // !=======================================================================

}
