
void setupMQTT()
{
  
}

void onConnectionEstablished()
{
  iConnectedMQTT = 1; 
  if (iDebug) {
    Serial.println("MQTT Connecté!");
  }
}

void readMQTT ()
{
  //   Subscribe to sTopic and display received message to Serial
  client.subscribe(sNomRuche + "/Parametres", [](const String & payload) {
    if (iDebug)
    {
      Serial.println(payload);
    }
    // Décode lecture paramètres format JSON : "sNomRuche","iTypeTrame","iDelayMin"
  }); 
}

void writeMQTT()
{
    // Publish a message to sTopic
  if (iDebug) {
    Serial.println("Topic: " + sTopic + sNomRuche);
  }
  client.publish(sTopic + sNomRuche, geneJSon(0)); // You can activate the retain flag by setting the third parameter to true
}
