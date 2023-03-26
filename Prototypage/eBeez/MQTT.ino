/*
 * encour de dev
 */

 
void setupMQTT_Serveur()
{
  client.setServer(mqtt_server, mqtt_port);
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
  
  decodeJSON((char*)payload, length);
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
              //client.publish(cTopicWrite,"hello world");
              // ... and resubscribe
              client.subscribe(cTopicRead);
              Serial.print("TopicRead: ");
              Serial.println(cTopicRead);
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

void GotoClient_MQTT()
{ 
  //client.disconnect();  
  reconnect();

  // Récupérartion du JSON
  geneJSON(iTrame);
  
  if (iDebug) {
    Serial.print("JSON: ");
    Serial.println(sOutputJSon);
  }
  
  // Publish a message to cTopicWrite
  client.publish(cTopicWrite, sOutputJSon, true);
  if(iTrame==10) { iTrame=0; } // Pour ne faire qu'une calibration
  vTaskDelay(1);   // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble
  
}
 
