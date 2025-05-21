// permet de voir la presence des capteur de poids 
/* T.LOT  le 13-03-2023
 * contient 6 appels
 * setupHX711 () à lancer une foi au demarrage
 * LoadMesurement() affiche dans le serial monitor l'état des resultats retournés par les 4 capteurs.utilisé pour le débug
 * long measWeight1() retournent une valeur en milligramme
 * long measWeight2() retournent une valeur en milligramme
 * long measWeight3() retournent une valeur en milligramme
 * long measWeight4() retournent une valeur en milligramme
 * long totalWeight() retourne la somme des 4 capteurs en gramme
 * LoadCalibrationAllHX711 () effectue la mise à 0 des valeurs de la somme des capteurs
 * 
 */

void setupHX711 () // comme il le dit !
  {
    scale.begin(LOADCELL_1_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(5);
    Serial.print("Setup - Capteur de poids n°1 ");
    if (scale.is_ready()) {Serial.println("OK");}
    else                  {Serial.println("Non trouvé !"); }
  
    scale.begin(LOADCELL_2_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(5);
    Serial.print("Setup - Capteur de poids n°2 ");
    if (scale.is_ready()) {Serial.println("OK");}
    else                  {Serial.println("Non trouvé !"); }

    scale.begin(LOADCELL_3_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(5);
    Serial.print("Setup - Capteur de poids n°3 ");
    if (scale.is_ready()) {Serial.println("OK");}
    else                  {Serial.println("Non trouvé !"); }
  
    scale.begin(LOADCELL_4_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(5);
    Serial.print("Setup - Capteur de poids n°4 ");
    if (scale.is_ready()) {Serial.println("OK");}
    else                  {Serial.println("Non trouvé !"); }  
}

//----------------------------------------------------------
// mesures des 4 capteurs de poids
// pour reduire le nombres de broche limité de l'ESP
// les CLK sont montées en // 
//----------------------------------------------------------
void LoadMesurement() 
{
    scale.begin(LOADCELL_1_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long lReading1 = 0;
    lReading1 = scale.get_units(10);
    lReading1 = lReading1 - lValCalibLoadCPT1;

    scale.begin(LOADCELL_2_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long lReading2 = 0;
    lReading2 = scale.get_units(10);
    lReading2 = lReading2 - lValCalibLoadCPT2;
    
    scale.begin(LOADCELL_3_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long lReading3 = 0;
    lReading3 = scale.get_units(10);
    lReading3 = lReading3 - lValCalibLoadCPT3;
    
    scale.begin(LOADCELL_4_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(1); 
    long lReading4 = 0;
    lReading4 = scale.get_units(10);
    lReading4 = lReading4 - lValCalibLoadCPT4;
}

//---------------------------------------------------
// cette fonction retourne le poids du 1er capteur 
//---------------------------------------------------
long measWeight1()
{
    scale.begin(LOADCELL_1_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(1); 
    long lReading = 0;
    lReading = scale.get_units(10);
    vTaskDelay(1);
    lReading      = lReading - lValCalibLoadCPT1;
    lReading      = lReading * fAjustement; 
    return (lReading);
}

//---------------------------------------------------
// cette fonction retourne le poids du 2em capteur 
//---------------------------------------------------
long measWeight2()
{
    scale.begin(LOADCELL_2_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(1); 
    long lReading = 0;
    lReading = scale.get_units(10);
    vTaskDelay(1);
    lReading      = lReading - lValCalibLoadCPT2;
    lReading      = lReading * fAjustement;
    return (lReading);
}

//---------------------------------------------------
// cette fonction retourne le poids du 3em capteur 
//---------------------------------------------------
long measWeight3()
{
    scale.begin(LOADCELL_3_DOUT_PIN, LOADCELL_SCK_PIN);
    vTaskDelay(1);
    long lReading = 0;
    lReading = scale.get_units(10);
    vTaskDelay(1); 
    lReading      = lReading - lValCalibLoadCPT3;
    lReading      = lReading * fAjustement;
    return (lReading);
}

//---------------------------------------------------
// cette fonction retourne le poids du 4em capteur 
//---------------------------------------------------
long measWeight4()
{
    scale.begin(LOADCELL_4_DOUT_PIN, LOADCELL_SCK_PIN); 
    vTaskDelay(1);
    long lReading = 0;
    lReading = scale.get_units(10);
    vTaskDelay(1); 
    lReading      = lReading - lValCalibLoadCPT4;
    lReading      = lReading * fAjustement;
    return (lReading);
}

//------------------------------------------------
// Calcul du poid total + traduction en grammes 
//------------------------------------------------
long totalWeight()  //sommes des 4 capteurs de poids
{
    long measTotalWeight = 0;
    measTotalWeight = measWeight1() + measWeight2() + measWeight3() + measWeight4();
    measTotalWeight = measTotalWeight/1000;   // retour la valeur en grammes
    return (measTotalWeight);
}

//------------------------------------------------
// Calibration
// mesure a vide de capteurs et mise en méroire
// non volatile EEPROM
//------------------------------------------------
void LoadCalibrationAllHX711 ()
{
    scale.begin(LOADCELL_1_DOUT_PIN, LOADCELL_SCK_PIN); 
    long lReading1    = 10;
    lReading1         = scale.get_units(10);
    lValCalibLoadCPT1 = lReading1;
    vTaskDelay(1);
    
    scale.begin(LOADCELL_2_DOUT_PIN, LOADCELL_SCK_PIN); 
    long lReading2    = 11;
    lReading2         = scale.get_units(10);
    lValCalibLoadCPT2 = lReading2;
    vTaskDelay(1);
    
    scale.begin(LOADCELL_3_DOUT_PIN, LOADCELL_SCK_PIN);
    long lReading3    = 12;
    lReading3         = scale.get_units(10);
    lValCalibLoadCPT3 = lReading3;
    vTaskDelay(1);
    
    scale.begin(LOADCELL_4_DOUT_PIN, LOADCELL_SCK_PIN); 
    long lReading4    = 13;
    lReading4         = scale.get_units(10);
    lValCalibLoadCPT4 = lReading4;
    vTaskDelay(1);
}


   
