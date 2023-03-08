
//detecte les commandes d'entrées dans le moniteur serie et lance les procedures corespondantes
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
          LoadMesurement();
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

// Force la reconnexion en cas de perte de réseau
      else if (sLectureSerie.equals("rip") || sLectureSerie.equals("RIP"))
        {
            vTaskDelay(1);
            TryToConnectWifi();
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
