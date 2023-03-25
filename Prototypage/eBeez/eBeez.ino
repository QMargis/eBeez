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
//                                     10-|IO34  ADC1 CH0               RXD|-40 ------Ne pas utiliser .réservé USB RX1
//                                     11-|IO35  ADC3 CH0              IO21|-42 ------I2CSDA--- Retiré de la derniere version
//                   DS18B20 Data ---> 12-|IO32                         GND|-32    
//   Coupe les alims des capteurs ---> 13-|IO33                        IO19|-38    
//                                     14-|IO25                        IO18|-35    DAT 4 HX711 
//                                     15-|IO26                         IO5|-34    DAT 3 HX711 
//                                     16-|IO27                        IO17|-27    DAT 2 HX711 
//                                     17-|IO14                        IO16|-25    DAT 1 HX711 
//              empeche la prog        18-|IO12                         IO4|-24    CLK   HX711
//                                       -|GND                          IO0|-23    lui aussi c'est la merde çà telecharge pas avec 3K en ligne çà marche ----
//                                     20-|IO13                         IO2|-22    (inutilisatble avec page Webb)
//              Ne pas utiliser        28-|SD2                         IO15|-21    inutilisatble avec page Webb
//              Ne pas utiliser        29-|SD3                          SD1|-33    Ne pas utiliser         
//              Ne pas utiliser        30-|CMD                          SD0|-32    Ne pas utiliser         
//                                       -|5V                           SCK|-31    Ne pas utiliser         
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
*/

#include "WiFi.h"
#include <Wire.h>  //include Wire.h library
#include "HX711.h" //include load Amplifier
#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

//-----------serveur Web-----------------
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
//#include <WiFiUdp.h>  // pour récuperer une horloge sur  le net 
AsyncWebServer server(80);

// Definition de la taille de l'EEPROM pour les calibration des capteurs.
//
#define EEPROM_SIZE 64 // 32bits par capteurs X4

//----capteur BME280 température ,pression ,Humiditée ----------
#include <BME280I2C.h>
BME280I2C bme;    // Default : forced mode, standby time = 1000 ms
                  // Oversampling = pressure ×1, temperature ×1, humidity ×1, filter off,

//----INA219 Capteur de courant----------
#include "INA226.h";
INA226    INA(0x40);

//----HX711 circuit wiring---------------
const int LOADCELL_1_DOUT_PIN = 16;   //Capteur de poids n°1
const int LOADCELL_2_DOUT_PIN = 17;   //Capteur de poids n°2
const int LOADCELL_3_DOUT_PIN =  5;   //Capteur de poids n°3
const int LOADCELL_4_DOUT_PIN = 18;   //Capteur de poids n°4
const int LOADCELL_SCK_PIN    =  4;   //Capteur de poids n°5
HX711 scale;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 32;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//-----------serveur MQTT----------------
// 
const char* mqtt_server =       "qmrs.ddns.net";  // Adresse Base de donnée
#define mqtt_port               1883              // Port 
#define MQTT_USER               "tlot"            // Log
#define MQTT_PASSWORD           "test_safran"     // MDP
#define MQTT_SERIAL_PUBLISH_CH  "EbeezTopic/Ruche_1"   // TX  Topic Emission
#define MQTT_SERIAL_RECEIVER_CH "mytopic/test1"   // Rx  Topic Reception

/* Ancien code sans JSON...
// les appels ci-dessous sont entierement à reprendre . çà fonctionne mais ce n'est pas la bonne méthode
#define MQTT_SERIAL_PUBLISH_CH0  "EbeezTopic/TempInRuche_1"   // TX  Topic Emission
#define MQTT_SERIAL_PUBLISH_CH1  "EbeezTopic/TempOutRuche_1"   // TX  Topic Emission
#define MQTT_SERIAL_PUBLISH_CH2  "EbeezTopic/HumOutRuche_1"   // TX  Topic Emission
#define MQTT_SERIAL_PUBLISH_CH3  "EbeezTopic/WeightSensorRuche_1"   // TX  Topic Emission
#define MQTT_SERIAL_PUBLISH_CH4  "EbeezTopic/PressureSensorRuche_1"   // TX  Topic Emission
#define MQTT_SERIAL_PUBLISH_CH5  "EbeezTopic/RSSI_Ruche_1"   // TX  Topic Emission
*/

//--Variable de temps pour la mise en sommeil de l'ESP--Variable à revoir ! -----
//#define uS_TO_S_FACTOR 1000000 // Conversion factor for micro seconds to seconds     
//#define TIME_TO_SLEEP  3       // Time ESP32 will go to sleep (in seconds)

WiFiClient wifiClient;
PubSubClient client(wifiClient);

//-----------configuration Connexion (multi)Wifi Local----------------
//
const char*   ssid0        =  "xxxx";
const char*   password0    =  "xxxx";
const char*   ssid1        =  "xxxx";
const char*   password1    =  "xxxx";
const char*   ssid2        =  "xxxx";
const char*   password2    =  "xxxx";

//-----------configuration de la broche de pilotage de l'alim des capteurs----------------

const int     iCmdPowerI2cSensors = 33;  // broche pour activer une regulateur pilotable 3V3 exclusivement pour les périphériques 

//-----------Variables-----------------
long          ValCalibLoadCPT1  =  0;   
long          ValCalibLoadCPT2  =  0;
long          ValCalibLoadCPT3  =  0;
long          ValCalibLoadCPT4  =  0;
     
String        sLectureSerie     =  "NAS";  // buffeur de lecture sur le com
String        sSSID_Select      =  "NAS";  // SSID trouvé par l'ESP32
const float   fAjutement        =  0.565;  // Coéfficient empirique d'ajustment du poids
unsigned long previousMillis    =  0;
unsigned long interval          =  30000;
uint8_t       iNbReconnections  =  0;
bool          bMenu             =  true;
int           iTrame            =  0;
int           iNbPaquet         =  1;
char          sOutputJSon[150];

char*         sTopic            =  "Ruche/Virtuel";
int           iDebug            =  1; // 0: Debug désactivé  ; 1: Debug activé
int           iCptVirtuel       =  1; // 0: Capteurs réelles ; 1: Simulation des capteurs
long          lDelayTrameMin    =  1; // Délais entre deux trames




// 
//*******//
// SETUP //
//*******//

void setup() 
{
  bMenu = true ; 
  Serial.begin(115200);
  delay (500);
  Wire.begin(); // Wire communication begin
  if (!iCptVirtuel)
  {
    setupI2C();
    setupHX711   ();
    setupINA226  ();
    setupBME280  ();
    LoadCalibrationAllHX711 ();
    setupDS18B20 ();
  }
  setupPageWeb ();                    // a faire en dernier dans le setup
  setupMQTT_Serveur();
  setupGotoToSleepAndWakeUpByTimer;
  bMenu = false ; // pour limiter les retours d'information inutile et economiser un un poil de jus
  Menu();
}

//**************************//
// Boucle principale : LOOP //
//**************************//
void loop() 
{
    WifiReconnect();  // permet de tester la connection periodiquement et de reconnecter si perte de réseau.
    SerialRead();
    GotoClient_MQTT();
    vTaskDelay(60000);   // libere un peut de temp pour que le core 0 puisse effectuer des fonctions sans faire de timeout et reset de l'ensemble
    
}
