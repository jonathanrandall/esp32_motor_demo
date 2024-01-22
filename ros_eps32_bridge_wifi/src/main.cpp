#include <Arduino.h>
#include "motor_driver_stuff.h"
#include "commands.h"
#include "pid_stuff.h"
#include "encoder_stuff.h"
#include "webserver_stuff.h"



int arg = 0;
int idx = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

long arg1;
long arg2;

// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];




void resetCommand()
{
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  idx = 0;
}



void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("begun");

  init_wifi();

  robot_setup();
  resetPID();
  init_encoders();
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (millis() > nextPID)
  {
    updatePID();
    nextPID =millis()+ PID_INTERVAL;
  }
  delay(2);
  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL)
  {
    
    setMotorSpeeds(0, 0);
    moving = 0;
  }
}
