#include <bluetooth.h>
#include <vector.h>

byte pindex = 0;
byte data;
byte prevData;
byte packetData[260];
short packetLength;

HardwareSerial BTserial(1);

float Diametr1 = WHEEL_DIAMETER;
float PhasesPerRev1 = WHEEL_PHASES;

void bluetoothTask(void *pvParameters) {  
    BTserial.begin(BtBaudRate, SERIAL_8N1, RX_BT, TX_BT);
    BTserial.setRxBufferSize(1024);

    for(;;) {
        delay(10);

        if(!CONTROLLER_AUTOSEND) {
            sendPacket1(SEND_TrmRqst);
        }

        while(BTserial.available()) {
            data = BTserial.read();

            _uart_rx_count++;

            if(data == 0xFF && prevData == 0xFF) {
                packetLength = BTserial.read();
                byte packetType = BTserial.read();

                // data packet (5)
                if(packetType == 5) {
                    memset(packetData, 0, sizeof(packetData));
                    pindex = 0;
                
                    // we are waiting for all data comes to RX
                    while(BTserial.available() < packetLength) {
                        delay(1);
                    }

                    while(pindex < packetLength-1) { // length = type byte + dataBytes[n]
                        packetData[pindex] = BTserial.read();
                        pindex++;
                    }

                    int crc = BTserial.read();
                    int calcCrc = calculateCRC(packetType, packetLength, packetData);

                    if(crc != calcCrc) {
                        Serial.println(F("Got data, but invalid CRC"));
                        continue;
                    }

                    packetType5 packetData5;
                    if(packetLength > sizeof(packetType5))
                      packetLength = sizeof(packetType5);
                    memcpy(&packetData5, packetData, packetLength);

                    currentData.ImpNa1M = 100.0 / (WHEEL_DIAMETER * (PI/10) / WHEEL_PHASES);

                    currentData.speed = packetData5.Spd1Fl;
                    if(currentData.speed > 200) {
                        currentData.speed = 0; // FIXME bug with enabled regen
                    }

                    currentData.voltage = (float)packetData5.UBT/10;
                    currentData.tempCont = (float)packetData5.Temperature2/10;
                    currentData.tempEngine = (float)packetData5.Temperature1/10;
                    currentData.ah = ((((float)packetData5.CurrTuda1 * CurrPerDigit) / 36000.0) / 1000.0);
                    currentData.ahRegen = ((((float)packetData5.CurrRegen1 * CurrPerDigit) / 36000.0) / 1000.0);

                    currentData.flgs3 = packetData5.Flgs3;
                    currentData.flgs4 = packetData5.Flgs4;
                    currentData.flgs5 = packetData5.Flgs5;

                    currentData.distance = (float)(round(100 * ((packetData5.Distance / currentData.ImpNa1M) / 1000)) / 100);
                    currentData.odometer = (float)(round(100 * ((packetData5.Odometr / currentData.ImpNa1M) / 1000)) / 100);
                    currentData.unlocked = (packetData5.Flgs5 & ((long)0x400000)) == 0;

                    if ((((long)currentData.flgs5) & ((long)0x100)) == ((long)0x100)) {
                        currentData.cv = true;
                        currentData.cc = false;
                    } else {
                        currentData.cv = false;
                        currentData.cc = true;
                    }

                    currentData.current = (packetData5.Curr1 * CurrPerDigit)/1000.0;

                    currentData.volt3v3 = (float)packetData5._3VFl / 100.0;
                    currentData.volt5v0 = (float)packetData5._5VFl / 100.0;
                    currentData.volt12v0 = (float)packetData5._12VFl / 100.0;

                    //if(ahDelta) currentData.ah = currentData.ah + ahDelta;

                    checkMaxData();

                    currentDataChanged = true;
                    lastDataSeen = millis();

                }else

                if(data == 6){
                  /*int Tm;
                  Tm = PacketRec[115];
                  Tm = Tm << 8;
                  Tm = Tm + PacketRec[114];
                  Diametr1 = Tm;

                  Tm = PacketRec[117];
                  Tm = Tm << 8;
                  Tm = Tm + PacketRec[116];
                  PhasesPerRev1 = Tm;*/
                }

            }

            prevData = data;
        }   
    }
}

void checkMaxData(void) {
  if(currentData.speed > maxData.speed) {
    maxData.speed = currentData.speed;
  }
  if(currentData.current > maxData.current) {
    maxData.current = currentData.current;
  }
  if(currentData.current < 0 && currentData.current < maxData.currentRegen) {
    maxData.currentRegen = currentData.current;
  }
  if(currentData.tempEngine > maxData.tempEngine) {
    maxData.tempEngine = currentData.tempEngine;
  }
  if(currentData.tempCont > maxData.tempCont) {
    maxData.tempCont = currentData.tempCont;
  }
}

byte calculateCRC(int type, int plength, byte *data) {
  unsigned int summ = type + plength;
  
  for (byte j = 0; j < (plength-1); j++) {
    summ = summ + data[j];
  }
  summ = ~summ;
  
  return (byte)summ;
}

void sendPacket(byte cmd, byte argslen, byte *args)
{
  packetLength=0;
  packetData[packetLength++] = -1;
  packetData[packetLength++] = -1;
  packetData[packetLength++] = argslen + 1;
  packetData[packetLength++] = cmd;
  for(byte i=0; i<argslen; i++){
    packetData[packetLength++] = args[i];
  }
  //Serial.print("now crc ");
  packetData[packetLength++] = calculateCRC(cmd, argslen+1, args);
//Send FF FF D F3 37 AC 2B 33 F1 91 7A B0 EC 46 10 AA D1
//crc must be 26

  Serial.print("Send ");
  for(byte i=0; i<packetLength; i++){
    Serial.print(packetData[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  BTserial.write(packetData, packetLength);
  //for(byte i=0; i<packetLength; i++){
  //  BTserial.write(packetData[i]);
  //}
}

void sendPacket1(byte cmd)
{
  sendPacket(cmd, 0, NULL);
}

void sendPacket2(byte cmd)
{
  byte tmp[1];
  tmp[0] = cmd;
  sendPacket(SEND_SecondCMD, 1, tmp);
}

void sendUnlock(void) {
  if(!currentData.unlocked) {
    currentData.unlocked = true;
    // send SEND_unlock command for unlock controller
    byte unlockseq[12] = {0x37,0xac,0x2b,0x33,0xf1,0x91,0x7a,0xb0,0xec,0x46,0x10,0xaa};
    sendPacket(SEND_Unlock, 12, unlockseq);
  }
}

void sendLock(void) {
  if(currentData.unlocked) {
    currentData.unlocked = false;
    // send SEND_lock command for lock again

    sendPacket1(SEND_lock);
  }
}

void sendSaveSettings(void) {
  // sending SEND_ProgrammOptions
  sendPacket1(SEND_ProgrammOptions);
}

void sendResetTrip(void) {  
  // sending SEND_ResetDistance
  sendPacket1(SEND_ResetDistance);
}

void sendResetAh(void) {
  // send SEND_ClearCurrentAH command
  sendPacket1(SEND_ClearCurrentAH);
}

void sendChargerCurr(void) {
  int chargeA = (int)((CHARGER_CURRENT * 1000) / CurrPerDigit);

  // send SEND_ChagerCurr command
  byte trm[3] = {SEND_ChagerCurr, (byte)chargeA, (byte)(chargeA >> 8)};
  sendPacket(SEND_SecondCMD, 3, trm);
}

void sendStartCharge(void) {
  if(!currentData.unlocked){
    sendUnlock();
    delay(2000);
  }

  sendPacket2(SEND_ChagerViaMotorOn);

  /*if(currentData.unlocked){
    delay(1500);
    //sendChargerCurr();
    //delay(1500);
    sendLock();
  }*/
}


void sendStopCharge(void) {
  if(!currentData.unlocked){
    sendUnlock();
    delay(2000);
  }

  sendPacket2(SEND_ChagerViaMotorOff);
  //delay(1500);

  //sendLock();
}
