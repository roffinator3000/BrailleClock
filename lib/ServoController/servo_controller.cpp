#include "servo_controller.h"
#include <config.h>

ServoController::ServoController() 
    : pwm1(PCA9685_1_ADDRESS), pwm2(PCA9685_2_ADDRESS) {}

bool ServoController::begin() {
    Wire.begin(I2C_SDA, I2C_SCL);
    
    if (!pwm1.begin()) {
        Serial.println("Could not find PCA9685 #1!");
        return false;
    }
    
    if (!pwm2.begin()) {
        Serial.println("Could not find PCA9685 #2!");
        return false;
    }
    
    pwm1.setPWMFreq(SERVO_FREQUENCY);
    pwm2.setPWMFreq(SERVO_FREQUENCY);
    Serial.println("Both PCA9685 controllers initialized successfully");
    
    // Initialize all servos to down position
    allDown();
    
    return true;
}

uint8_t ServoController::angleToPulse(uint8_t angle) {
    uint16_t pulse = map(angle, 0, 180, SERVO_MIN_PULSE, SERVO_MAX_PULSE);
    return pulse;
}

Adafruit_PWMServoDriver* ServoController::getDriverForServo(uint8_t servoNum) {
    if (servoNum < 8) {
        return &pwm1;
    } else {
        return &pwm2;
    }
}

uint8_t ServoController::getChannelForServo(uint8_t servoNum) {
    return servoNum % 8;  // 8 channels per PCA9685
}

void ServoController::setServoPosition(uint8_t servoNum, uint8_t position) {
    if (servoNum >= TOTAL_SERVOS) return;
    
    Adafruit_PWMServoDriver* driver = getDriverForServo(servoNum);
    uint8_t channel = getChannelForServo(servoNum);
    uint8_t pulse = angleToPulse(position);
    
    driver->setPWM(channel, 0, pulse);
}

void ServoController::updateBrailleDisplay(const uint8_t* braillePattern) {
    for (int i = 0; i < TOTAL_SERVOS; i++) {
        if (braillePattern[i] == 1) {
            setServoPosition(i, SERVO_UP_POSITION);  // Raise dot
        } else {
            setServoPosition(i, SERVO_DOWN_POSITION); // Lower dot
        }
    }
}

void ServoController::testAllServos() {
    Serial.println("Testing all 16 servos...");
    
    // Test each servo sequentially
    for (int i = 0; i < TOTAL_SERVOS; i++) {
        Serial.print("Testing servo ");
        Serial.println(i);
        
        setServoPosition(i, SERVO_UP_POSITION);
        delay(200);
        setServoPosition(i, SERVO_DOWN_POSITION);
        delay(100);
    }
    
    // Test all servos together
    allUp();
    delay(1000);
    allDown();
    delay(500);
    
    Serial.println("Servo test completed");
}

void ServoController::allDown() {
    for (int i = 0; i < TOTAL_SERVOS; i++) {
        setServoPosition(i, SERVO_DOWN_POSITION);
    }
}

void ServoController::allUp() {
    for (int i = 0; i < TOTAL_SERVOS; i++) {
        setServoPosition(i, SERVO_UP_POSITION);
    }
}