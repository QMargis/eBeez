/*
//https://elttesters.com/cospas-beacon-decoder.html
//https://cospas-sarsat.int/en/beacons-pro/beacon-message-decode-program-txsep/beacon-decode-2019
//https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/ 
//https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/
// UploadSpeed 921600
// CPU Fréquency 80Mhz

 F4EQD le 03/09/2022: Démarrage des 1er test sur les Periphérique I2C et HX711
 F4EQD le 03/09/2022: Validation des lectures des 4 capteurs de poids avec une horloge en commun
                      Instalation d'une antenne wifi directiveà grand gain (Antenne Biquad)        
 F4EQD le 03/09/2022: Indication des valeur du capteur BME280 dans la page web
                      Idem pour les capteurs de poids 
 F4EQD le 27/09/2022: Integration du capteur de température DS18B20
                      Integration des informations dans la page Web embarquée.
 F4EQD le 27/03/2023: Mise en EEPROM des valeurs de calibration capteur à vide 
 F4EQD le 01/04/2023: Ajout bouton de sortie de veille pour le bébug.   
                      Ajout onglet Json_Bertrand reformate les données au format Json 
                      avant de les envoyer sur la base MQTT  
 F4EQD le 29/04/2023  Correction du bugg du verouillage de connection au serveur MQTT boucle infinie
                      Condition si pas de connection Wifi pas de passage dans le serveur MQTT 
 Thorvald-FR 07/2024  Ajout pages Web pour paramètrage, gestion du SPIFFS et OTA
                      Mise en EEPROM de toutes les variables paramètrables
                      Ajout du fonctionnement entre MQTT & JSON
                      Mise au propre
 Thorvald-FR 09/2024  Ajout de l'envoie en JSON
                      Ajout de la réception en JSON
*/

// Gestionnaire de cartes ESP32 : "esp32 by Espressif" V3.2.0
// Carte : ESP32 Wrover Module

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>    // http://arduino.cc/en/Reference/Wire Version 2.0.0
#include "HX711.h"   // https://github.com/RobTillaart/HX711 Version 0.6.0
#include <OneWire.h> // http://www.pjrc.com/teensy/td_libs_OneWire.html Version 2.3.8
#include <DallasTemperature.h> // https://github.com/milesburton/Arduino-Temperature-Control-Library Version 4.0.3
#include <Preferences.h> // https://github.com/vshymanskyy/Preferences Version 2.1.0
#include <ArduinoJson.h> // https://arduinojson.org/ Version 7.4.1
#include <PubSubClient.h> // https://pubsubclient.knolleary.net Version 2.8
#include "esp_private/esp_task_wdt.h"
#include "FS.h" // Version 2.0.0

// https://randomnerdtutorials.com/arduino-ide-2-install-esp32-littlefs/
// Le moniteur série doit être fermé pour faire le transfert
#include "LittleFS.h" // https://github.com/lorol/LITTLEFS Version 1.0.6

#include <WiFiClient.h> // http://www.arduino.cc/en/Reference/WiFi101 Version 0.16.1
#include <AsyncTCP.h> // https://github.com/ESP32Async/AsyncTCP Version 3.4.1
#include <ESPAsyncWebServer.h> // https://github.com/ESP32Async/ESPAsyncWebServer Version 3.7.7

// ElegantOTA installation : https://docs.elegantota.pro/getting-started/async-mode
#include <ElegantOTA.h> // https://github.com/ayushsharma82/ElegantOTA Version 3.1.7

#include <ESPmDNS.h> // Version 2.0.0
#include <Update.h> // Version 2.0.0

#include "manager/edit_html.h"
#include "manager/manager_html.h"
#include "manager/ok_html.h"
#include "manager/failed_html.h"

#include "init.h"

//3 seconds WDT
#define WDT_TIMEOUT 1200 // 20 min 

//----capteur BME280 température ,pression ,Humiditée ----------
//BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

//----HX711 circuit wiring---------------
HX711 scale;

// FileSystem
#define FORMAT_LITTLEFS_IF_FAILED true
#define SPIFFS LittleFS

// EEPROM
Preferences prefs;
#define ebeezSpace "ebeezSpace"
const boolean ReadOnly  = true;
const boolean ReadWrite = !ReadOnly; // not-operator inverts the value 

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

// Files Manager
String filesDropdownOptions = "";
String textareaContent = "";
String savePath = "";
String savePathInput = "";
String allowedExtensionsForEdit = "txt, h, htm, html, css, cpp, js";
const char* param_delete_path = "delete_path";
const char* param_edit_path = "edit_path";
const char* param_edit_textarea = "edit_textarea";
const char* param_save_path = "save_path";

//-----------serveur MQTT----------------
String mqtt_serveur  =       "";      // Adresse Base de donnée
int mqtt_port        =       35300;   // Port 
String mqtt_user     =       "";      // Log
String mqtt_pass     =       "";      // MDP

// Serveur WEB
AsyncWebServer server(80);

//-----------configuration Connexion Wifi Local----------------
String ssid_client  = "";
String pass_client  = "";

//-----------WiFi AP--------------------
String ssid_ap    = "";
String pass_ap    = "";
String wifi_mode  = "";

//-----------Création du client WiFi--------------
WiFiClient          wifiClient;
PubSubClient client(wifiClient);

// Login serveur Web
String webLogin   = "";
String webPass    = "";

//-----------Variables-----------------
String        rucheName;
long          lValCalibLoadCPT1  =  0;     // memorisation des valeurs de calibration
long          lValCalibLoadCPT2  =  0;
long          lValCalibLoadCPT3  =  0;
long          lValCalibLoadCPT4  =  0;     
float		      fAjustement        =  0;     // Coéfficient empirique d'ajustment du poids
unsigned long previousMillis     =  0;
unsigned long lInterval          =  30000;
uint8_t       iNbReconnections   =  0;
int           iNbPaquet          =  1;
uint8_t       LoopNumber         =  5;    // valeur pour le nombre de passage dans le loop avant mise en veille 
uint32_t      iTimeToSleep       =  0;    // 10 min de valeur de mise en veille 10*60sec a faire passer via node red plus tard 
//bool          StatusToConnect    =  false;
//bool          bStatusJsonDisplay =  false;
bool		      modeVeille		     =	false;

//-----------Variables gestion Json-----------------
char          sOutputJSon[200];  // Variable JSON avec les valeurs des capteurs 
String        cTopicWrite        =  "";
String        cTopicRead         =  "";
int           iDebug             =  1;    // 0: Debug désactivé  ; 1: Debug activé
int           iCptVirtuel        =  0;    // 0: Capteurs réelles ; 1: Simulation des capteurs
long          lDelayTrame        =  10;   // Délais entre deux trames en seconds
int           iTrame             =  0;
 
// ************************ //
// SETUP parametres initaux //
// ************************ //
void setup() 
{  
  Serial.begin(115200);
  setupGotoToWathDog();
  setupProm();                          // Setup de l'EEPROM et récupération des valeurs
  cTopicWrite =  rucheName + "/data";   // Topic d'envoi des informations de la ruche
  cTopicRead  =  rucheName + "/param";  // Topic de récepcion des parametres
  setupOTA();                           // Setup de l'OTA et pages Web
  SetupPeripherialPowerState();         // autorise la mise sous tension des periphériques I2C
  Wire.begin() ;                        // init du capteur DS18B20 capteur de température étanche.
  //setupI2C();                           // initialisation des adresse I2C
  SetupConfigVoltage();                 // configuration du covertisseur 12bits de l'esp32 pour la mesure de tension batterie
  //setupBME280();                        // initialisation du capteur de température exterieur + pression atmosphérique + humiditée
  setupDS18B20();                       // initialisation du capteur de température interne de la ruche DS18B20
  setupMQTT_Serveur();                  // initialisation de la connexion du serveur MQTT distant
  setupGotoToSleepAndWakeUpByTimer();   // initialisation du mode de mise en veille 
  setupHX711();                         // initialisation des convertisseur analogiques 24Bits des capteur de poids 
}

// ************************* //
// Boucle principale : LOOP  //
// ************************* //
void loop() 
{ 
  WifiReconnect();                                  // permet de tester la connection periodiquement et de se reconnecter si deconnection ou perte de réseau.

  // Envoi des trames
  int nbTrames = 5;                                 // Nombre de trames envoyées  
  while(nbTrames>0) {
    GotoClient_MQTT();                              // envois des données vers la base de données
    nbTrames--;
    vTaskDelay(1000*portTICK_PERIOD_MS);            // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble 
  }
  Serial.print("Lecture MQTT sur le topic : ");
  Serial.println(cTopicRead);
  int i=lDelayTrame;                                // Délai entre deux réception de trames  
  while(i>0) {
    client.loop();
    i--;
    vTaskDelay(1000/portTICK_PERIOD_MS);            // Tempo 1s  
  }
  if( modeVeille == true ) {                        // viens controler l'autorisation de mise en veille.
    Serial.println("Au dodo!"); 
    GotoToSleepAndWakeUpByTimer(iTimeToSleep);
  } 
  else { Serial.println("Veille désactivée"); }
  vTaskDelay(100);
}
