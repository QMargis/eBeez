#include <ArduinoJson.h>

void geneJSON(int iTrame) { // 0:donneesFull ; 10:donneesCalib ; 20:donneeParams; 99:donneesDebug
  int iCapacity;

  switch (iTrame) {
    case 0: // donneesFull[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids]
    {
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(5) + iNbPaquet * JSON_OBJECT_SIZE(5) + 2;
      StaticJsonDocument<100> doc;

      if (!iCptVirtuel) {
        doc["TempRuche"]  = TemperatureInterneRuche();
        doc["TempExt"]    = MesureTemperature();
        doc["Humidite"]   = MesureHumidite();
        doc["Pression"]   = MesurePression();
        doc["Poids"]      = totalWeight();
      }  
      else {
        doc["TempRuche"]  = (int)(random(1000, 40000))/1000;
        doc["TempExt"]    = (int)(random(1000, 40000))/1000;
        doc["Humidite"]   = (int)(random(40, 60));
        doc["Pression"]   = (int)(random(950, 1030));
        doc["Poids"]      = (int)(random(1000, 5000))/1000;
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    case 10: // donneesCalib[iNbPaquet][lPresCpt1, lPresCpt2, lPresCpt3, lPresCpt4, bCalFaite, lPostCpt1, lPostCpt2, lPostCpt3, lPostCpt4]
    {  
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(9) + iNbPaquet * JSON_OBJECT_SIZE(9) + 2;
      StaticJsonDocument<256> doc;

      if (!iCptVirtuel) {
        doc["lPresCpt1"] = ValCalibLoadCPT1;
        doc["lPresCpt2"] = ValCalibLoadCPT2;
        doc["lPresCpt3"] = ValCalibLoadCPT3;
        doc["lPresCpt4"] = ValCalibLoadCPT4;
        doc["bCalFaite"] = LoadCalibrationAllHX711();
        doc["lPostCpt1"] = ValCalibLoadCPT1;
        doc["lPostCpt2"] = ValCalibLoadCPT2;
        doc["lPostCpt3"] = ValCalibLoadCPT3;
        doc["lPostCpt4"] = ValCalibLoadCPT4;
      } 
      else {
        doc["lPresCpt1"] = (int)(random(400, 1500))/10;
        doc["lPresCpt2"] = (int)(random(400, 1500))/10;
        doc["lPresCpt3"] = (int)(random(400, 1500))/10;
        doc["lPresCpt4"] = (int)(random(400, 1500))/10;
        doc["bCalFaite"] = true;
        doc["lPostCpt1"] = (int)(random(400, 1500))/10;
        doc["lPostCpt2"] = (int)(random(400, 1500))/10;
        doc["lPostCpt3"] = (int)(random(400, 1500))/10;
        doc["lPostCpt4"] = (int)(random(400, 1500))/10;
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    
    case 20: // donneeParams[iNbPaquet][cTopicWrite, cTopicRead, iTrame, lDelayTrameMin]
    {   
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(3) + iNbPaquet * JSON_OBJECT_SIZE(3) + 2;
      StaticJsonDocument<100> doc;

      doc["TopicWrite"]         = cTopicWrite;
      doc["TopicRead"]          = cTopicRead;
      doc["NumTrame"]           = iTrame;
      doc["DelaisTrameMinute"]  = lDelayTrameMin;

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
        doc["TempExt"]    = MesureTemperature();
        doc["Humidite"]   = MesureHumidite();
        doc["Pression"]   = MesurePression();
        doc["Poids"]      = totalWeight();
        doc["IP"]         = WiFi.localIP();
        doc["RSSI"]       = WiFi.RSSI();
      } 
      else {
        doc["TempRuche"]  = (int)(random(1000, 40000))/1000;
        doc["TempExt"]    = (int)(random(1000, 40000))/1000;
        doc["Humidite"]   = (int)(random(40, 60));
        doc["Pression"]   = (int)(random(950, 1030));
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

void decodeJSON(char* payload, unsigned int length) { // donneeParams[iNbPaquet][iTrame, lDelayTrameMin]
  
  StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload, length);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  iTrame = doc["iTrame"];
  lDelayTrameMin = doc["lDelayTrameMin"];

  if (iDebug) {
    Serial.print("iTrame: ");
    Serial.println(iTrame);
    Serial.print("lDelayTrameMin: ");
    Serial.println(lDelayTrameMin);
  }
 
  vTaskDelay(1);   // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble 
}
