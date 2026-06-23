#include <Arduino.h>
#include "Arduino.h"
#include <ArduinoWebsockets.h>
#include <ArduinoOTA.h>

#if defined(ESP32)
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#include "config.h"

using namespace websockets;
WebsocketsServer server;
AsyncWebServer webserver(80);

int forward;
int strafe;
int rotate;
int aux;

int motorFL;
int motorFR;
int motorRL;
int motorRR;

const bool invertFL = false;
const bool invertFR = false;
const bool invertRL = false;
const bool invertRR = false;

void setMotor(int pwmPin, int dirPin, int value)
{
    value = constrain(value, -255, 255);

    if (value > 0)
    {
        analogWrite(pwmPin, value);
        digitalWrite(dirPin, HIGH);
    }
    else if (value < 0)
    {
        analogWrite(pwmPin, abs(value));
        digitalWrite(dirPin, LOW);
    }
    else
    {
        analogWrite(pwmPin, 0);
    }
}

void driveMecanum(int forward, int strafe, int rotate)
{
    motorFL = forward + strafe + rotate;
    motorFR = forward - strafe - rotate;
    motorRL = forward - strafe + rotate;
    motorRR = forward + strafe - rotate;

    int maxValue = max(
        max(abs(motorFL), abs(motorFR)),
        max(abs(motorRL), abs(motorRR))
    );

    if (maxValue > 100)
    {
        motorFL = motorFL * 100 / maxValue;
        motorFR = motorFR * 100 / maxValue;
        motorRL = motorRL * 100 / maxValue;
        motorRR = motorRR * 100 / maxValue;
    }

    motorFL = map(motorFL, -100, 100, -255, 255);
    motorFR = map(motorFR, -100, 100, -255, 255);
    motorRL = map(motorRL, -100, 100, -255, 255);
    motorRR = map(motorRR, -100, 100, -255, 255);

    if(invertFL) motorFL = -motorFL;
    if(invertFR) motorFR = -motorFR;
    if(invertRL) motorRL = -motorRL;
    if(invertRR) motorRR = -motorRR;


    setMotor(pwm_FL, dir_FL, motorFL);
    setMotor(pwm_FR, dir_FR, motorFR);
    setMotor(pwm_RL, dir_RL, motorRL);
    setMotor(pwm_RR, dir_RR, motorRR);

    Serial.printf(
        "FL[pwm=%d dir=%d]=%4d  "
        "FR[pwm=%d dir=%d]=%4d  "
        "RL[pwm=%d dir=%d]=%4d  "
        "RR[pwm=%d dir=%d]=%4d\n",

        pwm_FL, dir_FL, motorFL,
        pwm_FR, dir_FR, motorFR,
        pwm_RL, dir_RL, motorRL,
        pwm_RR, dir_RR, motorRR
    );
}

void setup()
{
  Serial.begin(115200);
  Serial.println("");

  ESP.wdtEnable(5000);
  // Mount FS
  if (!LittleFS.begin())
  {
      Serial.println("LittleFS mount failed");
      return;
  }

  FSInfo fs_info;
  LittleFS.info(fs_info);

  Serial.printf(
      "LittleFS: %u / %u bytes\n",
      fs_info.usedBytes,
      fs_info.totalBytes
  );  

  pinMode(pwm_FL, OUTPUT);
  pinMode(dir_FL, OUTPUT);

  pinMode(pwm_FR, OUTPUT);
  pinMode(dir_FR, OUTPUT);

  pinMode(pwm_RL, OUTPUT);
  pinMode(dir_RL, OUTPUT);

  pinMode(pwm_RR, OUTPUT);
  pinMode(dir_RR, OUTPUT);


  // Create a unique wifi ssid
  String chipHex = String(ESP.getChipId(), HEX);
  chipHex.toUpperCase();
  String ssid = String(ssid_base) + chipHex;

  // Create AP
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.println("Wifi Name: " + ssid);
  Serial.println("Password: " + String(password) );
  Serial.print("AP IP address: ");
  Serial.println(IP);

  ArduinoOTA.setHostname(ssid.c_str());

  ArduinoOTA.onStart([]() {
      Serial.println("OTA Start");
  });

  ArduinoOTA.onEnd([]() {
      Serial.println("\nOTA End");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf(
          "OTA Progress: %u%%\r",
          (progress * 100) / total
      );
  });

  ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf(
          "OTA Error[%u]\n",
          error
      );
  });

  ArduinoOTA.begin();

  Serial.print("Free heap: ");
  Serial.println(ESP.getFreeHeap());

  Serial.println("OTA Ready");  
  Serial.print("OTA IP: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("OTA Hostname: ");
  Serial.println(ssid);

  // Serve all files from root
  webserver.serveStatic("/", LittleFS, "/");
  webserver.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(LittleFS, "/index.html", "text/html");
  });  

  // start server
  webserver.begin();
  server.listen(82);

  // Verbose
  Serial.print("Server status: ");
  Serial.println(server.available());
 
}
 
// handle http messages
void handle_message(WebsocketsMessage msg)
{
    String data = msg.data();

    int p1 = data.indexOf(',');
    int p2 = data.indexOf(',', p1 + 1);
    int p3 = data.indexOf(',', p2 + 1);

    forward = data.substring(0, p1).toInt();
    strafe  = data.substring(p1 + 1, p2).toInt();
    rotate  = data.substring(p2 + 1, p3).toInt();
    aux     = data.substring(p3 + 1).toInt();

    driveMecanum(forward, strafe, rotate);

    Serial.printf(
        "F:%d S:%d R:%d | FL:%d FR:%d RL:%d RR:%d\n",
        forward,
        strafe,
        rotate,
        motorFL,
        motorFR,
        motorRL,
        motorRR
    );
}
 


WebsocketsClient client;

void loop()
{
    ArduinoOTA.handle();

    if (!client.available())
    {
        client = server.accept();

        if (client.available())
        {
            client.onMessage(handle_message);
            Serial.println("WS client connected");
        }
    }

    if (client.available())
    {
        client.poll();
    }

    yield();
}


