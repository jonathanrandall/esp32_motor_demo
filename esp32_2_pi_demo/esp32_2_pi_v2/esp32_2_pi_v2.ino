#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "";
const char* password = "";


///Added static IP
// Set your Static IP address
IPAddress local_IP(192, 168, 1, 211);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

AsyncWebServer server(80);

void handleRequest(AsyncWebServerRequest *request) {
  unsigned long int tmp1, tmp2;
  tmp1 = millis();
  ///control?var=variable&val=10_20

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
  if(!strcmp(variable.c_str(), "o")){
    resp = String(arg1+10) + " " + String(arg2+100);
    //20 120
  }
  else if(!strcmp(variable.c_str(), "e")){
    resp = String(tmp1) + " " + String(tmp2);
  }


  request->send(200, "text/plain", resp);
}

void setup() {
  Serial.begin(115200);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.mode(WIFI_AP_STA); //need this mode to use esp now
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
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

void loop() {
  // Your code here
}
