void handleNotFound(){
  if (webserver.uri().substring(1)=="reset"){
    webserver.send(200, "text/plain", "reset");
    lakukan_reset();
  } 
  
  else if (webserver.uri().substring(1)=="ya"){
    webserver.send(200, "text/plain", "ya");
    lakukan_ya();
  } 
  
  else if (webserver.uri().substring(1)=="tidak"){
    webserver.send(200, "text/plain", "tidak");
    lakukan_tidak();
  }

  else if (webserver.uri().substring(1)=="baca"){
    webserver.send(200, "text/plain", "baca");
    moco=1;
  }

  else if (webserver.uri().substring(1)=="sensor"){
    if (t1>2) sensor_1=1;
    else sensor_1=0;
    if (t2>2) sensor_2=1;
    else sensor_2=0;
    if (t3>2) sensor_3=1;
    else sensor_3=0;
    
    webserver.send(200, "text/plain",  "a" + String(sensor_1) + "b" + String(sensor_2) + "c" + String(sensor_3) + "d");
    moco=0;
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
