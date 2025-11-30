#include <Arduino.h>
#include "config.h"
#include "../lib/BrailleMapper/braille_mapper.h"
#include "../lib/ServoController/servo_controller.h"
#include "../lib/WiFiTime/wifi_time.h"
#include <WiFi.h>

ServoController servoController;
WiFiTime wifiTime;

// Buffer for braille pattern (16 servos: 4 cells × 4 dots each)
uint8_t braillePattern[TOTAL_SERVOS];

// Button state variables
bool buttonPressed = false;
bool lastButtonState = false;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
unsigned long buttonPressTime = 0;

// Time tracking
unsigned long lastDisplayUpdate = 0;
unsigned long lastTimeSync = 0;

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32-C3 Braille Clock Starting...");

    // Initialize button pin
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize servo controller
    if (!servoController.begin()) {
        Serial.println("Failed to initialize servo controller!");
        while(1) delay(1000);
    }

    // Initialize WiFi and time sync
    wifiTime.begin();
    
    // Attempt initial time sync
    if (wifiTime.syncTime()) {
        Serial.println("Initial time sync successful");
    } else {
        Serial.println("Initial time sync failed, using default time");
    }

    // Test all servos on startup
    servoController.testAllServos();

    Serial.println("ESP32-C3 Braille Clock Ready!");
    Serial.println("Short press: Force time display update");
    Serial.println("Long press (>3s): Force time re-sync");
}

void handleButtonPress() {
    Serial.println("Button pressed - updating display");
    // Force immediate display update
    lastDisplayUpdate = 0;
}

void handleLongPress() {
    Serial.println("Long press - re-syncing time");
    wifiTime.syncTime();
    lastDisplayUpdate = 0; // Force display update
}

void loop() {
    // Read button state with debouncing
    int reading = digitalRead(BUTTON_PIN);
    
    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }
    
    if ((millis() - lastDebounceTime) > debounceDelay) {
        if (reading == LOW && !buttonPressed) {
            buttonPressed = true;
            buttonPressTime = millis();
        }
        
        if (reading == HIGH && buttonPressed) {
            buttonPressed = false;
            unsigned long pressDuration = millis() - buttonPressTime;
            
            if (pressDuration < 3000) {
                // Short press
                handleButtonPress();
            } else {
                // Long press
                handleLongPress();
            }
        }
    }
    
    lastButtonState = reading;

    // Sync time periodically (every hour)
    if (millis() - lastTimeSync > TIME_SYNC_INTERVAL) {
        wifiTime.syncTime();
        lastTimeSync = millis();
    }

    // Update display periodically (every minute) or when forced
    if (millis() - lastDisplayUpdate > DISPLAY_UPDATE_INTERVAL || lastDisplayUpdate == 0) {
        if (wifiTime.isTimeSet()) {
            int hours = wifiTime.getHours();
            int minutes = wifiTime.getMinutes();
            
            // Generate braille pattern
            get_braille_pattern(hours, minutes, braillePattern);
            
            // Update servo positions
            servoController.updateBrailleDisplay(braillePattern);
            
            Serial.print("Display updated: ");
            Serial.print(wifiTime.getTimeString());
            Serial.print(" (");
            Serial.print(hours);
            Serial.print(":");
            Serial.print(minutes);
            Serial.println(")");
        } else {
            Serial.println("Time not set yet, cannot update display");
        }
        
        lastDisplayUpdate = millis();
    }

    // Print status every 5 minutes
    static unsigned long lastStatus = 0;
    if (millis() - lastStatus > 300000) {
        lastStatus = millis();
        Serial.print("Status - Time: ");
        Serial.print(wifiTime.getTimeString());
        Serial.print(", WiFi: ");
        Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
    }

    delay(100);
}