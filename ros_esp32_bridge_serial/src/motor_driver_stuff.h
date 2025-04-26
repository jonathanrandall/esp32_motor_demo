#ifndef MOTORSSTUFF
#define MOTORSSTUFF
#include <Arduino.h>
#include "commands.h"

// BTS7960 Motor Control Pins
// Left Motor
#define LEFT_MOTOR_LPWM 33  // L PWM pin (reverse)
#define LEFT_MOTOR_RPWM 32  // R PWM pin (forward)
#define LEFT_MOTOR_EN 5     // Enable pin (optional, can be tied to HIGH)

// Right Motor
#define RIGHT_MOTOR_LPWM 27 // L PWM pin (reverse)
#define RIGHT_MOTOR_RPWM 26 // R PWM pin (forward)
#define RIGHT_MOTOR_EN 25   // Enable pin (optional, can be tied to HIGH)

// PWM parameters
const int mtr_left_rpwm_channel = 5;
const int mtr_left_lpwm_channel = 6;
const int mtr_right_rpwm_channel = 7;
const int mtr_right_lpwm_channel = 8;
const int lresolution = 8;
const int freq = 4000;

void robot_setup() {
    // Configure motor control pins
    pinMode(LEFT_MOTOR_LPWM, OUTPUT);
    pinMode(LEFT_MOTOR_RPWM, OUTPUT);
    pinMode(LEFT_MOTOR_EN, OUTPUT);
    
    pinMode(RIGHT_MOTOR_LPWM, OUTPUT);
    pinMode(RIGHT_MOTOR_RPWM, OUTPUT);
    pinMode(RIGHT_MOTOR_EN, OUTPUT);

    // Set enable pins high
    digitalWrite(LEFT_MOTOR_EN, HIGH);
    digitalWrite(RIGHT_MOTOR_EN, HIGH);

    // Configure PWM channels
    ledcSetup(mtr_left_rpwm_channel, freq, lresolution);
    ledcAttachPin(LEFT_MOTOR_RPWM, mtr_left_rpwm_channel);
    ledcWrite(mtr_left_rpwm_channel, 0);

    ledcSetup(mtr_left_lpwm_channel, freq, lresolution);
    ledcAttachPin(LEFT_MOTOR_LPWM, mtr_left_lpwm_channel);
    ledcWrite(mtr_left_lpwm_channel, 0);

    ledcSetup(mtr_right_rpwm_channel, freq, lresolution);
    ledcAttachPin(RIGHT_MOTOR_RPWM, mtr_right_rpwm_channel);
    ledcWrite(mtr_right_rpwm_channel, 0);

    ledcSetup(mtr_right_lpwm_channel, freq, lresolution);
    ledcAttachPin(RIGHT_MOTOR_LPWM, mtr_right_lpwm_channel);
    ledcWrite(mtr_right_lpwm_channel, 0);
}

void setMotorSpeed(int i, int spd) {
    unsigned char reverse = 0;
    if (spd < 0) {
        spd = -spd;
        reverse = 1;
    }
    if (spd > 255) spd = 255;

    if (i == LEFT) {
        // Left motor control for BTS7960
        if (reverse) {
            // Reverse: LPWM active, RPWM inactive
            ledcWrite(mtr_left_rpwm_channel, 0);
            ledcWrite(mtr_left_lpwm_channel, spd);
        } else {
            // Forward: RPWM active, LPWM inactive
            ledcWrite(mtr_left_rpwm_channel, spd);
            ledcWrite(mtr_left_lpwm_channel, 0);
        }
    } else {  // RIGHT motor
        // Right motor control for BTS7960
        if (reverse) {
            // Reverse: LPWM active, RPWM inactive
            ledcWrite(mtr_right_rpwm_channel, 0);
            ledcWrite(mtr_right_lpwm_channel, spd);
        } else {
            // Forward: RPWM active, LPWM inactive
            ledcWrite(mtr_right_rpwm_channel, spd);
            ledcWrite(mtr_right_lpwm_channel, 0);
        }
    }
}

void setMotorSpeeds(int leftSpeed, int rightSpeed) {
    setMotorSpeed(LEFT, leftSpeed);
    setMotorSpeed(RIGHT, rightSpeed);
}

#endif