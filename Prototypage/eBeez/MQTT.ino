/*
 * encour de dev
 */

 
void setupMQTT_Serveur()
{
  client.setServer  (mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}


void callback(char* topic, byte *payload, unsigned int length) 
{
  if (iDebug) {
    Serial.println("-------new message from broker-----");
    Serial.print  ("channel:");
    Serial.println(topic);
    Serial.print  ("data:");  
    Serial.write  (payload, length);
    Serial.println();
  }
  if (String(topic) == String("ruche")) { }
}


void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
    {
        Serial.print("Essais de connexion du serveur MQTT...");
        // Create a random client ID
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) 
            {
              Serial.println("Connecté ");
              //Once connected, publish an announcement...
              //client.publish("/icircuit/presence/ESP32/", "hello world");
              client.publish("/icircuit/presence/ESP32/","hello world");
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

/* Ancien code sans JSON...
void GotoClient_MQTT()
{
    GoToClient(TemperatureInterneRuche(),3,1,0);  //Valeurs Capteur , Nbs chiffres avant la virgule , Nbs chiffres aprés la virgule , Rang du topic 
    GoToClient(MesureTemperature()      ,3,1,1);
    GoToClient(MesureHumidite()         ,3,0,2);
    GoToClient(totalWeight()            ,6,0,3);
    GoToClient(MesurePression()         ,3,1,4);
    GoToClient(WiFi.RSSI()              ,4,0,5);
}


// Transfert l'info vers le client MQTT
//
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
  if (MQTT_SerialOrder ==0) {client.publish(MQTT_SERIAL_PUBLISH_CH0, serialData);}
  if (MQTT_SerialOrder ==1) {client.publish(MQTT_SERIAL_PUBLISH_CH1, serialData);}
  if (MQTT_SerialOrder ==2) {client.publish(MQTT_SERIAL_PUBLISH_CH2, serialData);}
  if (MQTT_SerialOrder ==3) {client.publish(MQTT_SERIAL_PUBLISH_CH3, serialData);}
  if (MQTT_SerialOrder ==4) {client.publish(MQTT_SERIAL_PUBLISH_CH4, serialData);}
  if (MQTT_SerialOrder ==5) {client.publish(MQTT_SERIAL_PUBLISH_CH5, serialData);} 
  // Serial.println (MQTT_SerialOrder,"  ",serialData);
}
*/

void GotoClient_MQTT()
{  
  // Récupérartion du JSON
  geneJSon(99);
   
  // Publish a message to sTopic
  if(!client.connected()) {
      Serial.println("MQTT Not Connected!");
      reconnect();
  }
  if (iDebug) {
    Serial.print("JSON: ");
    Serial.println(sOutputJSon);
  }
  client.publish(sTopic, sOutputJSon, true);
  vTaskDelay(1);   // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble
}
 
