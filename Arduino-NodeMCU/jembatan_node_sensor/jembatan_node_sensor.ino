#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "Jembatan AP";
const char* password = "24242424";
const char* WiFi_hostname = "Jembatan-Sensor-Node";

IPAddress ip(192, 168, 1, 100);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

int moco=0;
int sensor_1,sensor_2,sensor_3;
int t1,t2,t3;

ESP8266WebServer webserver(80);
double last2;

void setup() {
  Serial.begin(115200);
  delay(10);
  
  Serial.print("Connecting to ");
  Serial.println(ssid);

  
  WiFi.hostname(WiFi_hostname);
  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  pinMode(16, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(0, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  digitalWrite(16, 0);
  digitalWrite(5, 0);
  digitalWrite(4, 0);
  //digitalWrite(3, 0);
  //digitalWrite(4, 0);
  //digitalWrite(5, 0);
  
  webServerBegin();
}

void loop() { 
  lakukan();
  cekSensor();
  webserver.handleClient();
}
