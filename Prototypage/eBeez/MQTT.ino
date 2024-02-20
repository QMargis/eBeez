/*
 * pas tout compris sur le fonctionnement
 */
void setupMQTT_Serveur()
{
  client.setServer  (mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}


void reconnect() 
{

if (WiFi.status() == WL_CONNECTED) 
  {
        // Loop until we're reconnected
      while (!client.connected()) 
        {
            Serial.print(F("Essais de connexion du serveur MQTT..."));
            String clientId = "E-BezzClient";
            //clientId += String(random(0xffff), HEX);
            // Attempt to connect
            if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) 
                {
                  Serial.println(F("Connecté "));
                  //Once connected, publish an announcement...
                  //client.publish("/icircuit/presence/ESP32/","E-Bezz");
                  // ... and resubscribe
                  client.subscribe(MQTT_SERIAL_RECEIVER_CH);
                }
            else 
                {
                  Serial.print("failed, rc=");
                  Serial.print(client.state());
                  Serial.println(" try again in 5 seconds");
                  // Wait 5 seconds before retrying
                  delay(5000);
                }
         }
      }
  }
//------------------------------------------------------------------------
// recupére et formate les datas des fonctions appelées dans le programme 
//------------------------------------------------------------------------
void GotoClient_MQTT()
{
    GoToClient(TemperatureInterneRuche(),3,1,0);  //Valeurs Capteur , Nbs chiffres avant la virgule , Nbs chiffres aprés la virgule , Rang du topic MQTT_SERIAL_PUBLISH_CHxx
    GoToClient(MesureTemperature()      ,3,1,1);
    GoToClient(MesureHumidite()         ,3,0,2);
    GoToClient(totalWeight()            ,6,0,3);
    GoToClient(MesurePression()         ,3,1,4);
    GoToClient(WiFi.RSSI()              ,4,0,5);
    GoToClient(GetVoltage ()            ,2,3,6);
    GoToClient(measWeight1()            ,6,0,11);  //ATTENTION les valeurs sont transferées en milligrames
    GoToClient(measWeight2()            ,6,0,12);  //ATTENTION les valeurs sont transférées en milligrames
    GoToClient(measWeight3()            ,6,0,13);  //ATTENTION les valeurs sont transférées en milligrames
    GoToClient(measWeight4()            ,6,0,14);  //ATTENTION les valeurs sont transférées en milligrames
}

//------------------------------------------
// Transfert l'info vers le client MQTT
//------------------------------------------
void GoToClient(float fTransfertData, uint8_t iBeforPoint, uint8_t iAfterPoint, uint8_t MQTT_SerialOrder) 
 {    
      char sTransfertData[10];
      dtostrf(fTransfertData, iBeforPoint, iAfterPoint, sTransfertData);
      publishSerialData(sTransfertData,MQTT_SerialOrder);
      vTaskDelay(1);   // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble   
 }

void publishSerialData(char *serialData, uint8_t MQTT_SerialOrder)
{
  if (!client.connected()) 
  {
    reconnect();
  }
  if (MQTT_SerialOrder == 0) {client.publish(MQTT_SERIAL_PUBLISH_CH0  , serialData);}
  if (MQTT_SerialOrder == 1) {client.publish(MQTT_SERIAL_PUBLISH_CH1  , serialData);}
  if (MQTT_SerialOrder == 2) {client.publish(MQTT_SERIAL_PUBLISH_CH2  , serialData);}
  if (MQTT_SerialOrder == 3) {client.publish(MQTT_SERIAL_PUBLISH_CH3  , serialData);}
  if (MQTT_SerialOrder == 4) {client.publish(MQTT_SERIAL_PUBLISH_CH4  , serialData);}
  if (MQTT_SerialOrder == 5) {client.publish(MQTT_SERIAL_PUBLISH_CH5  , serialData);} 
  if (MQTT_SerialOrder == 6) {client.publish(MQTT_SERIAL_PUBLISH_CH6  , serialData);} 
  
  if (MQTT_SerialOrder ==11) {client.publish(MQTT_SERIAL_PUBLISH_CH11 , serialData);} 
  if (MQTT_SerialOrder ==12) {client.publish(MQTT_SERIAL_PUBLISH_CH12 , serialData);}
  if (MQTT_SerialOrder ==13) {client.publish(MQTT_SERIAL_PUBLISH_CH13 , serialData);}
  if (MQTT_SerialOrder ==14) {client.publish(MQTT_SERIAL_PUBLISH_CH14 , serialData);} 
  // client.setCallback(callback);
}

//------------------------------------------------
// retour provenant de Node red . a develloper
//------------------------------------------------
void callback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) 
  {
    Serial.print  ((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == MQTT_SERIAL_RECEIVER_CH) 
      {
    Serial.print("Changing output to ");
    if(messageTemp == "on")
      {
         Serial.println("on");
      }
    else if(messageTemp == "off")
      {
         Serial.println("off");
      }
  }
}

 
