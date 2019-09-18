// Vector-S/M Monitor v3
// Version for ESP32
// Special for ElectroTransport.Ru
// (c) 2019 Nikolay Viguro aka Neuronix

#include "main.h"

// internal
// DO NOT change anything below this line if you don't know what you do
#ifdef UseLedBar
LedControl lc = LedControl(LED_DIN, LED_CLK, LED_CS, 1); // Pins: DIN,CLK,CS, # of Display connected
#endif

WiFiUDP ntpUDP; 
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

//Adafruit_BME280 bme; // I2C 

void setup(void)
{
    Serial.begin(SerialBaudRate);
    cfg.begin("vortex", false);

    if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
    }


#ifdef UseLedBar
    // the zero refers to the MAX7219 number, it is zero for 1 chip
    lc.shutdown(0, false); // turn off power saving, enables display
    lc.setIntensity(0, 15); // sets brightness (0~15 possible values)
    lc.clearDisplay(0); // clear screen

    for(byte x = 0; x <= 8; x++) {
      lc.setChar(0, x, '-', false);
    }
#endif    

    NEXTION_PORT.begin(NexBaudRate);
    NEXTION_PORT.setRxBufferSize(512);

    nex.sendCommand("baud=115200");
    delay(100);
    NEXTION_PORT.begin(115200);

    if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println(F("SPIFFS Mount Failed"));
        spiffsMounted = false;
    }

    if(cfg.getBool("updateTFT", false)) {
      Serial.println(F("Update TFT is requested"));
      delay(2000);
      cfg.putBool("updateTFT", false);
      updateTFT();
    }

    //ahDelta = cfg.getDouble("ahDelta", 0);
    
    nex.init();

    version.setText(VERSION);
    status.setText("Starting up WiFi connection...");
    
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(STA_ssid, STA_password);

    int i = 0;
    while (WiFi.status() != WL_CONNECTED && i < 30) {
      delay(100);
      i++;
      Serial.print(".");
    }

    if(WiFi.status() == WL_CONNECTED) {
      status.setText("WiFi connected...");
    } else {
      status.setText("Starting WiFi hotspot...");
      WiFi.softAP(AP_ssid, AP_password);
    }

    timeClient.begin();

    //unsigned int bmestatus = bme.begin();
    //Serial.println(bmestatus);
    delay(500);

    ArduinoOTA.setHostname("Vortex");
    ArduinoOTA.setPort(3232);

    ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";
  
        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.print(F("Start updating "));
        Serial.println(type);
        
      })
      .onEnd([]() {
        Serial.println(F("\nEnd"));
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println(F("Auth Failed"));
        else if (error == OTA_BEGIN_ERROR) Serial.println(F("Begin Failed"));
        else if (error == OTA_CONNECT_ERROR) Serial.println(F("Connect Failed"));
        else if (error == OTA_RECEIVE_ERROR) Serial.println(F("Receive Failed"));
        else if (error == OTA_END_ERROR) Serial.println(F("End Failed"));
      });
  
    ArduinoOTA.begin();

    xTaskCreatePinnedToCore(
                  bluetoothTask,   /* Function to implement the task */
                  "bluetoothTask", /* Name of the task */
                  14000,           /* Stack size in words */
                  NULL,            /* Task input parameter */
                  100,             /* Priority of the task */
                  NULL,            /* Task handle. */
                  1);              /* Core where the task should run */
                  
    voltageInROM = cfg.getUInt("voltage", 0);

  initWebServer();

  status.setText("Syncthing the time...");
  if(timeClient.update()){
    rtc.adjust(DateTime(timeClient.getEpochTime() + TIMEZONE_OFFSET));
    timeClient.end();
  }

  status.setText("Waiting for the data...");

  setupNoDataScreen();
}

unsigned long lastScreenRefresh = 0;

void loop(void) {
  //delay(10);
  nex.poll();

  ArduinoOTA.handle();

  if((millis() - lastDataSeen) > (10 * 1000)) {
    if(initialized && currentDataChanged && lastDataSeen != 0) {
      initialized = false;
      setupNoDataScreen();
    }
  } else {
    if(!initialized && currentDataChanged) {
      if(false && firstRun) {//TODO uncomment
        Serial.println(F("First run detected"));
        firstRun = false; // do this check only once at startup

        Serial.print(F("Current voltage is: "));
        Serial.println((int)currentData.voltage);
        Serial.print(F("Remembered voltage is: "));
        Serial.println(voltageInROM);

        boolean ahReset = false;

        //check with little gap to obtain voltage 
        if(voltageInROM < (int)currentData.voltage - 2) {
          ahReset = true;
        }

        voltageInROM = currentData.voltage;
        cfg.putUInt("voltage", voltageInROM);

        // if previous remembered battery voltage lower than current value
        // we suppose that the battery was recharged
        // So we reset Ah counters and store new value in EEPROM
        if(ahReset) {
          status.setText("Unlocking the controller...");

          sendUnlock();
          delay(1000);

          Serial.println(F("Reseting Ah counters"));
          status.setText("Reseting Ah counters...");
          
          sendResetAh();
          sendResetTrip();
          
          delay(1000);

          status.setText("Locking the controller...");
          delay(1000);
          sendLock();

          // do some calculations about delta in Ah between full charged battery and current charge
          // later we apply this value to Ah counter
          // this value has been calculationg only if battery type is li-ion
          /* strange thing
          if(LI_ION_BATTERY_TYPE) {
            ahDelta = BATTERY_CAPACITY - (BATTERY_CAPACITY * (voltageInROM - BATTERY_CAPACITY) / (FULL_VOLTAGE - EMPTY_VOLTAGE));
            cfg.putDouble("ahDelta", ahDelta);
          }*/
        }
      }

      chargeModeButton.attachCallback(&unlockButtonPress);
      resetAhButton.attachCallback(&unlockButtonPress);
      resetDistanceButton.attachCallback(&unlockButtonPress);
      unlockButton.attachCallback(&unlockButtonPress);

      initialized = true;
      setupScreen();
    }
  }

  //limit refresh rate because TFT can not get in time
  if(initialized && currentDataChanged && ((millis() - lastScreenRefresh) > 500)) {
#ifdef UseLedBar
    displaySpeed(currentData.speed);
    displayPower(fabs(currentData.voltage * currentData.current));
#endif

    lastScreenRefresh = millis();

    byte page = nex.getCurrentPage();
    if(page == 0)page = nex.getCurrentPage();
    //if(page == 0)main.show(); tft bug

    if(page){
      currentDataChanged = false;

      // settings page
      if(page == 3) {
        if(prevPageWas != 3)
          displayButtons();
      }else

      // statistics page
      if(page == 2) {
        displayStatistics();
      }else

      // main page
      if(page == 1) {
        displayTFTData();
        showDate();
      }
      prevPageWas = page;
    }

  }
}


void displayTFTData() {

  static char strbuf[10];
  //speed was below zero while breaks
  dtostrf(fabs(currentData.speed), 2, 0, strbuf);
  if(!speed.setText(strbuf)) return;

  dtostrf(currentData.voltage, 5, 1, strbuf);
  if(!voltage.setText(strbuf)) return;
 
    
  if (fabs(currentData.current) > 100) {
    dtostrf(currentData.current, 5, 0, strbuf);
  } else if (fabs(currentData.current) > 10) {
    dtostrf(currentData.current, 5, 1, strbuf);
  } else {
    dtostrf(currentData.current, 5, 2, strbuf);
  }
  if(!current.setText(strbuf)) return;

  dtostrf(currentData.tempCont, 5, 1, strbuf);
  if(!tempCont.setText(strbuf)) return;

  if(currentData.tempEngine >= 100){
    dtostrf(currentData.tempEngine, 5, 1, strbuf);
  }else{
    dtostrf(currentData.tempEngine, 4, 1, strbuf);
  }
  if(!tempEngine.setText(strbuf)) return;

  dtostrf(fabs(currentData.ah), 5, 1, strbuf);
  if(!ah.setText(strbuf)) return;

  if(currentData.ah > 0.1){
    dtostrf((currentData.ahRegen/currentData.ah)*100, 3, 1, strbuf);
    sprintf(strbuf, "%s%%", strbuf);
    if(!ahRegen.setText(strbuf)) return;
  }else{
    if(!ahRegen.setText("0.0%")) return;
  }

  dtostrf(currentData.distance, 7, 2, strbuf);
  sprintf(strbuf, "%s km", strbuf);
  if(!distance.setText(strbuf)) return;

  dtostrf(currentData.odometer, 7, 0, strbuf);
  sprintf(strbuf,"%s km", strbuf);
  if(!odometer.setText(strbuf)) return;

  dtostrf(currentData.voltage * currentData.current, 4, 0, strbuf);
  if(!power.setText(strbuf)) return;

  ahBar.setValue(100 - (fabs(currentData.ah - currentData.ahRegen) / (BATTERY_CAPACITY / 100)));

  if(currentData.distance > 0.1 && currentData.ah > 0.1){
    float wattPerKm = ((currentData.ah - currentData.ahRegen)*BATTERY_NOMINAL_VOLTAGE)/currentData.distance;
    //dtostrf(wattPerKm, 6, 1, strbuf);
    //wattKm.setText(strbuf) || return;

    long int kmforleft = ((BATTERY_CAPACITY - currentData.ah + currentData.ahRegen)*BATTERY_NOMINAL_VOLTAGE)/wattPerKm;
    dtostrf(kmforleft, 4, 0, strbuf);
    sprintf(strbuf,"%s km", strbuf);
    if(!kmleftavg.setText(strbuf)) return;
  }else{
    if(!kmleftavg.setText("-- km")) return;
  }

  if(currentData.speed > 0 && currentData.current > 1){
    float f_kmleftperbatt = (((BATTERY_CAPACITY - currentData.ah + currentData.ahRegen)*BATTERY_NOMINAL_VOLTAGE)/(currentData.voltage * currentData.current))*currentData.speed;
    dtostrf(f_kmleftperbatt, 4, 0, strbuf);
    sprintf(strbuf,"%s km", strbuf);
    if(!kmleftperbatt.setText(strbuf)) return;
  }else{
    if(!kmleftperbatt.setText("~ km")) return;
  }

  //TODO: some exception that crash esp
  //bme.readTemperature();
  //dtostrf(bme.readTemperature(), 4, 1, strbuf);
  if(!outTemp.setText("")) return;
}

void displayStatistics() {
  static char strbuf[10];
  dtostrf(fabs(maxData.speed), 2, 0, strbuf);
  if(!maxSpeed.setText(strbuf)) return;

  if (maxData.current > 100) {
    dtostrf(maxData.current, 6, 0, strbuf);
  } else if (maxData.current > 10) {
    dtostrf(maxData.current, 6, 1, strbuf);
  } else {
    dtostrf(maxData.current, 6, 2, strbuf);
  }
  if(!maxA.setText(strbuf)) return;

  if (maxData.currentRegen < 100) {
    dtostrf(maxData.currentRegen, 6, 0, strbuf);
  } else if (maxData.currentRegen < 10) {
    dtostrf(maxData.currentRegen, 6, 1, strbuf);
  } else {
    dtostrf(maxData.currentRegen, 6, 2, strbuf);
  }
  if(!maxARegen.setText(strbuf)) return;

  dtostrf(maxData.tempEngine, 6, 1, strbuf);
  if(!maxEngT.setText(strbuf)) return;

  dtostrf(maxData.tempCont, 6, 1, strbuf);
  if(!maxContT.setText(strbuf)) return;

  long rideInMs = millis();                     
  int hours = (rideInMs % DAY_MS) / HOUR_MS;                    
  int minutes = ((rideInMs % DAY_MS) % HOUR_MS) / MINUTE_MS;

  String hourStr;
  String minuteStr;

  if(hours < 10) {
    hourStr = "0" + hours;
  } else {
    hourStr = hours;
  }

  if(minutes < 10) {
    minuteStr = "0" + minutes;
  } else {
    minuteStr = minutes;
  }

  String rTimeStr = hourStr + ":" + minuteStr;
  rTimeStr.toCharArray(strbuf, 6);
  if(!rideTime.setText(strbuf)) return;

  if(currentData.distance > 0.1 && currentData.ah > 0.1){
    float wattPerKm = ((currentData.ah - currentData.ahRegen)*BATTERY_NOMINAL_VOLTAGE)/currentData.distance;
    dtostrf(wattPerKm, 6, 1, strbuf);
    if(!wattKm.setText(strbuf)) return;

    long int kmforfull = (BATTERY_CAPACITY*BATTERY_NOMINAL_VOLTAGE)/wattPerKm;
    dtostrf(kmforfull, 6, 1, strbuf);
    if(!kmfull.setText(strbuf)) return;

    long int kmforleft = ((BATTERY_CAPACITY - currentData.ah + currentData.ahRegen)*BATTERY_NOMINAL_VOLTAGE)/wattPerKm;
    dtostrf(kmforleft, 6, 1, strbuf);
    if(!kmleft.setText(strbuf)) return;
  }

  dtostrf(currentData.volt3v3, 3, 1, strbuf);
  if(!volt3v3.setText(strbuf)) return;

  dtostrf(currentData.volt5v0, 3, 1, strbuf);
  if(!volt5v0.setText(strbuf)) return;

  dtostrf(currentData.volt12v0, 4, 1, strbuf);
  if(!volt12v0.setText(strbuf)) return;

  dtostrf(currentData.voltage, 4, 1, strbuf);
  if(!voltbat.setText(strbuf)) return;

}

void unlockButtonPress(NextionEventType type, INextionTouchable *widget)
{
  if (type == NEX_EVENT_PUSH)
  switch(widget->getComponentID()){
    case 11: //unlockBtn
      if(currentData.unlocked){
        sendLock();
      }else{
        sendUnlock();
      }
      delay(1000);
      displayButtons();
      unlockButton.setActive(false);
      break;

    case 10: //resetDistance
      Serial.println(F("Reset trip requested!"));
      if(!currentData.unlocked){
        sendUnlock();
        delay(1000);
      }
      sendResetTrip();
      if(!currentData.unlocked){
        delay(1000);
        sendLock();
      }
      resetDistanceButton.setActive(false);
      break;

    case 9: //resetAh
      Serial.println(F("Reset Ah counters requested!"));
      if(!currentData.unlocked){
        sendUnlock();
        delay(1000);
      }
      
      sendResetAh();

      /* TODO: review
      ahDelta = 0;
      cfg.putDouble("ahDelta", 0);*/

      if(!currentData.unlocked){
        delay(1000);
        sendLock();
      }
      resetAhButton.setActive(false);
      break;

    case 8: //chargeMode
      if(!chargeMode) {
        Serial.println(F("Charge mode activated!"));
        sendStartCharge();
        chargeMode = true;
        chargeState.setText("ON");
        chargeState.setForegroundColour(NEX_COL_GREEN);
      }else{
        Serial.println(F("Charge mode deactivated!"));
        sendStopCharge();
        chargeMode = false;
        chargeState.setText("OFF");
        chargeState.setForegroundColour(NEX_COL_RED);
      }

      break;
  }
} 

void displayButtons()
{
    if(currentData.unlocked){
      unlockButton.setStringProperty("txt", "Lock");
    }else{
      unlockButton.setStringProperty("txt", "Unlock");
    }
}

#ifdef UseLedBar
void displayPower(int power) {
  
  byte num1, num2, num3, num4;

  if(power >= 1000) {
    num4 = power % 10;
    num3 = (power / 10) % 10;
    num2 = (power / 100) % 10;
    num1 = (power / 1000) % 10;
 
    lc.setDigit(0, 3, num1, false);
    lc.setDigit(0, 2, num2, false);
    lc.setDigit(0, 1, num3, false);
    lc.setDigit(0, 0, num4, false);
    
  }

  if(power < 1000 && power >= 100) {
    num3 = power % 10;
    num2 = (power / 10) % 10;
    num1 = (power / 100) % 10;

    lc.clearLed(0, 3);
    lc.setDigit(0, 2, num1, false);
    lc.setDigit(0, 1, num2, false);
    lc.setDigit(0, 0, num3, false);
     
  }

  if(power < 100 && power >= 10) {
    num2 = power % 10;
    num1 = (power / 10) % 10;

    lc.clearLed(0, 3);
    lc.clearLed(0, 2);
    lc.setDigit(0, 1, num1, false);
    lc.setDigit(0, 0, num2, false);
    
  }

  if(power < 10) {
    
    lc.clearLed(0, 3);
    lc.clearLed(0, 2);
    lc.clearLed(0, 1);
    lc.setDigit(0, 0, power, false);
    
  }
   
}

void displaySpeed(int speed) {
  byte num1, num2;

  if(speed < 100 && speed >= 10) {
    num2 = speed % 10;
    num1 = (speed / 10) % 10;

    lc.setDigit(0, 6, num1, false);
    lc.setDigit(0, 5, num2, false);
  }

  if(speed < 10) {
    lc.clearLed(0, 6);
    lc.setDigit(0, 5, speed, false);
  }
  
}
#endif

void showDate() {
  DateTime now = rtc.now();
  static char dataLabel[10];  
  
  sprintf(dataLabel, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  if(!timeLabel.setText(dataLabel)) return;
  
  sprintf(dataLabel, "%02d.%02d.%04d", now.day(), now.month(), now.year());
  if(!dateLabel.setText(dataLabel)) return;

} 

void setupScreen(void) {
  main.show();
#ifdef UseLedBar
  lc.clearDisplay(0);
#endif  
}

void setupNoDataScreen(void) {
#ifdef UseLedBar
  for(byte x = 0; x <= 8; x++) {
    lc.setChar(0, x, '-', false);
  }
#endif  
  splash.show();
  status.setText("Waiting for the data...");
  if(_uart_rx_count > 0){
    static char dataLabel[10];  
    sprintf(dataLabel, "%d", _uart_rx_count);
    version.setText(dataLabel);
  }else{
    version.setText(VERSION);
  }
}