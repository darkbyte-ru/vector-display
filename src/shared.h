#ifndef shared_h
#define shared_h
#include <Arduino.h>
#include <Preferences.h>

#include <Wire.h>
#include <RTClib.h>

#include <Nextion.h>
#include <NextionTypes.h>
#include <NextionPage.h>
#include <NextionText.h>
#include <NextionButton.h>
#include <NextionDualStateButton.h>
#include <NextionProgressBar.h>

#include "FS.h"
#include "SPIFFS.h"

#include <settings.h>

#define FORMAT_SPIFFS_IF_FAILED true
#define NEXTION_PORT Serial2

typedef struct 
{
uint32_t CntSamples;
int16_t AlfaXRes, AlfaYRes, GyroX, TiltXRes;
int16_t LightSensor, Ep, Ei, PWM1, Curr1, Spd1USTKMH, Temperature3;
uint32_t Odometr;
int16_t Spd1Res, Temperature2, Temperature1, Kp, UBT, GyroVert_Steer;
int16_t StatFlags, GyroZFilter, AlfaYResPrevAv, DiffBetweenTilts;
uint32_t Flgs, Flgs1, Flgs2;
int16_t TiltZad, StartRot, Ki, KRot, KpRot, KiRot, Preas1ADC, Preas2ADC;
int8_t CurrLimitTek;
int16_t KpSPD, AngleLimitReal;
uint32_t CurrTuda1, CurrRegen1, Flgs4, Flgs5, Flgs3, TimerCnt;
int16_t _3VFl, _5VFl, _12VFl; //floats
int16_t CurrA_P, CurrA_N, CurrC_P, CurrC_N;
uint16_t V8, V9, V10, V11, V12, V13, V14, V15, V16, V17, V18;
uint32_t BatteryKeys;
int8_t ChagerKeys;
uint16_t FaultStatus1Res, VGSStatus2Res;
int16_t AccZ;
int8_t Sensor1_Prev, Sensor2_Prev;
int16_t Temperature4;
uint32_t Distance;
uint16_t ProtectValue;
int8_t Temperature; //if ((Temperature & 0x80) == 0x80)Temperature = (int)(((uint)Temperature) | 0xffffff00);
int8_t _v48V;
int8_t KAccGyroSteer;
int16_t GyroXFl, GyroYFl, GyroZFl, Ed, GyroYAv, KdI, MaxSpdKMH;
uint32_t PhasesCnt2;
int16_t Spd1Fl, Temperature5, Temperature6;
uint32_t Phase1Period1, Phase1Period2, EpCurr1, Phase1Period4, Phase1Period5, Phase1Period6;
int16_t CurrUstTek; //(int)(Tmp * (int)CurrPerDigit);
int8_t CriticalError;
int16_t Spd1UST_Tmp;
uint16_t ADCThrottleBreak, RCON_Mem, FirmwareVersion;
int8_t MPU6050Err;
int16_t PhaseCurr; //PhaseCr = (double)PhaseCurr * CurrPerDigit;
int16_t HallErrCnt;
int16_t CurrPhaseLimitTek; //*=CurrPerDigit
uint32_t PhasePer1;
int8_t Halls, HallDelay1_Tek_F, HallDelay1_Tek_B;
int16_t EPID_Res;
int8_t BuzzerOnEvent;
}__attribute__((packed)) packetType5;


typedef struct 
{
int16_t zeroPad;
int16_t KpMustBe, KpNoMan, KdMem, KiNoMan, KpKpSpd;
int8_t But1Func, But2Func;
int16_t KGYRO, SensorLessTimerConst2, SenosrlessPWM1_2Const, SensorLessTimerConstMin, CalibrationGyroX;
int16_t CalibrationGyroY, CalibrationGyroZ, CalibrationAccX, CalibrationAccY, CalibrationAccZ;
int8_t PhasePERMASS, PhasePERMASS_SHFT, HallDelay1_F, HallDelay1_B, But3Func, But4Func;
int8_t Theta1Zero, Theta2Zero;
int16_t CurrLimit, ChagerCurr, KpSPDReal, KdSPD, KiSPD, AngleLimitInt, SpeedLimit1;
int16_t StatFlags, XLimit, YLimit, UBatLow, CntMixedModeMax, MixedModeLevelOn;
int8_t SpeedLimit2, SpeedLimit3, I_Curr;
int16_t CurrMixed, KpMax;
int8_t SensorllessSpdThreshold, SpdKpMax;
int16_t MixedModeLevelOff, KpCurr, Ki_PWM_Mode;
int8_t P_U, I_U;
int16_t KpNoManOneLeg, KiNoManOneLeg, TiltZadAddNoManOneLeg, MAX_PWM_MEM, KiTemperIN;
int16_t KpTemperIN, KpTemperOUT, KiTemperOUT;
int8_t MAX_PWM_Revers, MaxSpdRevers, MixedModeSlowSpeed, HallDelay1MaxSPD_B;
int16_t StatFlgs3, KiRotOneTwoLeg, AutoPID_On_CntConst;
int8_t AutoPID_On_PWM, AutoPID_On_Speed;
int16_t Diametr1, PhasesPerRev1, UBatHi, BreakThresholdDn, BreakThresholdUp, KTemperatureSPDControl;
int8_t Profile, _3psnSwitchBtNotPressed, _3psnSwitchBt3, _3psnSwitchBt4;
int16_t CurrPhaseLimit;
int8_t HallDelay1MaxSPD_F, CurrSensor;
int16_t CurrUstSpd0, CurrPhaseLimitSpd0, CurrLimitSpd0;
int8_t MaxAngleStopBreak, ThrottleTmpChngMem;
int16_t BeepPauseConstAlm, BzOnCntSeriaConstAlm;
int8_t CntSingleBeepAlm, DiffAlfaConstL, DiffAlfaConstH;
int16_t ZeroCurr1;
int8_t But5Func, MaxSpdPID;
int16_t Timer1Mem, Timer2Mem;
int8_t UseTmr;
int16_t SerNumber, SensorlessCurrLimit;
int8_t SlowStrtSpd, AntiPolicePower;
int16_t SensorlessCurrUst, GyroZFlTurn, Kp_PWM_Mode, Spd0TimerConst;
int8_t HallDelayMaxSPD, _2WDMode;
int16_t MAH, SensorThreshold, TiltZadState5;
int8_t SpdRstSpeedLim;
int16_t SensorlessCntAlignConst, SenosrlessPWM1_1Const, StatFlags2, TemperatureMaxOUT;
int8_t SpeedLimitSt4;
int16_t StatFlags1, SensorLessTimerConst, TemperatureMaxIN, KdOneTwoLeg, BigCurrent;
int8_t P_Curr;
int16_t JoystickCalibrX;
int8_t KdAverage;
int16_t SensorThresholdUp, BreakThreshold, KFirstBreak;
int8_t TemperatureTYPE, SensOrder1;
int16_t SensorlessSpdChngCntConst, V_MIN_OFF, V_MIN_Pre_OFF, V_MIN_Sound, V_min_OverV, KiCurr, CurrUst;
int8_t KpKpMustBe;
int16_t ThrottleLevelUpOff, TemperatureMaxIN_H, TemperatureMaxOUT_H, JoystickCalibrY, JoistickDeadZone;
}__attribute__((packed)) packetType6;


struct CurrentData 
{
 float speed;
 float voltage;
 float current;
 float ah;
 float ahRegen;
 float tempCont;
 float tempEngine;
 float distance;
 float odometer;
 long flgs3;
 long flgs4;
 long flgs5;
 float ImpNa1M;
 bool unlocked = false;
 bool cc = false;
 bool cv = false;
 float volt3v3;
 float volt5v0;
 float volt12v0;
};
typedef CurrentData CurrentData;

struct MaxData 
{
 float speed;
 float current;
 float currentRegen;
 float tempCont;
 float tempEngine;
};
typedef MaxData MaxData;

static Nextion nex(NEXTION_PORT);
static RTC_DS3231 rtc;

extern Preferences cfg;

extern char VERSION[10];

extern Preferences cfg;

extern volatile CurrentData currentData;
extern volatile bool currentDataChanged;
extern volatile MaxData maxData;

extern volatile unsigned long lastDataSeen;
extern volatile float ahDelta;
extern volatile unsigned long _uart_rx_count;

#endif