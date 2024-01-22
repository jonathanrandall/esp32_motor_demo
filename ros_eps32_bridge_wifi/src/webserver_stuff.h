
#ifndef webserver_stuff_
#define webserver_stuff_

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "encoder_stuff.h"
#include "motor_driver_stuff.h"
#include "pid_stuff.h"
#include "commands.h"

//wifi credentials are in here
// #include "wifi_p.h"

// Replace with your network credentials
//I've included mine in the wifi_p.h file
const char *ssid = "";
const char *password = "";

#define AUTO_STOP_INTERVAL 2000
unsigned long lastMotorCommand;

/// Added static IP
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 211);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

AsyncWebServer server(80);
void handleRequest(AsyncWebServerRequest *request);

void init_wifi()
{
    if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    {
        Serial.println("STA Failed to configure");
    }
    WiFi.mode(WIFI_AP_STA); // need this mode to use esp now

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    Serial.print("Go to: http://");
    Serial.println(WiFi.localIP());

    server.on("/control", HTTP_GET, handleRequest);

    server.begin();
}

void handleRequest(AsyncWebServerRequest *request)
{
    unsigned long int tmp1, tmp2;
    tmp1 = millis();
    /// control?var=variable&val=10_20

    String variable = request->arg("var");
    String valValue = request->arg("val");

    // Parse the string into two integers
    long arg1, arg2;
   
    sscanf(valValue.c_str(), "%d_%d", &arg1, &arg2);

    // Serial.print("value sent: ");
    // Serial.println(valValue.c_str());

    // Serial.println(variable);
    // Serial.print("Received values: ");
    // Serial.print("arg1 = ");
    // Serial.print(arg1);
    // Serial.print(", arg2 = ");
    // Serial.println(arg2);

    // because variable is type string and strcmp expects const char *
    // we need to use variable.c_str()
    String resp;
    tmp2 = millis();
    if (!strcmp(variable.c_str(), "o"))
    {
        // Serial.println("o recieved, moving set to zero");
        lastMotorCommand = millis();
        resetPID();
        moving = 0; // Sneaky way to temporarily disable the PID

        setMotorSpeeds(arg1, arg2);
        resp = String(arg1) + " " + String(arg1);
    }
    else if (!strcmp(variable.c_str(), "e"))
    {
        long lft, rgt;
        lastMotorCommand = millis();
        lft = readEncoder(LEFT);
        rgt = readEncoder(RIGHT);
        resp = String(lft) + " " + String(rgt);
    }
    else if (!strcmp(variable.c_str(), "m"))
    {
        lastMotorCommand = millis();
        if (arg1 == 0 && arg2 == 0)
        {
            setMotorSpeeds(0, 0);
            // Serial.println("calling reset PID arg1 and arg2 set to zero");
            resetPID();
            moving = 0;
        }
        else moving = 1;
        leftPID.TargetTicksPerFrame = arg1;
        rightPID.TargetTicksPerFrame = arg2;
        
        resp = String(arg1) + " " + String(arg2);
    }
    else
    {
        resp = "-1 -1";
    }

    request->send(200, "text/plain", resp);
}

#endif