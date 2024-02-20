/*
 * T.LOT le 13-03-2023
 * contient 2 appels
 * setupDS18B20 () à lancer une fois au demarrage
 * la fonction TemperatureInterneRuche() retoune un float 
 * - TemperatureInterneRuche() Fonction Retourne la température en float
 * Exemple d'appel serielp.print(TemperatureInterneRuche());retourne 28.5
 */

void setupDS18B20 ()
{
    // Start the DS18B20 sensor
  sensors.begin();
}

float TemperatureInterneRuche()
{
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  vTaskDelay(1);
  return (temperatureC);
}
