#include <bluetooth.h>
#include <vector.h>

byte pindex = 0;
byte data;
byte prevData;
byte packetData[260];
byte packetLength;

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
/*
            // sending TrmRequest (request controller for data)
            BTserial.write(-1);     // 0xff
            BTserial.write(-1);     // 0xff
            BTserial.write(1);      // data length
            BTserial.write(SEND_TrmRqst);    // command
            BTserial.write(-115);   // packet crc
             */
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
                
                    // reading the response of data packet
                    // packet structure is described below code
                    
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


                    //TODO check is locked to isLocked
                    //currentData.ImpNa1M = 100.0 / (Diametr1 * 0.314159 / PhasesPerRev1); // 100 / (diameter * PI / phases)
                    currentData.ImpNa1M = 100.0 / (WHEEL_DIAMETER * 0.314159 / WHEEL_PHASES); // 100 / (diameter * PI / phases)
                    
                    currentData.speed = packetData[185] + (packetData[186] << 8);
                    if(currentData.speed > 200) {
                        currentData.speed = 0; // FIXME bug with enabled regen
                    }

                    currentData.voltage = ((float)(packetData[38] + (packetData[39] << 8)) / 10);
                    currentData.tempCont = ((float)(packetData[32] + (packetData[33] << 8))) / 10;
                    currentData.tempEngine = ((float)(packetData[34] + (packetData[35] << 8))) / 10;
                    currentData.ah = (float)(((((float)((long)packetData[83]) + (((long)(packetData[84])) << 8) + (((long)(packetData[85])) << 16) + (((long)(packetData[86])) << 24)) * 48.34) / 36000.0) / 1000);
                    currentData.ahRegen = (float)(((((float)((long)packetData[87]) + (((long)(packetData[88])) << 8) + (((long)(packetData[89])) << 16) + (((long)(packetData[90])) << 24)) * 48.34) / 36000.0 ) / 1000);

                    currentData.flgs4 = packetData[91] + (((long)packetData[92]) << 8) + (((long)packetData[93]) << 16) + (((long)packetData[94]) << 24);
                    currentData.flgs5 = packetData[95] + (((long)packetData[96]) << 8) + (((long)packetData[97]) << 16) + (((long)packetData[98]) << 24);
                    currentData.flgs3 = packetData[99] + (((long)packetData[100]) << 8) + (((long)packetData[101]) << 16) + (((long)packetData[102]) << 24);

                    currentData.distance = (float)(round(100 * (((((long)packetData[158]) + (((long)packetData[159]) << 8) + (((long)packetData[160]) << 16) + (((long)packetData[161]) << 24)) / currentData.ImpNa1M) / 1000)) / 100);
                    currentData.odometer = (float)(round(100 * (((((long)packetData[26]) + (((long)packetData[27]) << 8) + (((long)packetData[28]) << 16) + (((long)packetData[29]) << 24)) / currentData.ImpNa1M) / 1000)) / 100);
                    currentData.unlocked = (currentData.flgs5 & ((long)0x400000)) == 0;

                    /*if(ahDelta) {
                        currentData.ah = currentData.ah + ahDelta;
                    }*/

                    if ((((long)currentData.flgs5) & ((long)0x100)) == ((long)0x100)) {
                        currentData.cv = true;
                        currentData.cc = false;
                    } else {
                        currentData.cv = false;
                        currentData.cc = true;
                    }

                    currentData.current = (packetData[20] + (((int8_t)packetData[21]) << 8)) * 0.048;

                    checkMaxData();

                    lastDataSeen = millis();

                    if(!gotData) {
                      gotData = true;
                    }
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

/* 
 *  -----------------------------------------------
 *  Data (5) packet structure
 *  -----------------------------------------------
 *  
 *  packetData[0] // CntSample
 *  packetData[1] // CntSample
 *  packetData[2] // CntSample
 *  packetData[3] // CntSample
 *  
 *  packetData[4] // AlfaXRes
 *  packetData[5] // AlfaXRes
 *  
 *  packetData[6] // AlfaYRes
 *  packetData[7] // AlfaYRes
 *  
 *  packetData[8] // GyroX
 *  packetData[9] // GyroX
 *  
 *  packetData[10] // TiltXres
 *  packetData[11] // TiltXres
 *  
 *  packetData[12] // TiltYres
 *  packetData[13] // TiltYres
 *  
 *  packetData[14] // Ep
 *  packetData[15] // Ep
 *  
 *  packetData[16] // Ei
 *  packetData[17] // Ei
 *  
 *  packetData[18] // PWM1
 *  packetData[19] // PWM1
 *  
 *  packetData[20] // Curr1 --- battery current
 *  packetData[21] // Curr1 --- battery current
 *  
 *  packetData[22] // Spd1USTKMH
 *  packetData[23] // Spd1USTKMH
 *  
 *  packetData[24] // Temperature3
 *  packetData[25] // Temperature3
 *  
 *  packetData[26] // Odometer
 *  packetData[27] // Odometer
 *  packetData[28] // Odometer
 *  packetData[29] // Odometer
 *  
 *  packetData[30] // Spd1Res
 *  packetData[31] // Spd1Res
 *  
 *  packetData[32] // Temperature2 --- controller temp
 *  packetData[33] // Temperature2 --- controller temp
 *  
 *  packetData[34] // Temperature1 --- engine temp
 *  packetData[35] // Temperature1 --- engine temp
 *  
 *  packetData[36] // Kp
 *  packetData[37] // Kp
 *  
 *  packetData[38] // UBT ------ BATTERY
 *  packetData[39] // UBT ------ VOLTAGE
 *  
 *  packetData[40] // GyroVert_Steer
 *  packetData[41] // GyroVert_Steer
 *  
 *  packetData[42] // StatFlags
 *  packetData[43] // StatFlags
 *  
 *  packetData[44] // GyroZFilter
 *  packetData[45] // GyroZFilter
 *  
 *  packetData[46] // AlfaYResPrevAv
 *  packetData[47] // AlfaYResPrevAv
 *  
 *  packetData[48] // DiffBetweenTilts
 *  packetData[49] // DiffBetweenTilts
 *  
 *  packetData[50] // Flgs
 *  packetData[51] // Flgs
 *  packetData[52] // Flgs
 *  packetData[53] // Flgs
 *  
 *  packetData[54] // Flgs1
 *  packetData[55] // Flgs1
 *  packetData[56] // Flgs1
 *  packetData[57] // Flgs1
 *  
 *  packetData[58] // Flgs2
 *  packetData[59] // Flgs2
 *  packetData[60] // Flgs2
 *  packetData[61] // Flgs2
 *  
 *  packetData[62] // TiltZad
 *  packetData[63] // TiltZad
 *  
 *  packetData[64] // StartRot
 *  packetData[65] // StartRot
 *  
 *  packetData[66] // Ki
 *  packetData[67] // Ki
 *  
 *  packetData[68] // KRot
 *  packetData[69] // KRot
 *  
 *  packetData[70] // KpRot
 *  packetData[71] // KpRot
 *  
 *  packetData[72] // KiRot
 *  packetData[73] // KiRot
 *  
 *  packetData[74] // Preas1ADC
 *  packetData[75] // Preas1ADC
 *  
 *  packetData[76] // Preas2ADC
 *  packetData[77] // Preas2ADC
 *  
 *  packetData[78] // CurrLimitTek
 *  
 *  packetData[79] // KpSPD
 *  packetData[80] // KpSPD
 *  
 *  packetData[81] // AngleLimitReal
 *  packetData[82] // AngleLimitReal
 *  
 *  packetData[83] // CurrTuda1 ------------
 *  packetData[84] // CurrTuda1 ------------  Ah consumed
 *  packetData[85] // CurrTuda1 ------------
 *  packetData[86] // CurrTuda1 ------------
 *  
 *  packetData[87] // CurrRegen1 ------------
 *  packetData[88] // CurrRegen1 ------------ Ah regen
 *  packetData[89] // CurrRegen1 ------------
 *  packetData[90] // CurrRegen1 ------------
 *  
 *  packetData[91] // Flgs4
 *  packetData[92] // Flgs4
 *  packetData[93] // Flgs4
 *  packetData[94] // Flgs4
 *  
 *  packetData[95] // Flgs5
 *  packetData[96] // Flgs5
 *  packetData[97] // Flgs5
 *  packetData[98] // Flgs5
 *  
 *  packetData[99] // Flgs3
 *  packetData[100] // Flgs3
 *  packetData[101] // Flgs3
 *  packetData[102] // Flgs3
 *  
 *  packetData[103] // TimerCnt
 *  packetData[104] // TimerCnt
 *  packetData[105] // TimerCnt
 *  packetData[106] // TimerCnt
 *  
 *  packetData[107] // _3VFl
 *  packetData[108] // _3VFl
 *  
 *  packetData[109] // _5VFl
 *  packetData[110] // _5VFl
 *  
 *  packetData[111] // _12VFl
 *  packetData[112] // _12VFl
 *  
 *  packetData[113] // V4
 *  packetData[114] // V4
 *  
 *  packetData[115] // V5
 *  packetData[116] // V5
 *  
 *  packetData[117] // V6
 *  packetData[118] // V6
 *  
 *  packetData[119] // V7
 *  packetData[120] // V7
 *  
 *  packetData[121] // V8
 *  packetData[122] // V8
 *  
 *  packetData[123] // V9
 *  packetData[124] // V9
 *  
 *  packetData[125] // V10
 *  packetData[126] // V10
 *  
 *  packetData[127] // V11
 *  packetData[128] // V11 
 *  
 *  packetData[129] // V12
 *  packetData[130] // V12
 *  
 *  packetData[131] // V13
 *  packetData[132] // V13
 *  
 *  packetData[133] // V14
 *  packetData[134] // V14
 *  
 *  packetData[135] // V15
 *  packetData[136] // V15
 *  
 *  packetData[137] // V16
 *  packetData[138] // V16
 *  
 *  packetData[139] // V17
 *  packetData[140] // V17
 *  
 *  packetData[141] // V18
 *  packetData[142] // V18
 *  
 *  packetData[143] // BatteryKeys
 *  packetData[144] // BatteryKeys
 *  packetData[145] // BatteryKeys
 *  packetData[146] // BatteryKeys
 *  
 *  packetData[147] // ChagerKeys
 *  
 *  packetData[148] // AccX
 *  packetData[149] // AccX
 *  
 *  packetData[150] // AccY
 *  packetData[151] // AccY
 *  
 *  packetData[152] // AccZ
 *  packetData[153] // AccZ
 *  
 *  packetData[154] // Sensor1_Prev
 *  
 *  packetData[155] // Sensor2_Prev
 *  
 *  packetData[156] // Temperature4
 *  packetData[157] // Temperature4
 *  
 *  packetData[158] // Distance
 *  packetData[159] // Distance
 *  packetData[160] // Distance
 *  packetData[161] // Distance
 *  
 *  packetData[162] // ProtectValue
 *  packetData[163] // ProtectValue
 *  
 *  packetData[164] // Temperature
 *  
 *  packetData[165] // _48V
 *  
 *  packetData[166] // KAccGyroSteer
 *  
 *  packetData[167] // GyroXFl
 *  packetData[168] // GyroXFl
 *  
 *  packetData[169] // GyroYFl
 *  packetData[170] // GyroYFl
 *  
 *  packetData[171] // GyroZFl
 *  packetData[172] // GyroZFl
 *  
 *  packetData[173] // Ed
 *  packetData[174] // Ed
 *  
 *  packetData[175] // GyroYAv
 *  packetData[176] // GyroYAv
 *  
 *  packetData[177] // KdI
 *  packetData[178] // KdI
 *  
 *  packetData[179] // MaxSpdKMH
 *  packetData[180] // MaxSpdKMH
 *  
 *  packetData[181] // PhasesCnt2
 *  packetData[182] // PhasesCnt2
 *  packetData[183] // PhasesCnt2
 *  packetData[184] // PhasesCnt2
 *  
 *  packetData[185] // Spd1Fl --\ SPEED
 *  packetData[186] // Spd1Fl --/ SPEED
 *  
 *  packetData[187] // Temperature5
 *  packetData[188] // Temperature5 
 *  
 *  packetData[189] // Temperature6
 *  packetData[190] // Temperature6 
 *  
 *  packetData[191] // Phase1Period1
 *  packetData[192] // Phase1Period1 
 *  packetData[193] // Phase1Period1
 *  packetData[194] // Phase1Period1 
 *  
 *  packetData[195] // Phase1Period2
 *  packetData[196] // Phase1Period2 
 *  packetData[197] // Phase1Period2
 *  packetData[198] // Phase1Period2 
 *
 *  packetData[199] // EpCurr1
 *  packetData[200] // EpCurr1 
 *  packetData[201] // EpCurr1
 *  packetData[202] // EpCurr1 
 *  
 *  packetData[203] // Phase1Period4
 *  packetData[204] // Phase1Period4 
 *  packetData[205] // Phase1Period4
 *  packetData[206] // Phase1Period4 
 *  
 *  packetData[207] // Phase1Period5
 *  packetData[208] // Phase1Period5 
 *  packetData[209] // Phase1Period5
 *  packetData[210] // Phase1Period5 
 *  
 *  packetData[211] // Phase1Period6
 *  packetData[212] // Phase1Period6  
 *  packetData[213] // Phase1Period6
 *  packetData[214] // Phase1Period6 
 *  
 *  packetData[215] // PDC3
 *  packetData[216] // PDC3
 *  
 *  packetData[217] // CriticalError
 *  
 *  packetData[218] // Preas1ADCResCalibr
 *  packetData[219] // Preas1ADCResCalibr 
 *  
 *  packetData[220] // ADCThrottleBreak
 *  packetData[221] // ADCThrottleBreak 
 *  
 *  packetData[222] // RCON_Mem
 *  packetData[223] // RCON_Mem 
 *  
 *  packetData[224] // FirmwareVersion
 *  packetData[225] // FirmwareVersion 
 *  
 *  packetData[226] // MPU6050Err
 *  
 *  packetData[227] // PhaseCurr
 *  packetData[228] // PhaseCurr 
 *  
 *  packetData[229] // HallErrCnt
 *  packetData[230] // HallErrCnt 
 *  
 *  packetData[231] // CurrPhaseLimitTek
 *  packetData[232] // CurrPhaseLimitTek
 *  
 *  packetData[233] // PhasePer1
 *  packetData[234] // PhasePer1 
 *  packetData[235] // PhasePer1
 *  packetData[236] // PhasePer1 
 *  
 *  packetData[237] // Halls
 *  
 *  packetData[238] // HallDelay1_Tek_F
 *  
 *  packetData[239] // HallDelay1_Tek_B 
 */