void webServerBegin(){
  webserver.begin();
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  webserver.on("/", handleRoot);
  webserver.on("/inline", [](){
    webserver.send(200, "text/plain", "jembatan_sensor_node");
  });
  webserver.onNotFound(handleNotFound);
  webserver.begin();
}

