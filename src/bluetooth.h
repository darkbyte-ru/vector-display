#ifndef bluetooth_h
#define bluetooth_h

#include <Arduino.h>
#include <settings.h>
#include <shared.h>

void bluetoothTask(void *pvParameters);
void checkMaxData(void);
byte calculateCRC(int type, int plength, byte *data);
void sendPacket(byte cmd, byte argslen, byte *args);
void sendPacket1(byte cmd);
void sendPacket2(byte cmd);

void sendUnlock(void);
void sendLock(void);
void sendSaveSettings(void);
void sendResetTrip(void);
void sendResetAh(void);
void sendStartCharge(void);
void sendStopCharge(void);
void sendChargerCurr(void);

#endif