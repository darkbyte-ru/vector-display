#ifndef vector_h
#define vector_h 

#include <Arduino.h>
#include <Preferences.h> 

        const byte SEND_V_MIN_Pre_OFF = 1;
        const byte SEND_Kp = 2;
        const byte SEND_Kd = 3;
        const byte SEND_Ki = 4;
        const byte SEND_KpPos = 5;
        const byte SEND_KdPos = 6;
        const byte SEND_KiPos = 7;
        const byte SEND_StrtPOS = 8;
        const byte SEND_SensorlessCurrUst = 9;
        const byte SEND_CntUDPTrmConst = 10;
        const byte SEND_AngleLimit = 11;
        const byte SEND_SenosrlessPWM1_1Const = 12;
        const byte SEND_KGYRO = 13;
        const byte SEND_GetOptions = 14;
        const byte SEND_ProgrammOptions = 15;
        const byte SEND_SensorlessCurrLimit = 16;
        const byte SEND_V_MIN_Sound = 17;
        const byte SEND_KpKpSpd = 18;
        const byte SEND_V_min_OverV = 19;
        const byte SEND_NotAll = 20;
        const byte SEND_Clear = 21;
        const byte SEND_PWM1 = 22;
        const byte SEND_MAX_PWM_MEM = 23;
        const byte SEND_AutoPIDOn = 24;
        const byte SEND_AutoPIDOff = 25;
        //const byte SEND_CalibrGyro = 26;
        const byte SEND_CalibrAcc = 27;
        const byte SEND_KRot = 28;
        const byte SEND_But1Func = 29;
        const byte SEND_PhasePERMASS = 30;
        const byte SEND_HallDelay1_F = 31;
        const byte SEND_HallDelay1_B = 32;
        const byte SEND_But2Func = 33;
        const byte SEND_CurrLimit = 34;
        const byte SEND_SpeedLimit1 = 35;
        const byte SEND_DirectMotor = 36;
        const byte SEND_NotDirectMotor = 37;
        const byte SEND_CalibrGyro = 38;
        const byte SEND_Theta1Zero = 39;
        const byte SEND_UBatLow = 40;
        const byte SEND_SbrosOdometra = 41;
        const byte SEND_Beep_10m = 42;
        const byte SEND_No_Beep_10m = 43;
        const byte SEND_Theta2Zero = 44;
        const byte SEND_VectorOn = 45;
        const byte SEND_VectorOff = 46;
        const byte SEND_SpeedLimit3 = 47;
        const byte SEND_NumSpeedLimit = 48;
        const byte SEND_GetErrors = 49;
        const byte SEND_AnalogBreakOn = 50;
        const byte SEND_KpMax = 51;
        const byte SEND_V_MIN_OFF = 52;
        const byte SEND_SpdKpMax = 53;
        const byte SEND_GetDataPacket = 54;
        const byte SEND_AnalogBreakOff = 55;
        const byte SEND_But3Func = 56;
        const byte SEND_But4Func = 57;
        const byte SEND_MAX_PWM_Revers = 58;
        const byte SEND_MaxSpdRevers = 59;
        const byte SEND_ClearErrors = 60;
        const byte SEND_WriteErrors = 61;
        const byte SEND_KpStp = 62;
        const byte SEND_TemperatureMaxOUT_H = 63;
        const byte SEND_TiltZadAddNoMan = 64;
        const byte SEND_KpNoManOneLeg = 65;
        const byte SEND_KiNoManOneLeg = 66;
        const byte SEND_KpTemperOUT = 67;
        const byte SEND_TiltZadAddNoManOneLeg = 68;
        const byte SEND_AutoPID_On_CntConst = 69;
        const byte SEND_UBatHi = 70;
        const byte SEND_KiTemperIN = 71;

        const byte SEND_KiTemperOUT = 72;
        const byte SEND_CopyProfiles = 73;
        const byte SEND_SensorTSTKeys = 74;
        const byte SEND_TiltZadAddOneTwoLeg = 75;
        const byte SEND_ZeroThroottleOn = 76;
        const byte SEND_KpRotOneTwoLeg = 77;
        const byte SEND_KiRotOneTwoLeg = 78;

        const byte SEND_PreasUpCntConst = 79;
        const byte SEND_PreasDnCntConst = 80;
        const byte SEND_ZeroThroottleOff = 81;
        const byte SEND_AutoPID_On_PWM = 82;
        const byte SEND_AutoPID_On_Speed = 83;

        const byte SEND_StrongModePIDOn = 84;
        const byte SEND_StrongModePIDOff = 85;
        const byte SEND_StartRotFreshmanL = 86;
        const byte SEND_TiltZadAddFreshman = 87;
        const byte SEND_KRotFreshmanL = 88;
        const byte SEND_SmoothBreakOn = 89;
        const byte SEND_SmoothBreakOff = 90;
        const byte SEND_CurrUstSpd0 = 91;
        const byte SEND_CurrPhaseLimitSpd0 = 92;
        const byte SEND_CurrLimitSpd0 = 93;
        const byte SEND_MaxAngleStopBreak = 94;
        const byte SEND_SaveOptions1 = 95;
        const byte SEND_SaveOptionsProfile = 96;
        const byte SEND_DefaultOptions = 97;
        const byte SEND_BeepPauseConstAlm = 98;
        const byte SEND_BzOnCntSeriaConstAlm = 99;
        const byte SEND_CntSingleBeepAlm = 100;

        const byte SEND_DiffAlfaConstL = 101;
        const byte SEND_DiffAlfaConstH = 102;
        const byte SEND_Robowell = 103;
        const byte SEND_RobowellSpd = 104;
        const byte SEND_GetRobowell = 105;
        const byte SEND_GetRobowellSpd = 106;
        const byte SEND_CalibrCurr = 107;
        const byte SEND_ClearCurrentAH = 108;
        const byte SEND__3psnSwitchBtNotPressed = 109;
        const byte SEND__3psnSwitchBt3 = 110;
        const byte SEND__3psnSwitchBt4 = 111;
        const byte SEND_Options1 = 112;
        const byte SEND_TrmRqst = 113;
        const byte SEND_NoAutoTrmOn = 114;
        const byte SEND_NoAutoTrmOff = 115;
        const byte SEND_SetTmr1 = 116;
        const byte SEND_SetTmr2 = 117;
        const byte SEND_UseTmr = 118;
        const byte SEND_TmrLoudOn = 119;
        const byte SEND_TmrLoudOff = 120;
        const byte SEND_SerBynber = 121;

        //const byte SEND_But5Func = 122;
        const byte SEND_KiOneTwoLeg_1 = 123;
        const byte SEND_StartRotOneTwoLeg_1 = 124;
        const byte SEND_SensorlessCntAlignConst = 125;
        const byte SEND_KRotOneTwoLeg_1 = 126;
        const byte SEND_KTemperatureSPDControl = 127;
        const byte SEND_TemperatureMaxIN_H = 128;
        const byte SEND_SensorlessSpdChngCntConst = 129;
        const byte SEND_SensorLessTimerConst2 = 130;
        const byte SEND_SenosrlessPWM1_2Const = 131;
        const byte SEND_SensorLessTimerConstMin = 132;
        const byte SEND_SensorllessSpdThreshold = 133;
        const byte SEND_Temperature3On = 134;
        const byte SEND_Temperature3Off = 135;
        const byte SEND_Temperature4On = 136;
        const byte SEND_Temperature4Off = 137;
        const byte SEND_Temperature5On = 138;
        const byte SEND_Temperature5Off = 139;
        const byte SEND_Temperature6On = 140;
        const byte SEND_Temperature6Off = 141;
        const byte SEND_TemperatureMaxOUT = 142;
        const byte SEND_TemperatureMaxIN = 143;
        const byte SEND_TemperatureTYPE = 144;
        const byte SEND_KpTemperIN = 145;
        const byte SEND_Profile = 146;
        const byte SEND_Key10_On = 147;
        const byte SEND_Key10_Off = 148;
        const byte SEND_Key11_On = 149;
        const byte SEND_Key11_Off = 150;
        const byte SEND_Key12_On = 151;
        const byte SEND_Key12_Off = 152;
        const byte SEND_Key13_On = 153;
        const byte SEND_Key13_Off = 154;
        const byte SEND_Key14_On = 155;
        const byte SEND_Key14_Off = 156;
        const byte SEND_Key15_On = 157;
        const byte SEND_Key15_Off = 158;
        const byte SEND_Key16_On = 159;
        const byte SEND_Key16_Off = 160;
        const byte SEND_Options2 = 161;
        const byte SEND_BreakThresholdDn = 162;
        const byte SEND_MAH = 163;
        const byte SEND_SensorThreshold = 164;
        const byte SEND_ManualStartOn = 165;
        const byte SEND_ManualStartOff = 166;
        const byte SEND_Balance2 = 167;
        const byte SEND_StopBalance2 = 168;
        const byte SEND_Balance3 = 169;
        const byte SEND_StopBalance3 = 170;
        const byte SEND_Balance4 = 171;
        const byte SEND_StopBalance4 = 172;
        const byte SEND_TiltZadState5 = 173;
        const byte SEND_SpdRstSpeedLim = 174;
        const byte SEND_PWMControlOn = 175;
        const byte SEND_SpeedControlOn = 176;
        const byte SEND_XLimit = 177;
        const byte SEND_YLimit = 178;
        const byte SEND_BreakThresholdUp = 179;
        const byte SEND_MonocycleOn = 180;
        const byte SEND_MonocycleOff = 181;
        const byte SEND_BreakButtonMonocycleOn = 182;
        const byte SEND_SpeedLimitSt4 = 183;
        const byte SEND_ResetDistance = 184;
        const byte SEND_TestVibroLROn = 185;
        const byte SEND_TestVibroLROff = 186;
        const byte SEND_StopVibroOn = 187;
        const byte SEND_StopVibroOff = 188;
        const byte SEND_BreakButtonMonocycleOff = 189;
        const byte SEND_MonoStopDirectionOn = 190;
        const byte SEND_DirPWM1_0 = 191;
        const byte SEND_DirPWM1_1 = 192;
        const byte SEND_MaxSpdPID = 193;
        const byte SEND_DirBalance1 = 194;
        const byte SEND_DirBalance0 = 195;
        //        const byte SEND_ResetBMS_RAM = 196;
        const byte SEND_MonoStopDirectionOff = 197;
        const byte SEND_AlignmentON = 198;
        const byte BuzzAndHornOn = 199;
        const byte BuzzAndHornOff = 200;
        const byte DoNotTestBMSOn = 201;
        const byte DoNotTestBMSOff = 202;
        const byte SEND_GetOptionsSteer = 203;
        const byte SEND_KGyroSteer = 204;
        const byte SEND_WriteEEPROMSteer = 205;
        const byte SEND_CalibrGyroSteer = 206;
        const byte SEND_AlignmentOff = 207;
        const byte SEND_ThreePositionSwitchOn = 208;
        const byte SEND_ThreePositionSwitchOff = 209;
        const byte SEND_KdOneTwoLeg = 210;
        const byte SEND_KdOneTwoLeg_1 = 211;
        const byte SEND_KdFreshman = 212;
        const byte SEND_Num_KdAverage = 213;
        const byte SEND_SensorThresholdUp = 214;
        const byte SEND_BreakInvBtnOn = 215;
        const byte SEND_BreakInvBtnOff = 216;
        const byte SEND_StrongModeOn = 217;
        const byte SEND_NormalMode = 218;
        const byte SEND_BreakThreshold = 219;
        const byte SEND_KFirstBreak = 220;
        const byte SEND_SensOrder2 = 221;
        const byte SEND_SensOrder1 = 222;
        const byte SEND_Balance5 = 223;
        const byte SEND_StopBalance5 = 224;
        const byte SEND_SensorLessTimerConst = 225;

        const byte SEND_SensorlessON = 226;
        const byte SEND_SensorlessOff = 227;
        const byte SEND_SignalizaciaON = 228;
        const byte SEND_SignalizaciaOFF = 229;
        const byte SEND_FixPeriod = 230;
        const byte SEND_KpCurr = 231;
        const byte SEND_VectorTrapezoidaOn = 232;
        const byte SEND_VectorTrapezoidaOff = 233;
        const byte SEND_CurrLimII_On = 234;
        const byte SEND_CurrLimII_Off = 235;
        const byte SEND_KiCurr = 236;
        const byte SEND_CurrUst = 237;
        const byte SEND_ClbrHalls = 238;
        const byte SEND_PWM32BitsOn = 239;
        const byte SEND_PWM32BitsOff = 240;
        const byte SEND_Sensor2Threshold = 241;
        const byte SEND_Sensor2ThresholdUp = 242;
        const byte SEND_Unlock = 243;
        const byte SEND_GetOptionsWhenLock = 244;
        const byte SEND_lock = 245;
        const byte SEND_GetPhase = 247;
        const byte SEND_GetNeutral = 248;
        const byte SEND_MixedMode = 249;
        const byte SEND_CntMixedModeMax = 250;
        const byte SEND_MixedModeLevelOn = 251;
        const byte SEND_CurrMixed = 252;
        const byte SEND_MixedModeLevelOff = 253;
        const byte SEND_SecondCMD = 254;


        const byte SEND_SecondCMD_RotAlfaXResOn = 1;
        const byte SEND_SecondCMD_RotAlfaXResOff = 2;
        const byte SEND_TurnSignalOn = 3;
        const byte SEND_LightsTudaSudaOn = 4;
        const byte SEND_LightsTudaSudaOff = 5;
        const byte SEND_LightsStopMonoDirOn = 6;
        const byte SEND_LightsStopMonoDirOff = 7;
        const byte SEND_GyroZFlTurn = 8;
        const byte SEND_ThrottleTmpChngMem = 9;
        const byte SEND_SlowStartOn = 10;
        const byte SEND_SlowStartOff = 11;
        const byte SEND_AntiPoliceOn = 12;
        const byte SEND_AntiPoliceOff = 13;
        const byte SEND_SlowStrtSpd = 14;
        const byte SEND_AntiPolicePower = 15;
        const byte SEND_MixedModeSlowSpeedOffOn = 16;
        const byte SEND_MixedModeSlowSpeedOffOff = 17;
        const byte SEND_MixedModeSlowSpeed = 18;
        const byte SEND_MixedMode2 = 19;
        const byte SEND_Ki_PWM_Mode = 20;
        const byte SEND_PWM_ModeOn = 21;
        const byte SEND_PWM_ModeOff = 22;
        const byte SEND_PWM_Mode_ZeroThrottleOn = 23;
        const byte SEND_PWM_Mode_ZeroThrottleOff = 24;
        const byte SEND_Kp_PWM_Mode = 25;
        const byte SEND_SIN_MIDDLEOn = 26;
        const byte SEND_SIN_MIDDLEOff = 27;
        const byte SEND_InvSinOn = 28;
        const byte SEND_InvSinOff = 29;
        const byte SEND_CurrLimitSoundOn = 30;
        const byte SEND_CurrLimitSoundOff = 31;
        const byte SEND_CurrPhaseLimit = 32;
        const byte SEND_CurrSensor = 33;
        const byte SEND_DecreaseCurrentOn = 34;
        const byte SEND_DecreaseCurrentOff = 35;
        const byte SEND_Spd0TimerConst = 36;
        const byte SEND_FillCurrentMassOn = 37;
        const byte SEND_FillSinMassOn = 38;
        const byte SEND_FillMassOff = 39;
        const byte SEND_CW_CCW_AnyProfileOn = 40;
        const byte SEND_CW_CCW_AnyProfileOff = 41;
        const byte SEND_BuzzerOnOff = 42;
        const byte SEND_AutoNastroyka = 43;
        const byte SEND_AutoNastroyka_ConfirmMSG = 44;
        const byte SEND_SensorThresholdUp_OK = 45;
        const byte SEND_SensorThresholdUp_nOK = 46;
        const byte SEND_BreakThresholdUp_OK = 47;
        const byte SEND_BreakThresholdUp_nOK = 48;
        const byte SEND_FinishOptionsTune = 49;
        const byte SEND_StopAutoNastroyka = 50;
        const byte SEND_AutoNastroykaOK = 51;
        const byte SEND_HallDelay1MaxSPD_F = 52;
        const byte SEND_HallDelay1MaxSPD_B = 53;
        const byte SEND_HallDelayMaxSPD = 54;
        const byte SEND_KZPhasesOn = 55;
        const byte SEND_Mgnovenniy = 56;
        const byte SEND_KZ_Mgnovenniy_Off = 57;
        const byte SEND_PINCODE = 58;
        const byte SEND_BothBreaksOn = 59;
        const byte SEND_BothBreaksOff = 60;
        const byte SEND_GAZ_IMMITATOROn = 61;
        const byte SEND_GAZ_IMMITATOROff = 62;
        const byte SEND_GazImm = 63;
        const byte SEND_PINCODE_Unlock = 64;
        const byte SEND_RESET_PASSWORD = 65;
        const byte SEND_ChagerViaMotorOn = 66;
        const byte SEND_ChagerViaMotorOff = 67;
        const byte SEND_TrapezoidaWhenBigCurrentOn = 68;
        const byte SEND_TrapezoidaWhenBigCurrentOff = 69;
        const byte SEND_BigCurrent = 70;
        const byte SEND_ChagerCurr = 71;
        const byte SEND_P_Curr = 72;
        const byte SEND_I_Curr = 73;
        const byte SEND_P_U = 74;
        const byte SEND_I_U = 75;
        const byte SEND_X1 = 76;
        const byte SEND_2WDMaster = 77;
        const byte SEND_2WDSlave = 78;
        const byte SEND_JOYSTICKOn = 79;
        const byte SEND_JOYSTICKOff = 80;
        const byte SEND_JOYSTICKCalibrOn = 81;
        const byte SEND_JOYSTICKCalibrOff = 82;
        const byte SEND_CalibrateJOYSTICK = 83;
        const byte SEND_JoistickDeadZone = 84;
        const byte SEND_CHANGE_AXISOn = 85;
        const byte SEND_CHANGE_AXISOff = 86;
        const byte SEND_KpKp = 87;
        const byte SEND_IRFP4110On = 88;
        const byte SEND_IRFP4110Off = 89;
        const byte SEND_ThrottleLevelUpOff = 90;
        const byte SEND_LightMode = 91;
        const byte SEND_LightSensorThresholdLo = 92;
        const byte SEND_LightSensorThresholdHi = 93;
        const byte SEND_SaveOptionsFlashMass2 = 94;
        const byte SEND_Options3 = 95;
        const byte SEND_LightOnOn = 96;
        const byte SEND_LightOnOff = 97;
        const byte SEND_CurrentControlOnly = 98;
        const byte SEND_CurrentMaxCurrentOnly = 99;
        const byte SEND_K_PWM_Nastr = 100;
        const byte SEND_DEADTIME_Si8233 = 101;
        const byte SEND_OCP_DEG = 102;
        const byte SEND_VDS_LVL = 103;
        const byte SEND_CLR_FLT = 104;

        const byte SEND_Crr1Corr = 105;
        const byte SEND_ButtonBreakOnlyOn = 106;
        const byte SEND_ButtonBreakOnlyOff = 107;
        const byte SEND_ButtonBreakOnlyPWM = 108;
        const byte SEND_StpLightsOn = 109;
        const byte SEND_StpLightsOff = 110;
        const byte SEND_CntTurnOnConst = 111;
        const byte SEND_CntTurnOffConst = 112;

        const byte SEND_TemperatureTYPE_FETs = 113;
        const byte SEND_TEST_PWM_KEYSOn = 114;
        const byte SEND_TEST_PWM_KEYSOff = 115;
        const byte SEND_TstPhase = 116;
        const byte SEND_VerticalOn = 117;
        const byte SEND_VerticalOff = 118;
        const byte SEND_SpdStartLevel = 119;
        const byte SEND_Ki_PWM_ModeOff = 120;
        const byte SEND_StrongModeSpdFilterOffMem = 121;
        const byte SEND_StrongModeBreakOffOn = 122;
        const byte SEND_StrongModeMotorOffOn = 123;
        const byte SEND_StrongModeBreakOffModeMotorOffOff = 124;
        const byte SEND_RegenOff = 125;
        const byte SEND_KeyLightStopOn = 126;
        const byte SEND_KeyLightBrightness = 127;
        const byte SEND_KeyLightStopGabaritOn = 128;
        const byte SEND_MigStopOn = 129;
        const byte SEND_MigStopOff = 130;
        const byte SEND_AutoNastroyka_State30 = 131;
        const byte SEND_AutoNastroyka_State32 = 132;
        const byte SEND_AutoNastroyka_State27 = 133;
        const byte SEND_ButtonBreakOnlyTime = 134;
        const byte SEND_BreakMonocycleSpd = 135;
        const byte SEND_PWM10kHzOn = 136;
        const byte SEND_PWM10kHzOff = 137;




#endif