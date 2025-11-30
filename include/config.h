#ifndef CONFIG_H
#define CONFIG_H

// WiFi Configuration
#define WIFI_SSID "WIFI_SSID"
#define WIFI_PASSWORD "WIFI_PASSWORD"

// NTP Configuration
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 0  
#define UTC_OFFSET_DST 0  

// Servo Configuration
#define SERVO_MIN_PULSE 500
#define SERVO_MAX_PULSE 2500
#define SERVO_FREQUENCY 50

// PCA9685 I2C Address
#define PCA9685_1_ADDRESS 0x40
#define PCA9685_2_ADDRESS 0x41

// Braille Display Configuration
#define NUM_CELLS 4           
#define DOTS_PER_CELL 4       
#define TOTAL_SERVOS (NUM_CELLS * DOTS_PER_CELL) 

// Servo positions 
#define SERVO_UP_POSITION 120   
#define SERVO_DOWN_POSITION 30 

// Pin Definitions for ESP32-C3
#define I2C_SDA 4
#define I2C_SCL 5
#define BUTTON_PIN 3

// Time update intervals
#define TIME_SYNC_INTERVAL 3600000 
#define DISPLAY_UPDATE_INTERVAL 60000

#endif