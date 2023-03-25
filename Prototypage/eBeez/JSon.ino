#include <ArduinoJson.h>

void geneJSon(int iTrame) { // 0:donneesFull ; 10:donneesCalib ; 20:donneeParams; 99:donneesDebug
  int iCapacity;

  switch (iTrame) {
    case 0: // donneesFull[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids]
    {
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(5) + iNbPaquet * JSON_OBJECT_SIZE(5) + 2;
      StaticJsonDocument<100> doc;
      
      // Add an array.
      JsonArray data = doc.to<JsonArray>();
      if (!iCptVirtuel) {
        JsonObject data1 = data.createNestedObject();
          data1["TempRuche"] = TemperatureInterneRuche();
          data1["TempExt"] = MesureTemperature();
          data1["Humidite"] = MesureHumidite();
          data1["Pression"] = MesurePression();
          data1["Poids"] = totalWeight();
      }  
      else {
        JsonObject data1 = data.createNestedObject();
          data1["TempRuche"] = (int)(random(1000, 40000))/1000;
          data1["TempExt"] = (int)(random(1000, 40000))/1000;
          data1["Humidite"] = (int)(random(40, 60));
          data1["Pression"] = (int)(random(950, 1030));
          data1["Poids"] = (int)(random(1000, 5000))/1000;
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    case 10: // donneesCalib[iNbPaquet][lPresCpt1, lPresCpt2, lPresCpt3, lPresCpt4, bCalFaite, lPostCpt1, lPostCpt2, lPostCpt3, lPostCpt4]
    {  
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(9) + iNbPaquet * JSON_OBJECT_SIZE(9) + 2;
      StaticJsonDocument<100> doc;
      
      // Add an array.
      JsonArray data = doc.to<JsonArray>();
      if (!iCptVirtuel) {
        JsonObject data1 = data.createNestedObject();
          data1["lPresCpt1"] = ValCalibLoadCPT1;
          data1["lPresCpt2"] = ValCalibLoadCPT2;
          data1["lPresCpt3"] = ValCalibLoadCPT3;
          data1["lPresCpt4"] = ValCalibLoadCPT4;
          data1["bCalFaite"] = LoadCalibrationAllHX711();
          data1["lPostCpt1"] = ValCalibLoadCPT1;
          data1["lPostCpt2"] = ValCalibLoadCPT2;
          data1["lPostCpt3"] = ValCalibLoadCPT3;
          data1["lPostCpt4"] = ValCalibLoadCPT4;
      } 
      else {
        JsonObject data1 = data.createNestedObject();
          data1["lPresCpt1"] = (int)(random(400, 1500))/1000;
          data1["lPresCpt2"] = (int)(random(400, 1500))/1000;
          data1["lPresCpt3"] = (int)(random(400, 1500))/1000;
          data1["lPresCpt4"] = (int)(random(400, 1500))/1000;
          data1["bCalFaite"] = true;
          data1["lPostCpt1"] = (int)(random(400, 1500))/1000;
          data1["lPostCpt2"] = (int)(random(400, 1500))/1000;
          data1["lPostCpt3"] = (int)(random(400, 1500))/1000;
          data1["lPostCpt4"] = (int)(random(400, 1500))/1000;
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    
    case 20: // donneeParams[iNbPaquet][sTopic, iTrame, lDelayTrameMin]
    {  
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(3) + iNbPaquet * JSON_OBJECT_SIZE(3) + 2;
      StaticJsonDocument<100> doc;
       
      // Add an array.
      JsonArray data = doc.to<JsonArray>();

      JsonObject data1 = data.createNestedObject();
        data1["Topic"] = sTopic;
        data1["NumTrame"] = iTrame;
        data1["DelaisTrameMinute"] = lDelayTrameMin;

      serializeJson(doc, sOutputJSon);
      break;
    }  
    case 99: // donneesDebug[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids, sIP, fRSSI]
    {
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(3) + iNbPaquet * JSON_OBJECT_SIZE(3) + 2;
      StaticJsonDocument<150> doc;
      
      // Add an array.
      JsonArray data = doc.to<JsonArray>();
      if (!iCptVirtuel) {
        JsonObject data1 = data.createNestedObject();
          data1["TempRuche"] = TemperatureInterneRuche();
          data1["TempExt"] = MesureTemperature();
          data1["Humidite"] = MesureHumidite();
          data1["Pression"] = MesurePression();
          data1["Poids"] = totalWeight();
          data1["IP"] = WiFi.localIP();
          data1["RSSI"] = WiFi.RSSI();
      } 
      else {
        JsonObject data1 = data.createNestedObject();
          data1["TempRuche"] = (int)(random(1000, 40000))/1000;
          data1["TempExt"] = (int)(random(1000, 40000))/1000;
          data1["Humidite"] = (int)(random(40, 60));
          data1["Pression"] = (int)(random(950, 1030));
          data1["Poids"] = (int)(random(1000, 5000))/1000;
          data1["IP"] = WiFi.localIP();
          data1["RSSI"] = WiFi.RSSI();          
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    default: // Demande inconnue : donneeError[iNbPaquet][iTrame]
    {
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(1) + iNbPaquet * JSON_OBJECT_SIZE(1) + 2;
      StaticJsonDocument<100> doc;
      
      // Add an array.
      JsonArray data = doc.to<JsonArray>();

      JsonObject data1 = data.createNestedObject();
        data1["ErrorNumTrame"] = iTrame;

      serializeJson(doc, sOutputJSon);
      break;
    }
  }
}
