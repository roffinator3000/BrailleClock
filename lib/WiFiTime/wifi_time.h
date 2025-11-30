#ifndef WIFI_TIME_H
#define WIFI_TIME_H

#include <Arduino.h>

class WiFiTime {
public:
    void begin();
    bool syncTime();
    int getHours();
    int getMinutes();
    int getSeconds();
    String getTimeString();
    bool isTimeSet();
    
private:
    bool timeSet = false;
    unsigned long lastSync = 0;
};

#endif