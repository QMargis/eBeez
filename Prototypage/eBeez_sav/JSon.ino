#include <ArduinoJson.h>

int iTrame = 0;
int iNbPaquet = 1;

String geneJSon(int iTrame) { // 0:donneesFull ; 10:donneesCalib ; 20:donneeParams; 99:donneesDebug
  String sOutputJSon = "";
  int iCapacity = 0;
  
  switch (iTrame) {
    case 0: // donneesFull[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids]
    {
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(5) + iNbPaquet * JSON_OBJECT_SIZE(5) + 2;
      StaticJsonDocument<100> doc;
      
      // Nom de la ruche
      doc["Ruche"] = sNomRuche;
    
      // Add an array.
      JsonArray data = doc.to<JsonArray>();
      if (!iCptVirtuel) {
        JsonObject data1 = data.createNestedObject();
          data1["TemperatureRuche"] = TemperatureInterneRuche();
          data1["TemperatureExt"] = MesureTemperature();
          data1["Humidite"] = MesureHumidite();
          data1["Pression"] = MesurePression();
          data1["Poids"] = totalWeight();
      }  
      else {
        JsonObject data1 = data.createNestedObject();
          data1["TemperatureRuche"] = (float)(random(1000, 40000))/1000;
          data1["TemperatureExt"] = (float)(random(1000, 40000))/1000;
          data1["Humidite"] = (float)(random(40, 60));
          data1["Pression"] = (float)(random(950, 1030));
          data1["Poids"] = (float)(random(1000, 5000))/1000;
      } 
      serializeJson(doc, sOutputJSon);
      break;
    }
    case 10: // donneesCalib[iNbPaquet][lPresCpt1, lPresCpt2, lPresCpt3, lPresCpt4, bCalFaite, lPostCpt1, lPostCpt2, lPostCpt3, lPostCpt4]
    {  
      // Allocate the JSON document
      iCapacity = JSON_ARRAY_SIZE(9) + iNbPaquet * JSON_OBJECT_SIZE(9) + 2;
      StaticJsonDocument<100> doc;
      
      // Nomn de la ruche
      doc["Ruche"] = sNomRuche;
      
      // Add an array.
      JsonArray data = doc.to<JsonArray>();
      if (!iCptVirtuel) {
        JsonObject data1 = data.createNestedObject();
          data1["lPresCpt1"] = measWeight1();
          data1["lPresCpt2"] = measWeight2();
          data1["lPresCpt3"] = measWeight3();
          data1["lPresCpt4"] = measWeight4();
          data1["bCalFaite"] = LoadCalibrationAllHX711();
          data1["lPostCpt1"] = measWeight1();
          data1["lPostCpt2"] = measWeight2();
          data1["lPostCpt3"] = measWeight3();
          data1["lPostCpt4"] = measWeight4();
      } 
      else {
        JsonObject data1 = data.createNestedObject();
          data1["lPresCpt1"] = (long)(random(400, 1500))/1000;
          data1["lPresCpt2"] = (long)(random(400, 1500))/1000;
          data1["lPresCpt3"] = (long)(random(400, 1500))/1000;
          data1["lPresCpt4"] = (long)(random(400, 1500))/1000;
          data1["bCalFaite"] = true;
          data1["lPostCpt1"] = (long)(random(400, 1500))/1000;
          data1["lPostCpt2"] = (long)(random(400, 1500))/1000;
          data1["lPostCpt3"] = (long)(random(400, 1500))/1000;
          data1["lPostCpt4"] = (long)(random(400, 1500))/1000;
      } 
      serializeJson(doc, sOutputJSon);
      if (iDebug) {
        serializeJsonPretty(doc, Serial);
      }
      break;
    }
    
    case 20: // donneeParams[iNbPaquet][sNomRuche, iTrame, lDelayTrameMin]
    {  
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(3) + iNbPaquet * JSON_OBJECT_SIZE(3) + 2;
      StaticJsonDocument<100> doc;
      
      // Nom de la ruche
      doc["Ruche"] = sNomRuche;
    
      // Add an array.
      JsonArray data = doc.to<JsonArray>();

      JsonObject data1 = data.createNestedObject();
        data1["NomRuche"] = sNomRuche;
        data1["NumTrame"] = iTrame;
        data1["DelaisTrameMinute"] = lDelayTrameMin;

      serializeJson(doc, sOutputJSon);
      if (iDebug) {
        serializeJsonPretty(doc, Serial);
      }
      break;
    }  
    case 99: // donneesDebug[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids, sIP, fRSSI]
    {
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(3) + iNbPaquet * JSON_OBJECT_SIZE(3) + 2;
      StaticJsonDocument<100> doc;
      
      // Nom de la ruche
      doc["Ruche"] = sNomRuche;
    
      // Add an array.
      JsonArray data = doc.to<JsonArray>();
      if (!iCptVirtuel) {
        JsonObject data1 = data.createNestedObject();
          data1["TemperatureRuche"] = TemperatureInterneRuche();
          data1["TemperatureExt"] = MesureTemperature();
          data1["Humidite"] = MesureHumidite();
          data1["Pression"] = MesurePression();
          data1["Poids"] = totalWeight();
          data1["IP"] = WiFi.localIP();
          data1["RSSI"] = WiFi.RSSI();
      } 
      else {
        JsonObject data1 = data.createNestedObject();
          data1["TemperatureRuche"] = (float)(random(1000, 40000))/1000;
          data1["TemperatureExt"] = (float)(random(1000, 40000))/1000;
          data1["Humidite"] = (float)(random(40, 60));
          data1["Pression"] = (float)(random(950, 1030));
          data1["Poids"] = (float)(random(1000, 5000))/1000;
          data1["IP"] = WiFi.localIP();
          data1["RSSI"] = WiFi.RSSI();          
      } 
      serializeJson(doc, sOutputJSon);
      if (iDebug) {
        serializeJsonPretty(doc, Serial);
      }
      break;
    }
    default: // Demande inconnue : donneeError[iNbPaquet][iTrame]
    {
      // Allocate the JSON document
      int iCapacity = JSON_ARRAY_SIZE(1) + iNbPaquet * JSON_OBJECT_SIZE(1) + 2;
      StaticJsonDocument<100> doc;
      
      // Nom de la ruche
      doc["Ruche"] = sNomRuche;
    
      // Add an array.
      JsonArray data = doc.to<JsonArray>();

      JsonObject data1 = data.createNestedObject();
        data1["ErrorNumTrame"] = iTrame;

      serializeJson(doc, sOutputJSon);

      break;
    }
  }
  if (iDebug) {
    Serial.println(sOutputJSon);
  }
  return (sOutputJSon);
}
