/*
à voir avec bertrand pour les explications
 */

void geneJSON(int iTrame)  // 0:donneesFull ; 10:donneesCalib ; 20:donneeParams; 99:donneesDebug
{ 
  int iCapacity;

  switch (iTrame) 
  {
    case 0: // donneesFull[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids, tension, RSSI]
    {
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(7) + iNbPaquet * JSON_OBJECT_SIZE(7) + 2;
      StaticJsonDocument<120> doc;   // a augmenter si d'autres mesures sont ajoutées 
      doc["TempRuche"]  = TemperatureInterneRuche();
      doc["TempExt"]    = MesureTemperature();
      doc["Humidite"]   = MesureHumidite();
      doc["Pression"]   = MesurePression();
      doc["Poids"]      = totalWeight();
      doc["Tension"]    = GetVoltage ();
      doc["RSSI"]       = WiFi.RSSI();
      serializeJson(doc, sOutputJSon);
      if ((bMenu == true) && (bStatusJsonDisplay == true)){Serial.print("Trame JSon  0: "); Serial.println(sOutputJSon);}
      
      break;
    }
    case 10: // donneesCalib[iNbPaquet][lPresCpt1, lPresCpt2, lPresCpt3, lPresCpt4, bCalFaite, lPostCpt1, lPostCpt2, lPostCpt3, lPostCpt4]
    {  
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(8) + iNbPaquet * JSON_OBJECT_SIZE(8) + 2;
      StaticJsonDocument<256> doc;
      doc["lPresCpt1"] = measWeight1();
      doc["lPresCpt2"] = measWeight2();
      doc["lPresCpt3"] = measWeight3();
      doc["lPresCpt4"] = measWeight4();
//      doc["bCalFaite"] = LoadCalibrationAllHX711 ();
      doc["lPostCpt1"] = lValCalibLoadCPT1;
      doc["lPostCpt2"] = lValCalibLoadCPT2;
      doc["lPostCpt3"] = lValCalibLoadCPT3;
      doc["lPostCpt4"] = lValCalibLoadCPT4;
      serializeJson(doc, sOutputJSon);
      if ((bMenu == true) && (bStatusJsonDisplay == true)) {Serial.print("OutputJSon 10: "); Serial.println(sOutputJSon);}
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
        doc["TempRuche"]  = TemperatureInterneRuche();
        doc["TempExt"]    = MesureTemperature();
        doc["Humidite"]   = MesureHumidite();
        doc["Pression"]   = MesurePression();
        doc["Poids"]      = totalWeight();
        doc["IP"]         = WiFi.localIP();
        doc["RSSI"]       = WiFi.RSSI();

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

  if (bMenu == true) 
  {
    Serial.print  ("iTrame: ");
    Serial.println(iTrame);
    Serial.print  ("lDelayTrameMin: ");
    Serial.println(lDelayTrameMin);
  }
 
  vTaskDelay(1);   // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble 
}
