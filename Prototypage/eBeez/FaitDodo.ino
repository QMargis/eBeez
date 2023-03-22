/* T.LOT  le 13-03-2023
 * contient 2 appels
 * setupGotoToSleepAndWakeUpByTimer() à lancer une foi au demarrage
 * la procedure GotoToSleepAndWakeUpByTimer(valeur en micro seconde) place l'ESP32 en veille profonde.
 * Le reveil s'effectue à la fin de "valeur en micro seconde" par un reboot systeme complet
 */
// initialisation du port de sortie 
void setupGotoToSleepAndWakeUpByTimer()
{
  pinMode     (iCmdPowerI2cSensors , OUTPUT); // config de la broche en sortie 
  digitalWrite(iCmdPowerI2cSensors , HIGH  ); // regulateur actif
}

// c'est là que l'ESP va faire dodo si node red donne un délai en µS > à 0
void GotoToSleepAndWakeUpByTimer (uint32_t iTopicTimeToSleep)
    {
      if (iTopicTimeToSleep > 0 ) // si > 0 info via node red alors OK pour mise en veille 
      {
             if (bMenu == true ) {Serial.println("Parti faire dodo A+");}
             ShutdownAllSensors  ();
             esp_sleep_enable_timer_wakeup(iTopicTimeToSleep);   // recupére l'information de node Red pour le temp de mise en veille en µS (à definir)
             esp_deep_sleep_start();
             WakeUpAllSensors    ();         
      }
    }


void ShutdownAllSensors ()
{
   digitalWrite(iCmdPowerI2cSensors, LOW); // coupe le régulateur auxiliaire des capteurs 
   delay(100);                              // attend qu'ils soient tous coupés
}

//ne sert à rien vue que le reboot efface toutesles données avant la mise en someil profond
void WakeUpAllSensors ()
{
   digitalWrite(iCmdPowerI2cSensors, HIGH); // active le régulateur auxiliaire des capteurs 
   delay(500);                              // attend un peut leurs reveil charge des condo etc...
   setupHX711  ();
   setupINA226 ();
   setupBME280 ();
   setupDS18B20();
}
