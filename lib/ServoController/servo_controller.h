#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include <Adafruit_PWMServoDriver.h>

class ServoController {
public:
    ServoController();
    bool begin();
    void setServoPosition(uint8_t servoNum, uint8_t position);
    void updateBrailleDisplay(const uint8_t* braillePattern);
    void testAllServos();
    void allDown();
    void allUp();

private:
    Adafruit_PWMServoDriver pwm1;
    Adafruit_PWMServoDriver pwm2;
    uint8_t angleToPulse(uint8_t angle);
    Adafruit_PWMServoDriver* getDriverForServo(uint8_t servoNum);
    uint8_t getChannelForServo(uint8_t servoNum);
};

#endif