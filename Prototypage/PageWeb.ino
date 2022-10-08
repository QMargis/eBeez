/*************************************************************
https://www.w3schools.com/css/default.asp
**************************************************************
 */

// Site Web a voir
// https://www.youtube.com/watch?v=LkxCr_n3JOw
// https://www.w3schools.com/w3css/w3css_input.asp
// https://tommydesrochers.com/recuperer-les-valeurs-dun-formulaire-web-sans-rafraichir-la-page-jquery-esp32-ep5/

//----------------------------------------------------------------------------

void setupPageWeb()
 {
  Serial.setDebugOutput(true);
  if(!SPIFFS.begin(true))
  {
    Serial.println(F("An Error has occurred while mounting SPIFFS"));
    return;
  }
    File root = SPIFFS.open("/");
    File file = root.openNextFile();

  while(file)
  {
    Serial.print(F("File: "));
    Serial.println(file.name());
    file.close();
    file = root.openNextFile();
  }
// Lance un test de connexion Wifi

  TryToConnectWifi();
  
  
  // Print ESP32 Local IP Address--------------------------------------
  Serial.print(F("Adresse IP : "));Serial.println(WiFi.localIP());



  // Route for root / web page-----------------------------------------
server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)  
  {
    request->send(SPIFFS, "/index.html", "text/html");
  });

server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/w3.css", "text/css");
  });

server.on("/jquery-3.6.0.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/jquery-3.6.0.min.js", "text/javasript");
  });

server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/script.js", "text/javasript");
  });
  
server.on("/Beez2.jpg", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/Beez2.jpg", "img/jpg");
  });
 // Lecture données / web page-----------------------------------------

 
//---------Lecture du SSID-------------------------
server.on("/lireSSID", HTTP_GET, [](AsyncWebServerRequest *request)
  {           
    request->send(200, "text/plain", sSSID_Select );
  }); 
 
  //---------Lecture du courant IN226--------------------------
server.on("/lireCourant", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sCourant = "Nan";
           vTaskDelay(1); 
           sCourant = MesureCourant();   
            
    request->send(200, "text/plain", sCourant );
  });  
  //---------Lecture de la puissance --------------------------
server.on("/lirePuissance", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sPuissance  = "Nan";
           vTaskDelay(1); 
           sPuissance  = MesurePuissance (); 
                 
    request->send(200, "text/plain", sPuissance );
  });

  //---------Tension de la batterie ---------------------
server.on("/lireBatterie", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sBatterie  = "Nan";
           vTaskDelay(1); 
           sBatterie  = String (MesureTension(),2); 
                         
    request->send(200, "text/plain", sBatterie);
  });
  
//-----------TNiveau RSSI--------------------------------
server.on("/lireNiveauRSSI", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sRSSI  = "Nan";
           vTaskDelay(1); 
           sRSSI  = String(WiFi.RSSI());   
                       
    request->send(200, "text/plain", sRSSI);
  });

//---------Lecture de la du niveau d'humiditée --------------------------
server.on("/lireHumidite", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sHumidite  = "Nan";
           vTaskDelay(1); 
           sHumidite  = String(MesureHumidite(),0);            //
           
    request->send(200, "text/plain", sHumidite );
  });

//---------Lecture de la température  --------------------------
server.on("/lireTemperature", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sTemperature  = "Nan";
           vTaskDelay(1); 
           sTemperature  = String(MesureTemperature(),1);            //
           
    request->send(200, "text/plain", sTemperature );
  });

//---------Lecture de la pression --------------------------
server.on("/lirePression", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sPression  = "Nan";
           vTaskDelay(1); 
           sPression  = String(MesurePression()/100 , 0);            //           
    request->send(200, "text/plain", sPression );
  });

//---------Lecture de la Température DS18B20 --------------------------
server.on("/LireTemperatureDS18S20", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sTemperatureDS18S20  = "Nan";
           vTaskDelay(1); 
           sTemperatureDS18S20  = String(TemperatureInterneRuche(),1);            //
           
    request->send(200, "text/plain", sTemperatureDS18S20 );
  });
/*
//---------Lecture du capteur de poids n°1 --------------------------
server.on("/lirePoids1", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sCptPoids1  = "Nan";
           vTaskDelay(1); 
           //sCptPoids1  = String(measWeight1()/100,0);            //
           
    request->send(200, "text/plain", sCptPoids1 );
  });

//---------Lecture du capteur de poids n°2 --------------------------
server.on("/lirePoids2", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sCptPoids2  = "Nan";
           vTaskDelay(1); 
           //sCptPoids2  = String(measWeight2()/100,0);            //
           
    request->send(200, "text/plain", sCptPoids2 );
  });
//---------Lecture du capteur de poids n°3 --------------------------
server.on("/lirePoids3", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sCptPoids3  = "Nan";
           vTaskDelay(1); 
           //sCptPoids3  = String(measWeight3()/100,0);            //
           
    request->send(200, "text/plain", sCptPoids3 );
  }); 
//---------Lecture du capteur de poids n°4 --------------------------
server.on("/lirePoids4", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sCptPoids4  = "Nan";
           vTaskDelay(1); 
          // sCptPoids4  = String(measWeight4()/100,0);            //          
    request->send(200, "text/plain", sCptPoids4 );
  });
*/
//---------Lecture du capteur de poids total --------------------------
server.on("/lirePoidsTotal", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sCptPoidsTotal  = "Nan";
           vTaskDelay(1); 
           sCptPoidsTotal  = String(totalWeight()); 
    request->send(200, "text/plain", sCptPoidsTotal);
  });

/*    
//---------Lecture du Mode Auto/manuel-----------------------------
server.on("/lireDistance", HTTP_GET, [](AsyncWebServerRequest *request)
  {   
    Distance();
    String sDistance  = ("--");
    if (sensor.tofa.distancemm < 1000) // si > 1m çà sert à rien.
        {
            sDistance  = (sensor.tofa.distancemm);        
        }     
    if (sensor.tofa.distancemm < 300) // si > 1m çà sert à rien.
        {
            Arreter ();          
        }  
 
           //Serial.println(sensor.tofa.distancemm);
    request->send(200, "text/plain", sDistance );
  });


  //---------Courant A heure ------------------------------------
server.on("/lireCourantAh", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sCourantAh  = "Nan";
           sCourantAh  = String(MesureAh ());               
    request->send(200, "text/plain", sCourantAh);
  });


 /*
//---------Heure de debut démarrage de la rampe -------------------------
server.on("/lireTimeLightOn", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sTimeLightOn  = "Nan";
           sTimeLightOn  = String (HeureStartLight) ;               
    request->send(200, "text/plain", sTimeLightOn ) ;
  });

//---------Heure de début d'arret  de la rampe --------------------------
server.on("/lireTimeLightOff", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String sTimeLightOff  = "Nan";
           sTimeLightOff  = String(HeureStopLight);              
    request->send(200, "text/plain", sTimeLightOff);
  });
*/
//---------------------------  
//-------Les boutons---------
//---------------------------
  
  //------- Boutons calibration----------------------------------------------- 
server.on("/LoadCalibration", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    LoadCalibrationAllHX711 ();
    request->send(204);
  });
  //--------Boutons Automatique------------------------------------------
server.on("/Recule", HTTP_GET, [](AsyncWebServerRequest *request)
  {   
    //Serial.println (F("Recule")); 
 
    request->send(204);
  });
  //------- Boutons Droite----------------------------------------------- 
server.on("/Droite", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    //Serial.println (F("Droite"));

    request->send(204);
  });
  //--------Boutons Gauche ------------------------------------------
server.on("/Gauche", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    //Serial.println ("Gauche");

    request->send(204);
  });

  //------- Boutons ferme----------------------------------------------- 
server.on("/Stop", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    //Serial.println (F("Stop")); 
  
    request->send(204);
  });
  //--------Boutons ouvre------------------------------------------
server.on("/OuvreVanne", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(204);
  });

  //---------------------------------------------------------------------------------------------------------------------------------------
  server.begin();
  Serial.println("Serveur actif!");
}


void TryToConnectWifi()
{
 uint8_t CountTrytoConnect = 0;
   // -------------Connect to Wi-Fi 1 ----------------------
   // Set Access Point configuration
  WiFi.begin(ssid0, password0);
  Serial.print(F("Tentative de connexion au Réseau WIFI_1"));
  while ((WiFi.status() != WL_CONNECTED) && (CountTrytoConnect < 20))
  {
    delay(500);
    Serial.print(F("."));
    ++CountTrytoConnect;
  }
  if (CountTrytoConnect>=20)         { Serial.print(ssid0); Serial.println(" Non connecté !");} 
  if (WiFi.status() == WL_CONNECTED) { Serial.print(ssid0); Serial.println("  connecté !");sSSID_Select = ssid0;}
   
if (WiFi.status() != WL_CONNECTED)
 { 
   CountTrytoConnect = 0;
    // -------------Connect to Wi-Fi 2 ----------------------
    WiFi.begin(ssid1, password1);
    Serial.print(F("Tentative de connexion au Réseau WIFI_2"));
     while ((WiFi.status() != WL_CONNECTED) && (CountTrytoConnect < 20))
      {
        delay(500);
        Serial.print(F("."));
        ++CountTrytoConnect;
      }
   if (CountTrytoConnect >= 20)       { Serial.print(ssid1); Serial.println(" Non connecté !");}
   if (WiFi.status() == WL_CONNECTED) { Serial.print(ssid1); Serial.println("  connecté !");sSSID_Select = ssid1;}
 }

if (WiFi.status() != WL_CONNECTED)
 { 
   CountTrytoConnect = 0;
    // -------------Connect to Wi-Fi 2 ----------------------
    WiFi.begin(ssid2, password2);
    Serial.print(F("Tentative de connexion au Réseau WIFI_3"));
     while ((WiFi.status() != WL_CONNECTED) && (CountTrytoConnect < 20))
      {
        delay(500);
        Serial.print(F("."));
        ++CountTrytoConnect;
      }
   if (CountTrytoConnect >= 20)       { Serial.print(ssid2); Serial.println(" Non connecté !");}
   if (WiFi.status() == WL_CONNECTED) { Serial.print(ssid2); Serial.println("  connecté !");sSSID_Select = ssid1;}
 } 
}


  
