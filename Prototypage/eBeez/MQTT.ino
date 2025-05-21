/*
 * pas tout compris sur le fonctionnement
 */
void setupMQTT_Serveur()
{
  client.setServer(mqtt_serveur.c_str(), mqtt_port);
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
            if (client.connect(clientId.c_str(),mqtt_user.c_str(),mqtt_pass.c_str()))
                {
                  Serial.println(F("Connecté "));
                  //Once connected, publish an announcement...
                  //client.publish("/icircuit/presence/ESP32/","E-Bezz");
                  // ... and resubscribe
                  //client.subscribe(cTopicWrite.c_str()); // Topic d'écriture
                  client.subscribe(cTopicRead.c_str()); // Topic de lecture
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
  if (!client.connected()) { reconnect(); }

  // Récupérartion du JSON
  geneJSON(iTrame);

  if (iDebug) {
    Serial.print("Topic: ");
    Serial.println(cTopicWrite.c_str());
    Serial.print("JSON: ");
    Serial.println(sOutputJSon);
  }
  
  // Publish a message to cTopicWrite
  client.publish(cTopicWrite.c_str(), sOutputJSon, true);

  if(iTrame==10) { iTrame=0; } // Pour ne faire qu'une calibration
  vTaskDelay(1);   // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble

}

//------------------------------------------------
// retour provenant de Node red . a develloper
//------------------------------------------------
void callback(char* topic, byte* message, unsigned int length) 
{
  Serial.print("Message arrivé sur le topic : ");
  Serial.println(topic);

  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) 
  {
    Serial.print  ((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  decodeJSON((char*)message, length);
} 
