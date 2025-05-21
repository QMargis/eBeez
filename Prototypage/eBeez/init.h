// ********************************* //
// Valeurs de paramètrage par défaut //
// ********************************* //

// Wifi Cllient
String  ssid_cl     = "";
String  pass_cl     = "";

// Wifi AP
String  ssid_a      = "WiBeez";
String  pass_a      = "WiBeez";

// Mode du WiFi par défaut
String  wifi_m      = "DOUBLE"; // CLIENT | AP | DOUBLE

// Login web
String  webLog      = "admin";
String  webPa       = "password";

// Nom de la ruche
String  rucheN      = "Ruche"; // Nom de la ruche

// Mise en veille 
bool modeV          = false;  // Activation de la mise en veuille ou non : true | false
int  timeToS        = 600;    // Temps d'attente avant mise en veille en second
int  LoopN          = 20;     // Nombre de boucle avant la mise en veuille

//Parametres balance
float ajust         = 0;  // Valeur du facteur d'ajustement pour les capteurs de poids
long  valCal1       = 0;
long  valCal2       = 0;
long  valCal3       = 0;
long  valCal4       = 0;

//Paramters MQTT
String  mqttServeur = "";  // Adresse du serveur MQTT
int     mqttPort    = 1883;             // Port de communication du serveur MQTT
String  mqttUser    = "";           // Identifiant MQTT
String  mqttPass    = "";    // Password MQTT

// ********************************* //
// Broches de connexion des capteurs //
// ********************************* //

//----HX711 circuit wiring---------------
const int LOADCELL_1_DOUT_PIN = 16;   //Capteur de poids n°1      GPIO16
const int LOADCELL_2_DOUT_PIN = 17;   //Capteur de poids n°2      GPIO17
const int LOADCELL_3_DOUT_PIN =  5;   //Capteur de poids n°3      GPIO05
const int LOADCELL_4_DOUT_PIN = 18;   //Capteur de poids n°4      GPIO18
const int LOADCELL_SCK_PIN    =  4;   //Capteur de poids Horloge  GPIO04

// GPIO where the DS18B20 is connected to
const int oneWireBus = 32;   //  GPIO32 

//-----------configuration de la broche de pilotage de l'alim des capteurs----------------
const int iCmdPowerI2cSensors = 14;   // broche pour activer une regulateur pilotable 3V3 exclusivement pour les périphériques        GPIO14 

//----------- Broche de mesure de la tension d'alim
const int iGetVoltPin = 34;  // GPIO34 

//----------- Coefficients de tension d'alim
const float fCoeffAlim  = 3.4233;
const float fOrdoAlim   = 0.6556;
