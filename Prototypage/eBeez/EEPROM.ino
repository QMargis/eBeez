// -------------------------------------------------------- //
// Fonctions de gestion de l'EEPROM avec la lib Preferences //
// -------------------------------------------------------- //

void setupProm()
{
  prefs.begin(ebeezSpace, ReadWrite);
  /////////////////////////////////
  // Recuperation des parametres //
  /////////////////////////////////

  // Wifi Cllient
  ssid_client       = prefs.getString("ssid_client", ssid_cl);
  pass_client       = prefs.getString("pass_client", pass_cl);

  // Wifi AP
  ssid_ap           = prefs.getString("ssid_ap", ssid_a);
  pass_ap           = prefs.getString("ssid_ap", pass_a);

  // Mode du WiFi
  wifi_mode         = prefs.getString("modWifi", wifi_m);

  // Login web
  webLogin          = prefs.getString("webLogin", webLog);
  webPass           = prefs.getString("webPass",webPa);

  // Nom de la ruche
  rucheName         = prefs.getString("rucheName",rucheN);

  // Mise en veille 
  modeVeille        = prefs.getBool("modeVeille", modeV);
  iTimeToSleep      = prefs.getInt("iTimeToSleep",timeToS);
  LoopNumber        = prefs.getInt("loopnumner",LoopN);
  vTaskDelay(1);

  //Parametres balance
  fAjustement = prefs.getFloat("fAjustement",ajust);
  lValCalibLoadCPT1 = prefs.getLong("ValCalCPT1",valCal1);
  lValCalibLoadCPT2 = prefs.getLong("ValCalCPT2",valCal1);
  lValCalibLoadCPT3 = prefs.getLong("ValCalCPT3",valCal3);
  lValCalibLoadCPT4 = prefs.getLong("ValCalCPT4",valCal4);
  vTaskDelay(1);

  //Paramters MQTT
  mqtt_serveur      = prefs.getString("mqtt_serveur",mqttServeur);
  mqtt_port         = prefs.getInt("mqtt_port",mqttPort);
  mqtt_user         = prefs.getString("mqtt_user",mqttUser);
  mqtt_pass         = prefs.getString("mqtt_pass",mqttPass);
  vTaskDelay(1);

  prefs.end();
}
