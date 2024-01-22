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
  lastMotorCommand = millis();

  init_wifi();

  robot_setup();
  resetPID();
  init_encoders();


  leftPID.nm = "left";
  rightPID.nm = "right";

  leftPID.TargetTicksPerFrame = 120;
  rightPID.TargetTicksPerFrame = 120;
  moving = 1;
}

void loop()
{
  // put your main code here, to run repeatedly:
  if ((millis() > nextPID))
  {
    updatePID();
    nextPID =millis()+ PID_INTERVAL;
  }
  delay(2);
<<<<<<< HEAD
=======

>>>>>>> be610b04e16e62b780a15877dc335f6db23eb2eb
  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > 10000)
  {
    Serial.println(lastMotorCommand);
    Serial.println(millis());
    Serial.println(" stopping motor for time out setting moving = 0");
    
    setMotorSpeeds(0, 0);
    // init_encoders();
    moving = 0;
    lastMotorCommand = millis();
  } 

  // for (int i=140; i<201;i=i+60){
  //   setMotorSpeeds(i,i);
  //   delay(3);
  // }

  // for (int i=201; i>140;i=i-60){
  //   setMotorSpeeds(i,i);
  //   delay(5);
  // }


  
}
