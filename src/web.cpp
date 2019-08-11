#include <web.h>

void initWebServer() {
    server.onNotFound(notFound);
    server.begin();

    server.on("/tft", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/tft.html");
    });

    // upload a file to /tft
    server.on("/tft", HTTP_POST, [](AsyncWebServerRequest *request){
      request->send(200);
    }, onTftUpload);

    // attach filesystem root at URL /fs
    server.serveStatic("/fs", SPIFFS, "/");
}

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void onTftUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  File file;
  if(!index){
    Serial.printf("UploadStart: %s\n", filename.c_str());
    file = SPIFFS.open("/1.tft", FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
  } else {
    file = SPIFFS.open("/1.tft", FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
  }

  for(size_t i=0; i<len; i++){
    file.write(data[i]);
  }

  Serial.printf("File size: %u B\n", file.size());
  file.close();

  if(final){
    Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);
    cfg.putBool("updateTFT", true);
    Serial.printf("Update flag: %s\n", cfg.getBool("updateTFT", false) ? "true":"false");
    request->send(SPIFFS, "/tft-ok.html");
  }
}

String updateTFT(void) {
  String string = String("");

  File file = SPIFFS.open("/1.tft");
  if(!file || file.isDirectory()){
      Serial.println("Failed to open TFT file for reading");
      return String("Failed to open TFT file for reading");
  }

  uint32_t _undownloadByte = file.size();

  nex.sendCommand("");
  nex.sendCommand("connect");
  recvRetString(string, 500, false);

  Serial.println(string.c_str());

  // set upload baudrate
  Serial.println("Set upload baudrate...");
  string = String(""); 
  String cmd = String("");
  
  String filesize_str = String(_undownloadByte,10);
  String baudrate_str = String(115200,10);
  cmd = "whmi-wri " + filesize_str + "," + baudrate_str + ",0";
  
  Serial.println(cmd);

  nex.sendCommand("");
  nex.sendCCommand(cmd.c_str());
  delay(50);
  recvRetString(string, 500, true);  
  if(string.indexOf(0x05) != -1)
  { 
      Serial.println("Failed to set baudrate");
      //return String("Failed to set baudrate");
  } 

  delay(1000);

  while(NEXTION_PORT.available()) {
    NEXTION_PORT.read();
  }

  // upload TFT file
  Serial.println("Upload TFT file...");

  uint8_t c;
  uint16_t send_timer = 0;
  uint16_t last_send_num = 0;
  string = String("");
  send_timer = _undownloadByte / 4096 + 1;
  last_send_num = _undownloadByte % 4096;

  Serial.print("Chunks: ");
  Serial.println(send_timer);

  while(send_timer)
  {
    Serial.print("Chunks remain: ");
    Serial.println(send_timer);

      if(send_timer == 1)
      {
          for(uint16_t j = 1; j <= 4096; j++)
          {
              if(j <= last_send_num)
              {
                  c = file.read();
                  NEXTION_PORT.write(c);
              }
              else
              {
                  Serial.println("Break");
                  break;
              }
          }
      }

      else
      {
          for(uint16_t i = 1; i <= 4096; i++)
          {
              c = file.read();
              NEXTION_PORT.write(c);
          }
      }
      recvRetString(string,500,true);  
      if(string.indexOf(0x05) != -1)
      { 
        Serial.println("Confirmation!");
        string = "";
      } 
      else
      {
          Serial.println("No confirmation!");
          file.close();
          return String("Can't get confirmation about block complete");
      }
        --send_timer;
  }  

  file.close();
  Serial.println("Done upload");

  // stop update
  return String("Ok");
}

uint16_t recvRetString(String &string, uint32_t timeout,bool recv_flag)
{
    uint16_t ret = 0;
    uint8_t c = 0;
    long start;
    bool exit_flag = false;
    start = millis();
    while (millis() - start <= timeout)
    {
        while (NEXTION_PORT.available())
        {
            c = NEXTION_PORT.read(); 
            if(c == 0)
            {
                continue;
            }
            string += (char)c;
            if(recv_flag)
            {
                if(string.indexOf(0x05) != -1)
                { 
                    exit_flag = true;
                } 
            }
        }
        if(exit_flag)
        {
            break;
        }
    }
    ret = string.length();
    return ret;
}