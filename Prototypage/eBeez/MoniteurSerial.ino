//--------------------------------------------------------------------------------------------------
//interaction avec la saisie de terminal pour le debug , configuration ou activation de fonctions
//detecte les commandes d'entrées dans le moniteur serie et lance les procedures corespondantes
//--------------------------------------------------------------------------------------------------

void SerialRead()
{
  while (Serial.available() > 1)
  {
    Serial.setTimeout(100);
    sLectureSerie = Serial.readStringUntil('\n');
    
// Appel l'IHM menu sur un terminal serie
    if (sLectureSerie.equals("menu") || sLectureSerie.equals("MENU"))
        {
            vTaskDelay(1);
            bMenu = true;
            Menu ();
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }

// Calibration des capteurs de poids
      else if (sLectureSerie.equals("cal") || sLectureSerie.equals("CAL"))
        {  
          vTaskDelay(1);
          LoadCalibrationAllHX711 ();
          Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }

// Affiche la mesure de poids des 4 capteurs
      else if (sLectureSerie.equals("poids") || sLectureSerie.equals("POIDS"))
        {  
          vTaskDelay(1);
          Serial.println("Données brutes des convertisseur HX711");
          LoadMesurement();
          Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }             

// Affiche la mesure des parametres de l'alimentation
      else if (sLectureSerie.equals("alim") || sLectureSerie.equals("ALIM"))
        {  
          vTaskDelay(1);
          Serial.print   (GetVoltage());Serial.println(" Volts");
          Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }          

 // Scan des peripheriques I2C   
      else if (sLectureSerie.equals("scan") || sLectureSerie.equals("SCAN"))
        {
            vTaskDelay(1);
            setupI2C();
            Scan_I2C();
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }   

// Affiche l'adresse IP  
      else if (sLectureSerie.equals("ip") || sLectureSerie.equals("IP"))
        {
            vTaskDelay(1);
            Serial.print("Adresse IP : ");
            Serial.print(WiFi.localIP());
            Serial.print("  Statut: ");Serial.print( WL_CONNECTED);Serial.print(" nb de reconnections : ");Serial.println(iNbReconnections);
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }   

// Affiche le niveau RSSI du Wifi 
      else if (sLectureSerie.equals("rssi") || sLectureSerie.equals("RSSI"))
        {
            vTaskDelay(1);
            Serial.print("Niveau de reception : ");
            Serial.print(WiFi.RSSI());Serial.println(" DBm");
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }  
// Donne la température,humidité,pression atmosphérique
      else if (sLectureSerie.equals("temp") || sLectureSerie.equals("TEMP"))
        {
            vTaskDelay(1);
            Serial.print("Température : "); Serial.print(MesureTemperature());Serial.print("°C  ");
            Serial.print("Humidité    : "); Serial.print(MesureHumidite())   ;Serial.print("%   ");
            Serial.print("Pression    : "); Serial.print(MesurePression())   ;Serial.print("Pa  ");
            Serial.println();
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }
// Donne la température interieure de la Ruche
      else if (sLectureSerie.equals("tempru") || sLectureSerie.equals("TEMPRU"))
        {
            vTaskDelay(1);
            Serial.print("Température Ruche : "); Serial.print(TemperatureInterneRuche());Serial.print("°C  ");
            Serial.println();
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }
        
// Test de mise en veille 10 secondes
      else if (sLectureSerie.equals("sleep") || sLectureSerie.equals("SLEEP"))
        {
            GotoToSleepAndWakeUpByTimer(iTimeToSleep);   // en seconde pour test
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }
// test coupure des périphériques 
      else if (sLectureSerie.equals("psuoff") || sLectureSerie.equals("PSUOFF"))
        {
            PsuShutDown();
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }
// test le remise en route des périphériques 
      else if (sLectureSerie.equals("psuon") || sLectureSerie.equals("PSUON"))
        {
            PsuWakeUp();
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }

// affiche la lecture des valeurs de calibration mémorisées en EEPROM des 4 capteur de poids 
      else if (sLectureSerie.equals("readcal") || sLectureSerie.equals("READCAL"))
        {
            Serial.println("Lecture des valeurs brutes de l'EEPROM");
            Serial.print  ("Capteur n°1: "); Serial.println(iEepromDataRead (0));
            Serial.print  ("Capteur n°2: "); Serial.println(iEepromDataRead (4));
            Serial.print  ("Capteur n°3: "); Serial.println(iEepromDataRead (8));
            Serial.print  ("Capteur n°4: "); Serial.println(iEepromDataRead(12));
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }   

// affiche la lecture des valeurs de calibration mémorisées en EEPROM des 4 capteur de poids 
      else if (sLectureSerie.equals("resetcal") || sLectureSerie.equals("RESETCAL"))
        {
            resetEEPROM();
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }  

             
// Force la reconnexion en cas de perte de réseau
      else if (sLectureSerie.equals("rip") || sLectureSerie.equals("RIP"))
        {
            vTaskDelay(1);
            TryToConnectWifi();
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        } 

// Affiche le Json à l'écran 
      else if (sLectureSerie.equals("jsonon") || sLectureSerie.equals("JSONON"))
        {
            bStatusJsonDisplay = true ;
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        }   

// Affiche le Json à l'écran 
      else if (sLectureSerie.equals("jsonoff") || sLectureSerie.equals("JSONOFF"))
        {
            bStatusJsonDisplay = false ;
            Serial.println(sLectureSerie + " OK"); Serial.println(">");
        } 
     
      // gestion des erreurs si mauvais retour.
      // evite de figer le programme dans une attente infinie sur le  port com
      else if (sLectureSerie.equals("") || sLectureSerie.equals("\n"))
        {
          // ne rien faire!
          vTaskDelay(1);
        }
      // retourne une erreur si mauvaise saisie clavier ou mauvaise commande
      else
        {
            vTaskDelay(1);
            Serial.println(">" + sLectureSerie);
            Serial.println(F("Invalid command"));
            Serial.println(F(">"));
        }
    }
}
