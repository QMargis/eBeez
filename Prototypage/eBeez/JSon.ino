#include <ArduinoJson.h>

void geneJSON(int iTrame) { // 0:donneesFull ; 10:donneesCalib ; 20:donneeParams; 99:donneesDebug
  int iCapacity;

  switch (iTrame) {
    case 0: // donneesFull[fTempRuche, fTempExt, fHumidite, fPression, lPoids, RSSI, Voltage]
    {
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(8) + iNbPaquet * JSON_OBJECT_SIZE(8) + 2;
      StaticJsonDocument<100> doc;

      if (!iCptVirtuel) {
        doc["TempRuche"]  = TemperatureInterneRuche();
        doc["Poids"]      = totalWeight();
        doc["RSSI"]       = WiFi.RSSI();
        doc["Voltage"]    = GetVoltage();
        doc["measWeight1"]= measWeight1();
        doc["measWeight2"]= measWeight2();
        doc["measWeight3"]= measWeight3();
        doc["measWeight4"]= measWeight4();
      }  
      else {
        doc["TempRuche"]  = (int)(random(1000, 40000))/1000;
        doc["Poids"]      = (int)(random(1000, 5000))/1000;
        doc["RSSI"]       = (int)(random(-120, -30));
        doc["Voltage"]    = (int)(random(3, 10));
        doc["measWeight1"]= (int)(random(1000, 400000))/1000;
        doc["measWeight2"]= (int)(random(1000, 400000))/1000;
        doc["measWeight3"]= (int)(random(1000, 400000))/1000;
        doc["measWeight4"]= (int)(random(1000, 400000))/1000;
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    case 10: // donneesCalib[iNbPaquet][lValCalibLoadCPT1, lValCalibLoadCPT2, lValCalibLoadCPT3, lValCalibLoadCPT4]
    {  
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(4) + iNbPaquet * JSON_OBJECT_SIZE(4) + 2;
      StaticJsonDocument<256> doc;

      if (!iCptVirtuel) {
        doc["lPresCpt1"] = lValCalibLoadCPT1;
        doc["lPresCpt2"] = lValCalibLoadCPT2;
        doc["lPresCpt3"] = lValCalibLoadCPT3;
        doc["lPresCpt4"] = lValCalibLoadCPT4;
      } 
      else {
        doc["lPresCpt1"] = (int)(random(400, 1500))/10;
        doc["lPresCpt2"] = (int)(random(400, 1500))/10;
        doc["lPresCpt3"] = (int)(random(400, 1500))/10;
        doc["lPresCpt4"] = (int)(random(400, 1500))/10;
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    
    case 20: // donneeParams[iNbPaquet][cTopicWrite, cTopicRead, iTrame, lDelayTrame]
    {   
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(3) + iNbPaquet * JSON_OBJECT_SIZE(3) + 2;
      StaticJsonDocument<100> doc;

      doc["TopicWrite"]         = cTopicWrite;
      doc["TopicRead"]          = cTopicRead;
      doc["NumTrame"]           = iTrame;
      doc["DelaisTrameMinute"]  = lDelayTrame;

      serializeJson(doc, sOutputJSon);
      break;
    }  
    case 99: // donneesDebug[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids, sIP, fRSSI]
    {
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(3) + iNbPaquet * JSON_OBJECT_SIZE(3) + 2;
      StaticJsonDocument<150> doc;
      
      if (!iCptVirtuel) {
        doc["TempRuche"]  = TemperatureInterneRuche();
        doc["Poids"]      = totalWeight();
        doc["IP"]         = WiFi.localIP();
        doc["RSSI"]       = WiFi.RSSI();
      } 
      else {
        doc["TempRuche"]  = (int)(random(1000, 40000))/1000;
        doc["Poids"]      = (int)(random(1000, 5000))/1000;
        doc["IP"]         = WiFi.localIP();
        doc["RSSI"]       = WiFi.RSSI();          
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    default: // Demande inconnue : donneeError[iNbPaquet][iTrame]
    {
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(1) + iNbPaquet * JSON_OBJECT_SIZE(1) + 2;
      StaticJsonDocument<100> doc;
      
      doc["ErrorNumTrame"] = iTrame;

      serializeJson(doc, sOutputJSon);
      break;
    }
  }
}

void decodeJSON(char* payload, unsigned int length) { // donneeParams[iNbPaquet][iTrame, modeVeille]
  
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, payload, length);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  iTrame = doc["iTrame"];
  int mVeille = doc["modeVeille"];
  if(mVeille == 1) { modeVeille=true; } else if(mVeille == 0) { modeVeille=false; }

  if (iDebug) {
    Serial.print("iTrame: ");
    Serial.println(iTrame);
    Serial.print("modeVeille: ");
    Serial.print(mVeille);
    Serial.print(" // ");
    Serial.println(modeVeille);
  }

  // Sauvegarde du mode de veille en EEPROM
  prefs.begin(ebeezSpace, ReadWrite);

  const size_t putModeVeille = prefs.putBool("modeVeille", modeVeille); 
  if (putModeVeille == 0) { Serial.println("Preferences: Could not write modeVeille to nvs. No reboot");  }
  else {Serial.print("modeVeille:"); Serial.println(modeVeille);}
  vTaskDelay(1);   // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble 

  prefs.end();
}
