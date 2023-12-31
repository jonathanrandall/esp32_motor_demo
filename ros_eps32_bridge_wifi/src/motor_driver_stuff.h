#ifndef MOTORS_STUFF_
#define MOTORS_STUFF_
#include <Arduino.h>
#include "commands.h"
// pins
#define LEFT_MOTOR_PWM 25
#define RIGHT_MOTOR_PWM 27
#define RIGHT_MOTOR_DIR 12
#define LEFT_MOTOR_DIR 26

// pwm paramters
const int mtr_left_pwm_channel = 8;
const int mtr_right_pwm_channel = 4;
const int lresolution = 8;
const int freq = 4000;

void robot_setup()
{
  // Pins for Motor Controller
  pinMode(LEFT_MOTOR_DIR, OUTPUT);
  pinMode(LEFT_MOTOR_PWM, OUTPUT);
  pinMode(RIGHT_MOTOR_DIR, OUTPUT);
  pinMode(RIGHT_MOTOR_PWM, OUTPUT);

  // Motor uses PWM Channel 8
  ledcSetup(mtr_left_pwm_channel, freq, lresolution);
  ledcAttachPin(LEFT_MOTOR_PWM, mtr_left_pwm_channel);
  ledcWrite(mtr_left_pwm_channel, 0);

  ledcSetup(mtr_right_pwm_channel, freq, lresolution);
  ledcAttachPin(RIGHT_MOTOR_PWM, mtr_right_pwm_channel);
  ledcWrite(mtr_right_pwm_channel, 0);
}

void setMotorSpeed(int i, int spd)
{
  unsigned char reverse = 0;

  if (spd < 0)
  {
    spd = -spd;
    reverse = 1;
  }
  if (spd > 255)
    spd = 255;
  // note left and right motors facing in opposite directions.
  if (i == LEFT)
  {
    if (reverse == 0)
    {
      digitalWrite(LEFT_MOTOR_DIR, 1);
    }
    else if (reverse == 1)
    {
      digitalWrite(LEFT_MOTOR_DIR, 0);
    }
    ledcWrite(mtr_left_pwm_channel, spd);
    // if (spd > 0)
    // {
    //   Serial.print("left motor: ");
    //   Serial.println(spd);
    // }
  }
  else /*if (i == RIGHT) //no need for condition*/
  {
    if (reverse == 0)
    {
      digitalWrite(RIGHT_MOTOR_DIR, 0);
    }
    else if (reverse == 1)
    {
      digitalWrite(RIGHT_MOTOR_DIR, 1);
    }
    ledcWrite(mtr_right_pwm_channel, spd);
    // if (spd > 0)
    // {
    //   Serial.print("right motor: ");
    //   Serial.println(spd);
    // }
  }
}

void setMotorSpeeds(int leftSpeed, int rightSpeed)
{
  // setMotorSpeed(LEFT, leftSpeed);
  //  ledcWrite(mtr_right_pwm_channel, rightSpeed);
  //  ledcWrite(mtr_left_pwm_channel, leftSpeed);
  setMotorSpeed(RIGHT, rightSpeed);
  setMotorSpeed(LEFT, leftSpeed);
  // ledcWrite(mtr_right_pwm_channel, rightSpeed);
  // ledcWrite(mtr_left_pwm_channel, leftSpeed);
}

#endif