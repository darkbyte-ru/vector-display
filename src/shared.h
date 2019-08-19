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
extern volatile MaxData maxData;

extern volatile unsigned long lastDataSeen;
extern volatile float ahDelta;
extern volatile bool gotData;
extern volatile unsigned long _uart_rx_count;

#endif