#include <Arduino.h>
#include "motor_driver_stuff.h"
#include "commands.h"
#include "pid_stuff.h"
#include "encoder_stuff.h"
#include "webserver_stuff.h"



#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

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

int runCommand()
{
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  // Serial.println(cmd);

  switch (cmd)
  {
  case GET_BAUDRATE:
    Serial.println(BAUDRATE);
    break;
  case ANALOG_READ:
    // Serial.println(analogRead(arg1));
    break;
  case DIGITAL_READ:
    Serial.println(digitalRead(arg1));
    break;
  case ANALOG_WRITE:
    // analogWrite(arg1, arg2);
    Serial.println("OK");
    break;
  case DIGITAL_WRITE:
    if (arg2 == 0)
      digitalWrite(arg1, LOW);
    else if (arg2 == 1)
      digitalWrite(arg1, HIGH);
    Serial.println("OK");
    break;
  case PIN_MODE:
    if (arg2 == 0)
      pinMode(arg1, INPUT);
    else if (arg2 == 1)
      pinMode(arg1, OUTPUT);
    Serial.println("OK");
    break;
  case READ_ENCODERS:
    Serial.print(readEncoder(LEFT));
    Serial.print(" ");
    Serial.println(readEncoder(RIGHT));
    break;
  case RESET_ENCODERS:
    resetEncoders();
    resetPID();
    Serial.println("OK");
    break;
  case MOTOR_SPEEDS:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    if (arg1 == 0 && arg2 == 0)
    {
      setMotorSpeeds(0, 0);
      resetPID();
      moving = 0;
    }
    else
      moving = 1;
    leftPID.TargetTicksPerFrame = arg1;
    rightPID.TargetTicksPerFrame = arg2;
    Serial.println("OK");
    break;
  case MOTOR_RAW_PWM:
    /* Reset the auto stop timer */
    lastMotorCommand = millis();
    resetPID();
    moving = 0; // Sneaky way to temporarily disable the PID
    // Serial.println(arg1);
    // Serial.println(arg2);
    setMotorSpeeds(arg1, arg2);
    Serial.println("OK");
    break;
  case UPDATE_PID:
    while ((str = strtok_r(p, ":", &p)) != NULL)
    {
      pid_args[i] = atoi(str);
      i++;
    }
    Kp = pid_args[0];
    Kd = pid_args[1];
    Ki = pid_args[2];
    Ko = pid_args[3];
    Serial.println("OK");
    break;
  default:
    Serial.println("Invalid Command");
    break;
  }

  return 1;
}


void handleRequest(AsyncWebServerRequest *request)
{
  unsigned long int tmp1, tmp2;
  tmp1 = millis();
  /// control?var=variable&val=10_20

  String variable = request->arg("var");
  String valValue = request->arg("val");

  // Parse the string into two integers
  int arg1, arg2;
  sscanf(valValue.c_str(), "%d_%d", &arg1, &arg2);

  Serial.println(variable);
  Serial.print("Received values: ");
  Serial.print("arg1 = ");
  Serial.print(arg1);
  Serial.print(", arg2 = ");
  Serial.println(arg2);

  // because variable is type string and strcmp expects const char *
  // we need to use variable.c_str()
  String resp;
  tmp2 = millis();
  if (!strcmp(variable.c_str(), "o"))
  {
    resp = String(arg1 + 10) + " " + String(arg2 + 100);
    // 20 120
  }
  else if (!strcmp(variable.c_str(), "e"))
  {
    resp = String(tmp1) + " " + String(tmp2);
  }

  request->send(200, "text/plain", resp);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("begun");

  robot_setup();
  resetPID();
  init_encoders();
}

void loop()
{
  // put your main code here, to run repeatedly:
  while (Serial.available() > 0)
  {
    chr = Serial.read();
    // Serial.println(chr);

    // Terminate a command with a CR
    if (chr == 13)
    {
      // Serial.println("here");
      if (arg == 1)
        argv1[idx] = NULL;
      else if (arg == 2)
        argv2[idx] = NULL;
      // Serial.println(cmd);
      runCommand();
      // Serial.println("reseting");
      resetCommand();
    }
    else if (chr == ' ')
    {
      // Serial.println("here now");
      // Step through the arguments
      if (arg == 0)
        arg = 1;
      else if (arg == 1)
      {
        argv1[idx] = NULL;
        arg = 2;
        idx = 0;
      }
      continue;
    }
    else
    {
      if (arg == 0)
      {
        // The first arg is the single-letter command
        cmd = chr;
      }
      else if (arg == 1)
      {
        // Subsequent arguments can be more than one character
        argv1[idx] = chr;
        idx++;
      }
      else if (arg == 2)
      {
        argv2[idx] = chr;
        idx++;
      }
    }
  }

  // If we are using base control, run a PID calculation at the appropriate intervals

  if (millis() > nextPID)
  {
    updatePID();
    nextPID += PID_INTERVAL;
  }

  // Check to see if we have exceeded the auto-stop interval
  if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL)
  {
    ;
    setMotorSpeeds(0, 0);
    moving = 0;
  }
}
