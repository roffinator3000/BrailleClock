#include "wifi_time.h"
#include <config.h>
#include <WiFi.h>
#include <time.h>

void WiFiTime::begin() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi!");
        configTime(UTC_OFFSET * 3600, UTC_OFFSET_DST * 3600, NTP_SERVER);
    } else {
        Serial.println("\nFailed to connect to WiFi!");
    }
}

bool WiFiTime::syncTime() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected, attempting reconnect...");
        begin();
        return false;
    }
    
    Serial.println("Syncing time with NTP server...");
    configTime(UTC_OFFSET * 3600, UTC_OFFSET_DST * 3600, NTP_SERVER);
    
    // Wait for time to be set
    delay(1000);
    
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    if (timeinfo.tm_year > (2024 - 1900)) {
        timeSet = true;
        lastSync = millis();
        Serial.println("Time synchronized successfully: " + getTimeString());
        return true;
    } else {
        Serial.println("Failed to get time from NTP server");
        return false;
    }
}

int WiFiTime::getHours() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_hour;
}

int WiFiTime::getMinutes() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_min;
}

int WiFiTime::getSeconds() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    return timeinfo.tm_sec;
}

String WiFiTime::getTimeString() {
    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    
    char timeStr[9];
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
    return String(timeStr);
}

bool WiFiTime::isTimeSet() {
    return timeSet;
}