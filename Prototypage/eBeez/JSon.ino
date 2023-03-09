#include <ArduinoJson.h>

int iTrame = 0;
int iNbPaquet = 1;

String geneJSon(int iTrame) { // 0:donneesFull ; 1:donneesCalib ; 2:donneesDebug
  String sOutputJSon = "";
  
  switch (iTrame) {
    case 0: // donneesFull[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids]
      // Allocate the JSON document
      const int iCapacity = JSON_ARRAY_SIZE(5) + iNbPaquet * JSON_OBJECT_SIZE(5) + 2;
      StaticJsonDocument<100> doc;
      
      // Nomn de la ruche
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
          data1["TemperatureRuche"] = random(1000, 40000)/1000;
          data1["TemperatureExt"] = random(1000, 40000)/1000;
          data1["Humidite"] = random(40, 60);
          data1["Pression"] = random(950, 1030);
          data1["Poids"] = random(1000, 5000)/1000;
      } 
      serializeJson(doc, sOutputJSon);
      if (!iDebug) {
        serializeJsonPretty(doc, Serial);
      }
      break;
    /*
    case 1: // donneesCalib[iNbPaquet][CalFaite, lPresCpt1, lPresCpt2, lPresCpt3, lPresCpt4, lPostCpt1, lPostCpt2, lPostCpt3, lPostCpt4]

      break;
    
    case 2: // donneesDebug[iNbPaquet][fTempRuche, fTempExt, fHumidite, fPression, lPoids, sIP, fRSSID]

      break;

    default: // ???

      break;*/
  }
  
  return (sOutputJSon);
}
