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
