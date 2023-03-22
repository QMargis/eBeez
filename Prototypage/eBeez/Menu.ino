// Affiche une aide aux commandes saisie au clavier dans le moniteur serie 
void Menu ()
{if (bMenu == true) 
  {
    Serial.println();
    Serial.println(F("|---------------------------------------------------------------------------------|"));
    Serial.println(F("|---------------------------------------------------------------------------------|"));
    Serial.println(F("|--------                     E-Beez Menu V0.02                         ----------|"));
    Serial.println(F("|                           _                                                     |"));
    Serial.println(F("|                          ( )       .'''.                                        |"));
    Serial.println(F("|                       =O(_)))) ...'     `.                                      |"));
    Serial.println(F("|                          (_)              `.    .''                             |"));
    Serial.println(F("|                                            `..'                                 |"));
    Serial.println(F("|                                                                                 |"));   
    Serial.println(F("|---------------------------------------------------------------------------------|"));
    Serial.println(F("| Menu-Active avec l'affichage des divers evenements-------| menu     ou   MENU   |"));
    Serial.println(F("| Lance la calibration des capteurs de poids --------------| cal      ou   CAL    |"));
    Serial.println(F("| Affiche la mesure des Quatre capteur de poids -----------| poids    ou   POIDS  |"));
    Serial.println(F("| Affiche la mesure des paramétres d'alimentation ---------| alim     ou   ALIM   |"));   
    Serial.println(F("| Scan des périphériques I2C pour trouver les adresses ----| scan     ou   SCAN   |"));
    Serial.println(F("| Affiche l'adresse IP de la page WEB ---------------------| ip       ou   IP     |"));
    Serial.println(F("| Reset Ip  -----------------------------------------------| rip      ou   RIP    |"));
    Serial.println(F("| Donne le niveau de reception RSSI du Wifi----------------| rssi     ou   RSSI   |"));
    Serial.println(F("| Donne la température interne de la ruche ----------------| tempru   ou   TEMPRU |"));
    Serial.println(F("| Donne la température,humidité,pression atmosphérique-----| temp     ou   TEMP   |"));
    Serial.println(F("| Test de la mise en sommeil 10secondes--------------------| sleep    ou   SLEEP  |"));
    Serial.println(F("|_________________________________________________________________________________|"));
  }
  else
      {
        Serial.println(F("Pour voir le menu des fonctions et activer les retours des informations, Tapez menu ou MENU dans le serial monitor "));
      }
}
