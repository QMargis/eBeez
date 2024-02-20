/*
  Fichier:      MarcheArretMoteurL298N
  Auteur:      (https://passionelectronique.fr/)
//https://elttesters.com/cospas-beacon-decoder.html
//https://cospas-sarsat.int/en/beacons-pro/beacon-message-decode-program-txsep/beacon-decode-2019
//https://randomnerdtutorials.com/esp32-useful-wi-fi-functions-arduino/ 
//https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/
// DOIT ESP32 DEVKIT V4
// UploadSpeed 921600
// CPU Fréquency 80Mhz
// Lors du chargement presser sur BOOT maintenir + Un appuis sur EN + relache + relache BOOT 
// pour AnalogWrite il faut la présence C:\Users\F4EQD\Documents\Arduino\libraries\ESP32_ESP32S2_AnalogWrite 
// tous les analogRead sont bloqués lorsque setupPageWeb (); est actif en fait çà merde dé qu'il y à appel à serveur web
// Il est possible que  pour le serveur , L'un des core soit utilisé à 100% interdisant certaines fonction Analogread hormis 
// les ports IO34 IO35 ADC1 ADC3 
//
// doit ESP32 Dev kit
//                                         ________________________________
//                                        |            ____      _____     |
//                                        |      |    |    |    |  |  |    | 
//                                        |      |    |    |    |  |  |    |
//                                        |      |____|    |____|  |  |    |
//                                        |                                |  
//                                       -|3V3                          GND|-38  
//                                      9-|RST           ESP32         IO23|-36    
//                                      5-|SUP                         IO22|-39 ------I2CSCL--- Retiré de la derniere version                                  
//                                      8-|SVN                          TXD|-41 ------Ne pas utiliser .réservé USB TX1
//                 mesure tension ---> 10-|IO34  ADC1 CH0               RXD|-40 ------Ne pas utiliser .réservé USB RX1
//                                     11-|IO35  ADC3 CH0              IO21|-42 ------I2CSDA--- Retiré de la derniere version
//                   DS18B20 Data ---> 12-|IO32                         GND|-32    
// ButonWakeUp aprés mise en veille-->  8-|IO33                        IO19|-38    
//                                     14-|IO25                        IO18|-35    DAT 4 HX711 
//                                     15-|IO26                         IO5|-34    DAT 3 HX711 
//                                     16-|IO27                        IO17|-27    DAT 2 HX711 
//   Coupe les alims des capteurs ---> 17-|IO14                        IO16|-25    DAT 1 HX711 
//                empeche la prog      18-|IO12                         IO4|-24    CLK   HX711
//                                       -|GND                          IO0|-23    lui aussi c'est la merde çà telecharge pas avec 3K en ligne çà marche ---->
//                                     20-|IO13                         IO2|-22    (inutilisatble avec page Webb)
//                Ne pas utiliser      28-|SD2                         IO15|-21    inutilisatble avec page Webb
//                Ne pas utiliser      29-|SD3                          SD1|-33    Ne pas utiliser         
//                Ne pas utiliser      30-|CMD                          SD0|-32    Ne pas utiliser         
//                                       -|5V        DevKit V1          SCK|-31    Ne pas utiliser         
//                                        |________________________________|
//
//----------------------------------------------------------------------------
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
 F4EQD le 29/04/2023  Correction du bugg du verouillage de connection au serveur MQTT bouclr infinie
                      Condition si pas de connection Wifi pas de passage dans le sergveur MQTT                   
*/

#include "WiFi.h"
#include <Wire.h>    //include Wire.h library
#include "HX711.h"   //include load Amplifier
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <esp_task_wdt.h>


//3 seconds WDT
#define WDT_TIMEOUT 1200 // 20 min 


//-----------serveur Web-----------------
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
//#include <WiFiUdp.h>  // pour récuperer une horloge sur  le net 
AsyncWebServer server(80);

// Definition de la taille de l'EEPROM pour les calibration des capteurs.
//
#define EEPROM_SIZE 128 // 32bits par capteurs X4

//----capteur BME280 température ,pression ,Humiditée ----------
#include <BME280I2C.h>
BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

//----INA219 Capteur de courant----------
//#include "INA226.h";
//INA226    INA(0x40);

//----HX711 circuit wiring---------------
const int LOADCELL_1_DOUT_PIN = 16;   //Capteur de poids n°1
const int LOADCELL_2_DOUT_PIN = 17;   //Capteur de poids n°2
const int LOADCELL_3_DOUT_PIN =  5;   //Capteur de poids n°3
const int LOADCELL_4_DOUT_PIN = 18;   //Capteur de poids n°4
const int LOADCELL_SCK_PIN    =  4;   //Capteur de poids Horloge
HX711 scale;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 32;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//-----------serveur MQTT----------------
// 
const char* mqtt_server =       "qmrs.ddns.net";      // Adresse Base de donnée
#define mqtt_port               1883                  // Port 
#define MQTT_USER               "tlot"                // Log
#define MQTT_PASSWORD           "test_safran"         // MDP

// les appels ci-dessous sont entierement à reprendre . çà fonctionne mais ce n'est pas la bonne méthode

//#define MQTT_SERIAL_PUBLISH_CH   "EbeezTopic/Ruche_1"
#define MQTT_SERIAL_PUBLISH_CH0  "EbeezTopic/TempInRuche_1"           // TX  Topic Emission Température interne de la ruche
#define MQTT_SERIAL_PUBLISH_CH1  "EbeezTopic/TempOutRuche_1"          // TX  Topic Emission Température Externe de la ruche
#define MQTT_SERIAL_PUBLISH_CH2  "EbeezTopic/HumOutRuche_1"           // TX  Topic Emission Huliditée Externe de la ruche
#define MQTT_SERIAL_PUBLISH_CH3  "EbeezTopic/WeightSensorRuche_1"     // TX  Topic Emission Poids total de la ruche
#define MQTT_SERIAL_PUBLISH_CH4  "EbeezTopic/PressureSensorRuche_1"   // TX  Topic Emission Pression atmosphérique
#define MQTT_SERIAL_PUBLISH_CH5  "EbeezTopic/RSSI_Ruche_1"            // TX  Topic Emission Niveau de reception Wifi
#define MQTT_SERIAL_PUBLISH_CH6  "EbeezTopic/PowerSupplyRuche_1"      // TX  Topic Emission Niveau de tension 

#define MQTT_SERIAL_PUBLISH_CH10 "EbeezTopic/JsonDataTransfert"       // TX  Topic Emission de la trame Json
#define MQTT_SERIAL_PUBLISH_CH11 "EbeezTopic/measWeight1_Ruche_1"     // TX  Topic Emission Poids capteur n°1
#define MQTT_SERIAL_PUBLISH_CH12 "EbeezTopic/measWeight2_Ruche_1"     // TX  Topic Emission Poids capteur n°2
#define MQTT_SERIAL_PUBLISH_CH13 "EbeezTopic/measWeight3_Ruche_1"     // TX  Topic Emission Poids capteur n°3
#define MQTT_SERIAL_PUBLISH_CH14 "EbeezTopic/measWeight4_Ruche_1"     // TX  Topic Emission Poids capteur n°4

#define MQTT_SERIAL_RECEIVER_CH  "EbeezTopic/ReceiveDataFromNR"       // Rx  Topic Reception ne fonctionne pas à étudier


WiFiClient          wifiClient;
PubSubClient client(wifiClient);

//-----------configuration Connexion (multi)Wifi Local----------------
//
const char*   ssid0        =    "HUAWEI-8FA5"       ;
const char*   password0    =    "118A09TJ6BL"       ;      // 118A09TJ6BL
const char*   ssid1        =    "JaimePasLeChocolat";      // JaimePasLeChocolat
const char*   password1    =    "B*Zail3520*"       ;      //  B*Zail3520*
const char*   ssid2        =    "AU_FOND_A_DROITE"  ; 
const char*   password2    =    "*Zail3520*"        ;      // *Zail3520*

//-----------configuration de la broche de pilotage de l'alim des capteurs----------------

const int     iCmdPowerI2cSensors = 33;   // broche pour activer une regulateur pilotable 3V3 exclusivement pour les périphériques 

//-----------Variables-----------------
long          lValCalibLoadCPT1  =  0;     // memorisation des valeurs de calibration
long          lValCalibLoadCPT2  =  0;
long          lValCalibLoadCPT3  =  0;
long          lValCalibLoadCPT4  =  0;     
String        sLectureSerie      =  "NAS";  // buffeur de lecture sur le com
String        sSSID_Select       =  "NAS";  // SSID trouvé par l'ESP32
const float   fAjutement         =  0.565;  // Coéfficient empirique d'ajustment du poids
unsigned long previousMillis     =  0;
unsigned long lInterval          =  30000;
uint8_t       iNbReconnections   =  0;
bool          bMenu              =  true;
int           iNbPaquet          =  1;
uint8_t       LoopNumber         =  20;    // valeur pour le le nombre de passage dans le loop avant mise en veille 
uint32_t      iTimeToSleep       = 600;    // 10 min de valeur de mise en veille 10*60sec  a fair epasser via node red plus tard 
bool          StatusToConnect    = false ;
bool          bStatusJsonDisplay = false ;

//-----------Variables gestion Json-----------------

char          sOutputJSon[150];  // demander à Bertrand à quoi cela sert 
const char*   cTopicWrite        =  "Ruche/Virtuel";
const char*   cTopicRead         =  "Ruche/Virtuel/Params";
int           iDebug             =  1; // 0: Debug désactivé  ; 1: Debug activé
int           iCptVirtuel        =  1; // 0: Capteurs réelles ; 1: Simulation des capteurs
long          lDelayTrameMin     =  1; // Délais entre deux trames
int           iTrame             =  0;

 
//*************************//
// SETUP parametres initaux//
//*************************//

void setup() 
{  
  setupGotoToWathDog();
  bMenu = true ;                       // Autorise les retours sur le terminal série si connecté
  Serial.begin(115200);
  delay (500);                         // attente liaison com Stable
  SetupPeripherialPowerState();        // autorise la mise sous tension des periphériques I2C
  Wire.begin() ;                       // init du capteur DS18B20 capteur de température étanche.
  setupEEPROM();                       // initialisation des affectations des zones ^émoires non volatiles du processeur
  setupI2C();                          // initialisation des adresse I2C
  setupHX711 ();                       // initialisation des convertisseur analogiques 24Bits des capteur de poids 
  SetupConfigVoltage ();               // configuration du covertisseur 12bits de l'esp32 pour la mesure de tension batterie
  setupBME280  ();                     // initialisation du capteur de température exterieur + pression atmosphérique + humiditée
  setupDS18B20 ();                     // initialisation du capteur de température interne de la ruche  DS18B20
  setupPageWeb ();                     // a faire en dernier dans le setup
  setupMQTT_Serveur();                 // initialisation de la connexion du serveur MQTT distant
  setupGotoToSleepAndWakeUpByTimer;    // initialisation du mode de mise en veille 
  bMenu = false ;                      // pour limiter les retours d'information inutile et economiser un un poil de jus
  Menu();                              // si bMenu = false affichades informations minimale sur la terminal si connecté
  SleepTimeSaveInEEPROM(iTimeToSleep); // 10*60Sec  à passer en variable par Node Red. non fonctionnel pour le moment 
}

//**************************//
// Boucle principale : LOOP //
//**************************//
void loop() 
{  
    WifiReconnect();     // permet de tester la connection periodiquement et de se reconnecter si deconnection ou perte de réseau.
    SerialRead();        // lecture des commande saisie dans le terminal
    geneJSON(iTrame);    // traduction en trame Json avec les données collectées 
    geneJSON(iTrame+10); // traduction en trame Json avec les données collectées le + 10 est pour attaquer le case 10 
    GotoClient_MQTT();   // envois des données vers la base de données
    vTaskDelay(1);       // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble 
    RepeatLoopBeforDeepSleep(iTimeToSleep);  // viens controler l'autorisation de mise en veille.
}
