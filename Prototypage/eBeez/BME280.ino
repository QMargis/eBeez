// T.LOT le 13-03-2023
/*
 * contient 6 appels
 * setupBME280 à lancer une fois au demarrage
 * printBME280Data pour le debug permet de sortir les infos dans le terminal a suprimer à la livraison
 * les fonction MesureTemperature,MesureHumidite et MesurePression retounent un float 
 * Exemple d'appel serielp.print(MesureTemperature());retourne 28.5
 * Liste des appels
 * - setupBME280()  initialisation du capteur 
 * - MesureTemperature() Fonction Retourne la température en float
 * - MesureHumidite()    Fonction Retourne le niveaau d'humiditée en float
 * - MesurePression()    Fonction Retourne le niveaau de pression atmosphérique en float
 */

//-----------------------------------------------------
// Controle de la présence du capteur 
//-----------------------------------------------------

void setupBME280()
{
 if (!bme.begin())
  {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }

  switch(bme.chipModel())
  {
     case BME280::ChipModel_BME280:
       Serial.println("Found BME280 sensor! Success.");
       break;
     case BME280::ChipModel_BMP280:
       Serial.println("Found BMP280 sensor! No Humidity available.");
       break;
     default:
       Serial.println("Found UNKNOWN sensor! Error!");
  }
}

//------------------------------------------------
// Sort les mesures en un seul appel
// n'est plus utilisé dans le programme 
// voir a suprimmer à la livraison
//------------------------------------------------
void printBME280Data (Stream* client)
{
   float temp(NAN), hum(NAN), pres(NAN);

   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);

   bme.read(pres, temp, hum, tempUnit, presUnit);

   client->print("Temp: ");
   client->print(temp);
   client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
   client->print("\t\tHumidity: ");
   client->print(hum);
   client->print("% RH");
   client->print("\t\tPressure: ");
   client->print(pres);
   client->println("Pa");

   delay(1000);
}


void DataBme280 ()
{
   float temp(NAN), hum(NAN), pres(NAN);
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);   
}

//-----------------------------------------------------------
// Séparation des 3 elements de mesure pour de appels ciblés
//-----------------------------------------------------------


float MesureTemperature()  // à terminer
{
   float temp(NAN), hum(NAN), pres(NAN);
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);  
   return (temp);    
}

float MesureHumidite()  // à terminer
{
   float temp(NAN), hum(NAN), pres(NAN);
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);  
   return (hum);    
}

float MesurePression()  // à terminer
{
   float temp(NAN), hum(NAN), pres(NAN);
   float fConvertHPascal;
   BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
   BME280::PresUnit presUnit(BME280::PresUnit_Pa);
   bme.read(pres, temp, hum, tempUnit, presUnit);  
   fConvertHPascal = pres/100;
   return (fConvertHPascal);    
}
