void setupHX711 ()
{
  scale.begin(LOADCELL_1_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.print("Capteur de poids n°1 ");
  if (scale.is_ready()) {Serial.println("OK");}
  else                  {Serial.println("Non trouvé !"); }
  
  scale.begin(LOADCELL_2_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.print("Capteur de poids n°2 ");
  if (scale.is_ready()) {Serial.println("OK");}
  else                  {Serial.println("Non trouvé !"); }

  scale.begin(LOADCELL_3_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.print("Capteur de poids n°3 ");
  if (scale.is_ready()) {Serial.println("OK");}
  else                  {Serial.println("Non trouvé !"); }

  scale.begin(LOADCELL_4_DOUT_PIN, LOADCELL_SCK_PIN);
  Serial.print("Capteur de poids n°4 ");
  if (scale.is_ready()) {Serial.println("OK");}
  else                  {Serial.println("Non trouvé !"); }  
}


void LoadMesurement() 
{
    scale.begin(LOADCELL_1_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long reading1 = scale.get_units(10);
    reading1 = reading1 - ValCalibLoadCPT1;
    Serial.print("CPT 1 : ");
    Serial.print(reading1);

    scale.begin(LOADCELL_2_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long reading2 = scale.get_units(10);
    reading2 = reading2 - ValCalibLoadCPT2;
    Serial.print("  CPT 2 : ");
    Serial.print(reading2);
    
    scale.begin(LOADCELL_3_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long reading3 = scale.get_units(10);
    reading3 = reading3 - ValCalibLoadCPT3;
    Serial.print("  CPT 3 : ");
    Serial.print(reading3);
    
    scale.begin(LOADCELL_4_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(1); 
    long reading4 = scale.get_units(10);
    reading4 = reading4 - ValCalibLoadCPT4;
    Serial.print("  CPT4 : ");
    Serial.print(reading4);
    Serial.println("");   
}

// cette fonction retourne le poids du 1er capteur 
long measWeight1()
{
   scale.begin(LOADCELL_1_DOUT_PIN, LOADCELL_SCK_PIN);
   vTaskDelay(1); 
    long reading1 = scale.get_units(2);
    reading1      = reading1 - ValCalibLoadCPT1;
    reading1      = reading1 * fAjutement; 
    return (reading1);
}

// cette fonction retourne le poids du 1er capteur 
long measWeight2()
{
   scale.begin(LOADCELL_2_DOUT_PIN, LOADCELL_SCK_PIN);
   vTaskDelay(1); 
    long reading2 = scale.get_units(2);
    vTaskDelay(1);
    reading2      = reading2 - ValCalibLoadCPT2;
    reading2      = reading2 * fAjutement;
    return (reading2);
}

long measWeight3()
{
   scale.begin(LOADCELL_3_DOUT_PIN, LOADCELL_SCK_PIN);

    long reading3 = scale.get_units(2);
    vTaskDelay(1); 
    reading3      = reading3 - ValCalibLoadCPT3;
    reading3      = reading3 * fAjutement;
    return (reading3);
}

long measWeight4()
{
   scale.begin(LOADCELL_4_DOUT_PIN, LOADCELL_SCK_PIN); 
   vTaskDelay(1);
    long reading4 = scale.get_units(2);
    vTaskDelay(1); 
    reading4      = reading4 - ValCalibLoadCPT4;
    reading4      = reading4 * fAjutement;
    return (reading4);
}

// Calcul du poid total
long totalWeight()  //sommes des 4 capteurs de poids
{
    long measTotalWeight = 0;
    measTotalWeight = measWeight1() + measWeight2() + measWeight3() + measWeight4();
    //Serial.println(measTotalWeight);
    measTotalWeight = measTotalWeight/100;
    return (measTotalWeight);
}

bool LoadCalibrationAllHX711 ()
{
    Serial.println("Calibration en cours");
    scale.begin(LOADCELL_1_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long reading1 = scale.get_units(10);
    ValCalibLoadCPT1 = reading1;
    
    scale.begin(LOADCELL_2_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long reading2 = scale.get_units(10);
    ValCalibLoadCPT2 = reading2;
    
    scale.begin(LOADCELL_3_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(1); 
    long reading3 = scale.get_units(10);
    ValCalibLoadCPT3 = reading3;
    
    scale.begin(LOADCELL_4_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long reading4 = scale.get_units(10);
    ValCalibLoadCPT4 = reading4;
    Serial.println("Calibration terminée");

    return true;
}
