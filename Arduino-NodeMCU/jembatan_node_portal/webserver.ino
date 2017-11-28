void webserver_start(){
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

  webserver.begin();
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  webserver.on("/", handleRoot);
  webserver.on("/inline", [](){
    webserver.send(200, "text/plain", "jembatan_portal_node");
  });
  webserver.onNotFound(handleNotFound);
  webserver.begin();
}

void handleRoot() {
  webserver.send(200, "text/plain", "jembatan_portal_node");
}

void handleNotFound(){
  if (webserver.uri().substring(1)=="reset"){
    webserver.send(200, "text/plain", "reset");
    lakukan_reset();
  } 
  
  else if (webserver.uri().substring(1)=="lolos"){
    webserver.send(200, "text/plain", "lolos");
    lakukan_ya();
  } 
  
  else if (webserver.uri().substring(1)=="tilang"){
    webserver.send(200, "text/plain", "tilang");
    lakukan_tidak();
  }
  
  else {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += webserver.uri();
    message += "\nMethod: ";
    message += (webserver.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += webserver.args();
    message += "\n";
    for (uint8_t i=0; i<webserver.args(); i++){
      message += " " + webserver.argName(i) + ": " + webserver.arg(i) + "\n";
    }
    webserver.send(404, "text/plain", message);
  }
}

