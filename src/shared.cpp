#include <shared.h>

char VERSION[10] = "v.1.6.4";
Preferences cfg;

volatile CurrentData currentData;
volatile MaxData maxData;

volatile float ahDelta = 0;
volatile unsigned long lastDataSeen = 0;
volatile bool gotData = false;

volatile unsigned long _uart_rx_count = 0;