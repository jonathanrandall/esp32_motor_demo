
#ifndef webserver_stuff_
#define webserver_stuff_

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "encoder_stuff.h"
#include "motor_driver_stuff.h"
#include "pid_stuff.h"

// Replace with your network credentials
const char *ssid = "";
const char *password = "";

#define AUTO_STOP_INTERVAL 2000
long lastMotorCommand = AUTO_STOP_INTERVAL;

/// Added static IP
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 211);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

AsyncWebServer server(80);

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
  else if (!strcmp(variable.c_str(), "m"))
  {
    lastMotorCommand = millis();
    resetPID();
    moving = 0; // Sneaky way to temporarily disable the PID
    
    setMotorSpeeds(arg1, arg2);
    resp = String(tmp1) + " " + String(tmp2);
  }


  request->send(200, "text/plain", resp);
}


#endif