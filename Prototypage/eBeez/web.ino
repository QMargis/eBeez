String processor(const String& var)
{
  if(var == "ALLOWED_EXTENSIONS_EDIT") { return allowedExtensionsForEdit; }
  if(var == "SPIFFS_FREE_BYTES") { return convertFileSize((SPIFFS.totalBytes() - SPIFFS.usedBytes())); }
  if(var == "SPIFFS_USED_BYTES") { return convertFileSize(SPIFFS.usedBytes()); }
  if(var == "SPIFFS_TOTAL_BYTES") { return convertFileSize(SPIFFS.totalBytes());  }
  if(var == "LISTEN_FILES") { return listDir(SPIFFS, "/", 0); }
  if(var == "EDIT_FILES")
  {
    String editDropdown = "<select name=\"edit_path\" id=\"edit_path\">";
    editDropdown += "<option value=\"choose\">Select file to edit</option>";
    editDropdown += "<option value=\"new\">New text file</option>";
    editDropdown += filesDropdownOptions;      
    editDropdown += "</select>";
    return editDropdown;
  }
  if(var == "DELETE_FILES")
  {
    String deleteDropdown = "<select name=\"delete_path\" id=\"delete_path\">";
    deleteDropdown += "<option value=\"choose\">Select file to delete</option>";
    deleteDropdown += filesDropdownOptions;      
    deleteDropdown += "</select>";
    return deleteDropdown;
  }
  if(var == "TEXTAREA_CONTENT") { return textareaContent; }
  if(var == "SAVE_PATH_INPUT")
  {
    if(savePath == "/new.txt") { savePathInput = "<input type=\"text\" id=\"save_path\" name=\"save_path\" value=\"" + savePath + "\" >"; }
    else { savePathInput = ""; }
    return savePathInput;
  }

  return String();
}


/* setup function */
void setupOTA(void) {
  TryToConnectWifi();

  // LITTLEFS
  Serial.println("---- LITTLEFS ----");
  if (!SPIFFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LITTLEFS Mount Failed");
    return;
  } else { Serial.println("LITTLEFS OK !!"); }

  // Fichiers
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/index.html", "text/html"); });
  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/w3.css", "text/css"); });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/script.js", "text/javascript"); });
  server.on("/jquery-3.7.1.min.js", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/jquery-3.7.1.min.js", "text/javascript"); });
  server.on("/transfert.html", HTTP_GET, [](AsyncWebServerRequest *request) { request->send(SPIFFS, "/transfert.html", "text/html"); });

  // Fonctions pour le JS
  server.on("/params", HTTP_POST, [](AsyncWebServerRequest *request) {
    String mVeille = "";
    mVeille = request->getParam("ModeVeille", true)->value();

    if(request->getParam("webLogin", true)->value() != "") { webLogin = request->getParam("webLogin", true)->value(); }
    if(request->getParam("webPass", true)->value() != "") { webPass = request->getParam("webPass", true)->value(); }
    if(request->getParam("rucheName", true)->value() != "") { rucheName = request->getParam("rucheName", true)->value(); }
    if(mVeille == "true") { modeVeille=true; } else if(mVeille == "false") { modeVeille=false; } 
    if(request->getParam("iTimeToSleep", true)->value() != "") { iTimeToSleep = request->getParam("iTimeToSleep", true)->value().toInt(); }
    
    Serial.print("modeVeille:");
    Serial.println(modeVeille);

    prefs.begin(ebeezSpace, ReadWrite);

    // Mise en eeprom des valeurs
    const size_t putWebLogin = prefs.putString("webLogin", webLogin); 
    if (putWebLogin == 0) { Serial.println("Preferences: Could not write webLogin to nvs. No reboot"); }
    else { Serial.print("webLogin:"); Serial.println(webLogin);}
    vTaskDelay(1);

    const size_t putWebPass = prefs.putString("webPass", webPass); 
    if (putWebPass == 0) { Serial.println("Preferences: Could not write webPass to nvs. No reboot");  }
    else {Serial.print("webPass:"); Serial.println(webPass);}
    vTaskDelay(1);

    const size_t putRucheName = prefs.putString("rucheName", rucheName); 
    if (putRucheName == 0) { Serial.println("Preferences: Could not write rucheName to nvs. No reboot");  }
    else {Serial.print("rucheName:"); Serial.println(rucheName);}
    vTaskDelay(1);

    const size_t putModeVeille = prefs.putBool("modeVeille", modeVeille); 
    if (putModeVeille == 0) { Serial.println("Preferences: Could not write modeVeille to nvs. No reboot");  }
    else {Serial.print("modeVeille:"); Serial.println(modeVeille);}
    vTaskDelay(1);

    const size_t putiTimeToSleep = prefs.putInt("iTimeToSleep", iTimeToSleep); 
    if (putiTimeToSleep == 0) { Serial.println("Preferences: Could not write iTimeToSleep to nvs. No reboot");  }
    else {Serial.print("iTimeToSleep:"); Serial.println(iTimeToSleep);}
    vTaskDelay(1);

    prefs.end();
    request->send(204);
  });

  server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->getParam("wifi_mode", true)->value() != "") { wifi_mode = request->getParam("wifi_mode", true)->value(); }
    if(request->getParam("ssid_ap", true)->value() != "") { ssid_ap = request->getParam("ssid_ap", true)->value(); }
    if(request->getParam("pass_ap", true)->value() != "") { pass_ap = request->getParam("pass_ap", true)->value(); }
    if(request->getParam("ssid_client", true)->value() != "") { ssid_client = request->getParam("ssid_client", true)->value(); }
    if(request->getParam("pass_client", true)->value() != "") { pass_client = request->getParam("pass_client", true)->value(); }

    prefs.begin(ebeezSpace, ReadWrite);

    // Mise en eeprom des valeurs
    const size_t putWifiMode = prefs.putString("modWifi", wifi_mode); 
    if (putWifiMode == 0) { Serial.println("Preferences: Could not write modWifi to nvs. No reboot"); }
    else { Serial.print("modWifi:"); Serial.println(wifi_mode);}
    vTaskDelay(1);

    const size_t putSsidAp = prefs.putString("ssid_ap", ssid_ap); 
    if (putSsidAp == 0) { Serial.println("Preferences: Could not write ssid_ap to nvs. No reboot");  }
    else {Serial.print("ssid_ap:"); Serial.println(ssid_ap);}
    vTaskDelay(1);

    const size_t putPassAp = prefs.putString("pass_ap", pass_ap); 
    if (putPassAp == 0) { Serial.println("Preferences: Could not write pass_ap to nvs. No reboot");  }
    else {Serial.print("pass_ap:"); Serial.println(pass_ap);}
    vTaskDelay(1);

    const size_t putSsidClient = prefs.putString("ssid_client", ssid_client); 
    if (putSsidClient == 0) { Serial.println("Preferences: Could not write ssid_client to nvs. No reboot");  }
    else {Serial.print("ssid_client:"); Serial.println(ssid_client);}
    vTaskDelay(1);

    const size_t putPassClient = prefs.putString("pass_client", pass_client); 
    if (putPassClient == 0) { Serial.println("Preferences: Could not write pass_client to nvs. No reboot");  }
    else {Serial.print("pass_client:"); Serial.println(pass_client);}
    vTaskDelay(1);

    prefs.end();
    request->send(204);
  });

  server.on("/mqtt", HTTP_POST, [](AsyncWebServerRequest *request) {
    if(request->getParam("mqtt_serveur", true)->value() != "") { mqtt_serveur = request->getParam("mqtt_serveur", true)->value(); }
    if(request->getParam("mqtt_port", true)->value() != "") { mqtt_port = request->getParam("mqtt_port", true)->value().toInt(); }
    if(request->getParam("mqtt_user", true)->value() != "") { mqtt_user = request->getParam("mqtt_user", true)->value(); }
    if(request->getParam("mqtt_pass", true)->value() != "") { mqtt_pass = request->getParam("mqtt_pass", true)->value(); }

    prefs.begin(ebeezSpace, ReadWrite);

    // Mise en eeprom des valeurs
    const size_t putMqttServeur = prefs.putString("mqtt_serveur", mqtt_serveur); 
    if (putMqttServeur == 0) { Serial.println("Preferences: Could not write mqtt_serveur to nvs. No reboot"); }
    else { Serial.print("mqtt_serveur:"); Serial.println(mqtt_serveur);}
    vTaskDelay(1);

    const size_t putMqttPort = prefs.putInt("mqtt_port", mqtt_port); 
    if (putMqttPort == 0) { Serial.println("Preferences: Could not write mqtt_port to nvs. No reboot");  }
    else {Serial.print("mqtt_port:"); Serial.println(mqtt_port);}
    vTaskDelay(1);

    const size_t putMqttUser = prefs.putString("mqtt_user", mqtt_user); 
    if (putMqttUser == 0) { Serial.println("Preferences: Could not write mqtt_user to nvs. No reboot");  }
    else {Serial.print("mqtt_user:"); Serial.println(mqtt_user);}
    vTaskDelay(1);

    const size_t putMqttPass = prefs.putString("mqtt_pass", mqtt_pass); 
    if (putMqttPass == 0) { Serial.println("Preferences: Could not write mqtt_pass to nvs. No reboot");  }
    else {Serial.print("mqtt_pass:"); Serial.println(mqtt_pass);}
    vTaskDelay(1);

    prefs.end();
    request->send(204);
  });

  server.on("/getData", HTTP_GET, [](AsyncWebServerRequest *request) {
    String mVeille;
    if(modeVeille == true) { mVeille="true"; } else if(modeVeille == false) { mVeille=false; } 
    String data = webLogin+"&"+webPass+"&"+rucheName+"&"+mVeille+"&"+iTimeToSleep+"&"+wifi_mode+"&"+ssid_ap+"&"+pass_ap+"&"+ssid_client+"&"+pass_client+"&"+fAjustement+"&"+lValCalibLoadCPT1+"&"+lValCalibLoadCPT2+"&"+lValCalibLoadCPT3+"&"+lValCalibLoadCPT4+"&"+mqtt_serveur+"&"+mqtt_port+"&"+mqtt_user+"&"+mqtt_pass;
    vTaskDelay(1);
    request->send(200, "text/plain", data);
  });

  server.on("/calib", HTTP_POST, [](AsyncWebServerRequest *request) {

    if(request->getParam("fAjustement", true)->value() != "") { fAjustement = request->getParam("fAjustement", true)->value().toFloat(); }
    prefs.begin(ebeezSpace, ReadWrite);

    // Mise en eeprom de la valriable fAjustement
    const size_t putfAjustement = prefs.putFloat("fAjustement", fAjustement); 
    if (putfAjustement == 0) { Serial.println("Preferences: Could not write fAjustement to nvs. No reboot"); }
    else { Serial.print("fAjustement:"); Serial.println(fAjustement);}
    vTaskDelay(1);

    // Lancement de la calibration
    LoadCalibrationAllHX711();

    const size_t putCal1 = prefs.putLong("ValCalCPT1", lValCalibLoadCPT1); 
    if (putCal1 == 0) { Serial.println("Preferences: Could not write lValCalibLoadCPT1 to nvs. No reboot");  }
    else {Serial.print("lValCalibLoadCPT1:"); Serial.println(lValCalibLoadCPT1);}
    vTaskDelay(1);

    const size_t putCal2 = prefs.putLong("ValCalCPT2", lValCalibLoadCPT2); 
    if (putCal2 == 0) { Serial.println("Preferences: Could not write lValCalibLoadCPT2 to nvs. No reboot");  }
    else {Serial.print("lValCalibLoadCPT2:"); Serial.println(lValCalibLoadCPT2);}
    vTaskDelay(1);

    const size_t putCal3 = prefs.putLong("ValCalCPT3", lValCalibLoadCPT3); 
    if (putCal3 == 0) { Serial.println("Preferences: Could not write lValCalibLoadCPT3 to nvs. No reboot");  }
    else {Serial.print("lValCalibLoadCPT3:"); Serial.println(lValCalibLoadCPT3);}
    vTaskDelay(1);

    const size_t putCal4 = prefs.putLong("ValCalCPT4", lValCalibLoadCPT4); 
    if (putCal4 == 0) { Serial.println("Preferences: Could not write lValCalibLoadCPT4 to nvs. No reboot");  }
    else {Serial.print("lValCalibLoadCPT4:"); Serial.println(lValCalibLoadCPT4);} 
    vTaskDelay(1);   

    prefs.end();
    request->send(204);
  });

  server.on("/reboot", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("reboot");
    prefs.end();
    ESP.restart();
  });

  server.on("/manager", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    if(!request->authenticate(webLogin.c_str(), webPass.c_str())) { return request->requestAuthentication(); }
    request->send_P(200, "text/html", manager_html, processor);
  });

  server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) 
  {
    request->send(204);
  }, uploadFile);

  server.on("/edit", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    //if(!request->authenticate(webLogin.c_str(), webPass.c_str())) { return request->requestAuthentication(); }

    String inputMessage = request->getParam(param_edit_path)->value();

    if(inputMessage =="new")
    {
      textareaContent = "";
      savePath = "/new.txt";
    }
    else
    {
      savePath = "/"+inputMessage;
      textareaContent = readFile(SPIFFS, savePath.c_str());
    }
    request->send_P(200, "text/html", edit_html, processor);
  });

  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    //if(!request->authenticate(webLogin.c_str(), webPass.c_str())) { return request->requestAuthentication(); }

    String inputMessage = "";
    if (request->hasParam(param_edit_textarea)) { inputMessage = request->getParam(param_edit_textarea)->value(); }
    if (request->hasParam(param_save_path)) { savePath = request->getParam(param_save_path)->value(); }
    writeFile(SPIFFS, savePath.c_str(), inputMessage.c_str());
    request->redirect("/manager");
  });

  server.on("/delete", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    //if(!request->authenticate(webLogin.c_str(), webPass.c_str())) { return request->requestAuthentication(); }

    String inputMessage = request->getParam(param_delete_path)->value();
    if(inputMessage !="choose") { SPIFFS.remove("/"+inputMessage);  }
    request->redirect("/manager");
  });

  // OTA
  //AsyncElegantOTA.begin(&server);  // Start ElegantOTA
  ElegantOTA.begin(&server);  // Start ElegantOTA

  // ERREUR 404
  server.onNotFound(notFound);

  server.begin();
}

void notFound(AsyncWebServerRequest *request) { request->send(404, "text/plain", "Page not found"); }

void TryToConnectWifi() {
  uint8_t CountTrytoConnect = 0;
  // -------------Connect to Wi-Fi 1 ----------------------
  // Set Access Point configuration
  if (wifi_mode == "AP") { WiFi.mode(WIFI_AP); }
  else if (wifi_mode == "CLIENT") { WiFi.mode(WIFI_STA); }
  else if (wifi_mode == "DOUBLE") { WiFi.mode(WIFI_AP_STA); }
  else { WiFi.mode(WIFI_AP); }

  if (wifi_mode == "AP" || wifi_mode == "DOUBLE") {
    WiFi.softAP(ssid_ap, pass_ap);
    Serial.println("------------------------------------");
    Serial.print("SSID AP : ");
    Serial.println(ssid_ap);
    Serial.print("Password AP : ");
    Serial.println(pass_ap);
    Serial.print("IP address : ");
    Serial.println(WiFi.softAPIP());
    Serial.println("------------------------------------");
  }
  if(wifi_mode == "CLIENT" || wifi_mode == "DOUBLE") {
    WiFi.begin(ssid_client, pass_client);
    Serial.print(F("Tentative de connexion au Réseau WiFi"));
    while ((WiFi.status() != WL_CONNECTED) && (CountTrytoConnect < 30)) {
      delay(500);
      Serial.print(F("."));
      ++CountTrytoConnect;
    }
    if (CountTrytoConnect >= 20) {
      Serial.print(ssid_client);
      Serial.println("Non connecté !");
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print(ssid_client);
      Serial.println("Connecté !");
    }
    /*
    * Ajout d'une information d'erreur de connection
    */
    Serial.println("------------------------------------");
    Serial.print("SSID Client : ");
    Serial.println(ssid_client);
    Serial.print("IP address Client : ");
    Serial.println(WiFi.localIP());
    Serial.print("webLogin : "); Serial.println(webLogin);
    Serial.print("webPass : "); Serial.println(webPass);
    Serial.println("------------------------------------");
  }
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);
}

void WifiReconnect() {
  unsigned long currentMillis = millis();
  if ((WiFi.status() != WL_CONNECTED) && (currentMillis - previousMillis >= lInterval)) {
    WiFi.disconnect();
    WiFi.reconnect();
    ++iNbReconnections;
    previousMillis = currentMillis;
  }
}
