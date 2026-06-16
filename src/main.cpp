#include <Arduino.h>
#include "Arduino.h"
#include <ArduinoWebsockets.h>

#if defined(ESP32)
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <ESPAsyncWebServer.h>

#include "config.h"

using namespace websockets;
WebsocketsServer server;
AsyncWebServer webserver(80);

int LValue, RValue, commaIndex;

void setFullSpeedDirFromSigned(int speed_dir_A, int speed_dir_B) { 
  
  if (speed_dir_A>0){
    digitalWrite(pwm_A, HIGH); 
    digitalWrite(dir_A, HIGH ); 
  } else if (speed_dir_A<0){
    digitalWrite(pwm_A, HIGH); 
    digitalWrite(dir_A, LOW); 
  }
  else{
    digitalWrite(pwm_A, LOW); 
  }

  if (speed_dir_B>0){
    digitalWrite(pwm_B, HIGH); 
    digitalWrite(dir_B, HIGH); 
  } else if (speed_dir_B<0){
    digitalWrite(pwm_B, HIGH); 
    digitalWrite(dir_B, LOW); 
  } else{
    digitalWrite(pwm_B, LOW); 
  }
}

void setSpeedDirFromSigned(int speed_dir_A, int speed_dir_B) { 
  //Serial.println("Speed A: " + String(speed_dir_A) + ", Speed B: " + String(speed_dir_B));
  if (speed_dir_A>0){
    analogWrite(pwm_A, speed_dir_A); 
    digitalWrite(dir_A, HIGH ); 
  } else if (speed_dir_A<0){
    analogWrite(pwm_A, abs(speed_dir_A)); 
    digitalWrite(dir_A, LOW); 
  }
  else{
    digitalWrite(pwm_A, LOW); 
  }

  if (speed_dir_B>0){
    analogWrite(pwm_B, speed_dir_B); 
    digitalWrite(dir_B, HIGH); 
  } else if (speed_dir_B<0){
    analogWrite(pwm_B, abs (speed_dir_B)); 
    digitalWrite(dir_B, LOW); 
  } else{
    digitalWrite(pwm_B, LOW); 
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");
  // Mount 
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Create a unique wifi ssid
  String chipId = String( ESP.getChipId());
  String ssid = ssid_base + chipId;

  // Create AP
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("Wifi Name: " + ssid);
  Serial.println("Password: " + String(password) );
  Serial.print("AP IP address: ");
  Serial.println(IP);

  webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  webserver.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  webserver.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/javascript");
  });  

  webserver.on("/joy.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/joy.js", "text/javascript");
  });  


  // start server
  webserver.begin();
  server.listen(82);

  // Verbose
  Serial.print("Server status: ");
  Serial.println(server.available());
 
}
 
// handle http messages
void handle_message(WebsocketsMessage msg) {
  commaIndex = msg.data().indexOf(',');
  LValue = msg.data().substring(0, commaIndex).toInt();
  RValue = msg.data().substring(commaIndex + 1).toInt();
  Serial.println("L: "+ String(LValue) + " R: " + String(RValue) );
  setSpeedDirFromSigned(LValue, RValue);
}
 
void loop()
{
  auto client = server.accept();
  client.onMessage(handle_message);
  while (client.available()) {
    client.poll();
  }
}


