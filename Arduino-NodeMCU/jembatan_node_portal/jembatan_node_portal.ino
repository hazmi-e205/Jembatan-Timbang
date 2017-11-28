#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h> 
#include "LiquidCrystal_I2C.h"

ESP8266WebServer webserver(80);
LiquidCrystal_I2C lcd(0x3F, 16, 2);
Servo portal;

IPAddress ip(192, 168, 1, 101);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

const char* ssid = "Jembatan AP";
const char* password = "24242424";
const char* WiFi_hostname = "Jembatan-Portal-Node";

enum mode_portal { mulai = 1 , buka = 2 , tutup = 3};

void setup() {
  
  webserver_start();
  awalan();
}

void loop() {
  webserver.handleClient();
  lakukan();
}
