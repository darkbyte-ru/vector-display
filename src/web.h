#ifndef web_h
#define web_h
#include "FreeRTOS.h"
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <shared.h>

static AsyncWebServer server(80);

void initWebServer();

void notFound(AsyncWebServerRequest *request);
String updateTFT(void);
uint16_t recvRetString(String &string, uint32_t timeout,bool recv_flag);

void onTftUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

#endif