#include <shared.h>

char VERSION[10] = "v.1.6.4";
Preferences cfg;

volatile CurrentData currentData;
volatile bool currentDataChanged = false;
volatile MaxData maxData;

volatile float ahDelta = 0;
volatile unsigned long lastDataSeen = 0;

volatile unsigned long _uart_rx_count = 0;