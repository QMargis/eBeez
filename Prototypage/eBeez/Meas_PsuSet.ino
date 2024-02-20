/*
 * L’ADC2 ne peut pas être utilisé avec le WiFi activé car il est utilisé en interne par le driver WiFi. 
 * Puisqu’il y a de grandes chances d’utiliser le WiFi sur un microcontrôleur prévu pour l’utiliser, il n’y a que l’ADC1 et ses 8 canaux qui peuvent être utilisés.
 * - GetVoltage () Fonction Retourne la tension de l'alimentation  
 */
//-------------------------------------------------------
// Configuration  via la broche 12 GPIO 14 de l'ESP32,
// de l'activation ou pas d'un régulateur 3.3V secondaire
//------------------------------------------------------- 
void SetupPeripherialPowerState()
  {
     pinMode      (14, OUTPUT);  //Il faut déclarer le pin en entrée
     digitalWrite (14, true  );  // sortie sur la broche 12 GPIO14
     vTaskDelay(500);            // Delais de mise en route des periphériques
  }

//------------------------------------------------------
// Appel de mise en route du regulateur 3.3V secondaire,
//------------------------------------------------------ 
void  PsuWakeUp()
  {
      digitalWrite (14, true) ;   // sortie sur la broche 12 GPIO14
      vTaskDelay(500);            // Delais de mise en route des periphériques
  }

//------------------------------------------------------
// Appel de coupure du regulateur 3.3V secondaire,
//------------------------------------------------------ 
void  PsuShutDown()
  {
      digitalWrite (14, false) ;   // sortie sur la broche 12 GPIO14
      vTaskDelay(1);               // Delais de mise en route des periphériques
  }

//------------------------------------------------------
// configuration de l'ADC sur 12 Bits
//------------------------------------------------------
void SetupConfigVoltage ()
  { 
      pinMode      (34, INPUT);   //Il faut déclarer le pin en entrée
      analogSetPinAttenuation(34,ADC_11db);
      analogReadResolution(12);   //Résolution entre 9-12 bits
      
  }

//--------------------------------------------------------------------------
// Effectue une mesure sous 12Bits de la tension d'alimentation 
// Au niveau de la batterie sous forme de fonction. raport de tension 0,1506
// Une moyenne est effectuée sur 10 echentillons pour lisser le bruit
//-------------------------------------------------------------------------- 

float GetVoltage ()
  {
    const  uint8_t  PinADC         = 34;                // GPIO 34 ADC6
           uint32_t iADC6          = analogRead (PinADC);
           float    fMesureVoltage = 0;
           float    fBridgeValue   = 0.0057  ;          // 0.1506 Valeur des convertion du pont diviseur pour ramener la valeur à Vmax de 3v en convertion max
           iADC6  = analogRead     (PinADC);
           
           for (uint8_t iCount  = 0 ; iCount < 10 ; iCount++) //moyenne pour lisser la tension à revoir à la baisse
              {    
                  iADC6 = iADC6 + analogRead (PinADC);        // fait 50 mesures
                  vTaskDelay(5);                             // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble
              }
           fMesureVoltage = iADC6/10 ; // fait la moyenne 
           fMesureVoltage = fMesureVoltage * fBridgeValue;
     return(fMesureVoltage);
  }
