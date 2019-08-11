#ifndef Main_h_INCLUDED
#define Main_h_INCLUDED

#include <WiFi.h>
#include <HardwareSerial.h>
#include <ArduinoOTA.h>

//#include <LedControl.h>

#include <shared.h>
#include <settings.h>
#include <bluetooth.h>
#include <web.h>

unsigned int voltageInROM;

//screens
NextionPage splash(nex, 0, 0, "splash");
NextionPage main (nex, 1, 0, "main");
NextionPage actions (nex, 3, 0, "actions");

//init screen
NextionText status(nex, 0, 2, "status");
NextionText version(nex, 0, 3, "version");

//main screen
NextionText speed(nex, 1, 13, "speed");
NextionText voltage(nex, 1, 9, "volts");
NextionText current(nex, 1, 10, "current");
NextionText ah(nex, 1, 3, "ah");
NextionText ahRegen(nex, 1, 4, "ahRegen");
NextionText tempCont(nex, 1, 17, "controllerTemp");
NextionText tempEngine(nex, 1, 16, "engineTemp");
NextionText distance(nex, 1, 18, "distance");
NextionText odometer(nex, 1, 19, "odometer");
NextionText power(nex, 1, 14, "watts");
NextionText dateLabel(nex, 1, 1, "date");
NextionText timeLabel(nex, 1, 2, "time");
NextionProgressBar ahBar(nex, 1, 11, "ahBar");

//stats screen
NextionText maxA(nex, 2, 11, "maxA");
NextionText maxARegen(nex, 2, 12, "maxARegen");
NextionText maxEngT(nex, 2, 13, "maxEngT");
NextionText maxContT(nex, 2, 14, "maxContT");
NextionText maxSpeed(nex, 2, 10, "maxSpeed");
NextionText wattKm(nex, 2, 15, "wattKm");
NextionText rideTime(nex, 2, 16, "rideTime");

//actions screen
NextionText chargeState(nex, 3, 5, "chargeState");
NextionText chargeCC(nex, 3, 6, "cc");
NextionText chargeCV(nex, 3, 7, "cv");
NextionDualStateButton chargeModeButton(nex, 3, 8, "chargeMode");
NextionDualStateButton resetAhButton(nex, 3, 9, "resetAh");
NextionDualStateButton resetDistanceButton(nex, 3, 10, "resetDistance");
NextionDualStateButton unlockButton(nex, 3, 11, "unlockBtn");

boolean initialized = false;
boolean firstRun = true;
boolean regen = false;
boolean chargeMode = false;
boolean stopSendRequests = false;
boolean spiffsMounted = true;

long DAY_MS = 86400000; // 86400000 milliseconds in a day
long HOUR_MS = 3600000; // 3600000 milliseconds in an hour
long MINUTE_MS = 60000; // 60000 milliseconds in a minute

byte prevPageWas = 0;

void checkButtons(void);

void showDate();
void setupScreen(void);
void setupNoDataScreen(void);

void displayPower(int power);
void displaySpeed(int speed);

void displayTFTData();
void displayAhBar();

void displayStatistics();
void displayButtons();
void unlockButtonPress(NextionEventType type, INextionTouchable *widget);

#endif