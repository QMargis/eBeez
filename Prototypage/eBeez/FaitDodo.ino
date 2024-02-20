/* T.LOT  le 13-03-2023
 * contient 2 appels
 * setupGotoToSleepAndWakeUpByTimer() à lancer une foi au demarrage
 * la procedure GotoToSleepAndWakeUpByTimer(valeur en secondes) place l'ESP32 en veille profonde.
 * C'est même un coma !
 * Le reveil s'effectue à la fin de "valeur en micro seconde" par un reboot systeme complet
 */
// --------------------------------------------------------------------------------- 
// initialisation du port de sortie de l'ESP32 pour le controle du regulateur piloté
//----------------------------------------------------------------------------------

void setupGotoToSleepAndWakeUpByTimer()
{

    print_wakeup_reason();
    pinMode     (iCmdPowerI2cSensors , OUTPUT);  // config de la broche en sortie 
    digitalWrite(iCmdPowerI2cSensors , HIGH  );  // regulateur actif
    
  // normalement çà devrais réveiller l'esp par l'appuis sur le bouton (sous le boitier mais çà ne fonctionne pas !!!
  
    pinMode     (33 , INPUT_PULLDOWN);           // config de la broche en entrée Bouton WakeUp aprés mise en veille
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1); //1 = High, 0 = Low
}

//---------------------------------------------------------------------------------- 
// initialisation du Wouaff 
//----------------------------------------------------------------------------------
void setupGotoToWathDog()

{
    esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
    esp_task_wdt_add(NULL);               //add current thread to WDT watch
}



//--------------------------------------------------------------------------------
// c'est là que l'ESP va faire un coma si node red donne un délai en Seconde > à 0
//--------------------------------------------------------------------------------
void GotoToSleepAndWakeUpByTimer (uint32_t iTopicTimeToSleep)
    {
      iTopicTimeToSleep = iTopicTimeToSleep * 1000000;  // pour passer de µs en seconde
      if (iTopicTimeToSleep > 0 ) // si > 0 info via node red alors OK pour mise en veille 
          { 
             bMenu = false ;                                     // coupe les retours sur le terminal
             esp_sleep_enable_ext0_wakeup(GPIO_NUM_33,1);        // 1 = High, 0 = Low  devrais faire sortir de la veille mais ne fonctionne pas
             PsuShutDown();                                      // Coupe l'alimentaion auxiliaire 

             esp_sleep_enable_timer_wakeup(iTopicTimeToSleep);   // recupére l'information de node Red pour le temp de mise en veille en µS (à definir)
             Serial.print(F("Deconnexion reseau Wifi : "));Serial.println(sSSID_Select);
             Serial.print  (F("Parti faire dodo... A dans "));
             Serial.print  (iTopicTimeToSleep/1000000);
             Serial.println(F(" Secondes... A+ "));
             WiFi.disconnect();                                  // Annonce au routeur une deconnexion           
             delay(1000);
             esp_deep_sleep_start();                             // Par dormir 
          }
    }

 void Backdoor ()
 {
  // a réfléchir pour sortir de cette folie
 }
//-------------------------------------------------
// donne le pourquoi de sortie de veille  ou pas !
// ------------------------------------------------
 void print_wakeup_reason()
 {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0     : Serial.println(F("Wakeup caused by external signal using RTC_IO"))       ; break;
    case ESP_SLEEP_WAKEUP_EXT1     : Serial.println(F("Wakeup caused by external signal using RTC_CNTL"))     ; break;
    case ESP_SLEEP_WAKEUP_TIMER    : Serial.println(F("Wakeup caused by timer"))                              ; break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println(F("Wakeup caused by touchpad"))                           ; break;
    case ESP_SLEEP_WAKEUP_ULP      : Serial.println(F("Wakeup caused by ULP program"))                        ; break;
    default                        : Serial.printf ("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

// mise en EEPROM à l'adresse 32 du temps de mise en veille  ( à Valider )

void SleepTimeSaveInEEPROM (long SleepTimeSet) //Valeur en secondes
{
     EepromDataWrite ( 32,SleepTimeSet ); 
}

//---------------------------------------------------------------------------
// fait 20 passages dans le programme (Loop) avant une nouvelle mise en veille
// cela laisse le temps de prendre la main en cas de de besoin.
//--------------------------------------------------------------------------
void RepeatLoopBeforDeepSleep(uint32_t iTimer)
{      
    if ((LoopNumber == 0) && (bMenu == false)) {GotoToSleepAndWakeUpByTimer (iTimer);}
    if ( bMenu == false ) {Serial.print("Décompte avant veille : "); Serial.println(LoopNumber);LoopNumber--;}
}
