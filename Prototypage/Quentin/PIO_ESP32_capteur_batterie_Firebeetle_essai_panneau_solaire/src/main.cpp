#include <Arduino.h>
#include <Wire.h>

// #include "ADC.h"

#include <WiFi.h>
#include <WiFiMulti.h>

#include <PubSubClient.h>

#include "driver/adc.h"

#include <esp_wifi.h>
#include <esp_bt.h>

#include <Preferences.h>

#include "secret.h"
/*
Non synchronisé Github, besoin de #define des variables suivantes :
Wifi :
WIFISSID1 ==> SSID du routeur WIFI
WIFIPASSWORD1 ==> Mot de passe du routeur WIFI
// MQTT
MQTTBROKER ==> Adresse du broker MQTT
MQTT_USERNAME ==> Login du compte MQTT
MQTT_PASSWORD ==> Mot de passe du compte MQTT
*/

// Nom de l'ESP32 sur le réseau MQTT
#define MQTT_CLIENT_NAME "ESPFIREBTLSOL"

/****************************************
 * Define Constants
 ****************************************/

// Variable pour passer de microsecondes vers secondes
#define uS_TO_S_FACTOR 1000000ULL

// Temps (secondes) à passer en veille
#define TIME_TO_SLEEP 600

// Temps (secondes) du timer Watchdog
#define WATCHDOG_TIMEOUT_S 30

hw_timer_t *watchDogTimer = NULL;

// Utilisation de Preferences pour stockage en nvm
Preferences preferences;

// MQTT
char mqttBroker[] = MQTTBROKER;
char mqtt_username[] = MQTT_USERNAME;
char mqtt_password[] = MQTT_PASSWORD;
char payload[100];
char topic[150];

// Pour calcul du temps passé sur une boucle
unsigned long uptime = 0;

// Nombre de réveils de l'ESP32
RTC_DATA_ATTR int bootCount = 0;

uint8_t LED = D9;

// Objet connexion wifi
WiFiClient wifi_client;

// Objet connexion MQTT
PubSubClient client(wifi_client);

/****************************************
 * Prototypes
 ****************************************/
// clignotement led
void blink();

// WIFI : Connection wifi avec gestion glitch et connexion second point accès si non dispo
void setup_wifi();

// MQTT : Pour traitement de message sur topic souscris par ESP32
void callback(char *topic, byte *payload, unsigned int length);

// MQTT : Essai reconnections MQTT et wifi en cas de perte connection
void reconnect();

// Sleep #DEBUG : Affiche sur sortie série le motif de réveil
void print_wakeup_reason();

// Sleep : Eteint les services et mets en veille
void go_to_sleep();

// Watchdog : Fonction appelée par le Watchdog si il atteint la fin de son timer
void IRAM_ATTR watchDogInterrupt();

// Watchdog : Reset du timer du Watchdog
void watchDogRefresh();

void log_pref(String &debug_pref, String log);

uint32_t adc_batterie = 0;

String debug_pref = "";

/****************************************
 * Main Functions
 ****************************************/

void setup()
{

  // Configuration du Watchdog
  watchDogTimer = timerBegin(2, 80, true);
  timerAttachInterrupt(watchDogTimer, &watchDogInterrupt, true);
  timerAlarmWrite(watchDogTimer, WATCHDOG_TIMEOUT_S * 1000000, false);
  timerAlarmEnable(watchDogTimer);

  // Série desactivée, pour DEBUG uniquement
  // Serial.begin(115200);

  // Utilisation de l'ADC pour lecture du niveau de batterie
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

  // Récupération du niveau de batterie, moyenne sur 5 lectures et envoi MQTT sur canal "Test2"
  for (int i = 1; i <= 5; i++)
  {
    adc_batterie += adc1_get_raw(ADC1_CHANNEL_6);
    delay(10);
  }
  adc_batterie /= (uint32_t)5;

  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);

  preferences.begin("my−app", false);
  debug_pref = preferences.getString("debug", "");

  pinMode(LED, OUTPUT);

  ++bootCount;

  log_pref(debug_pref, "\nBootcount : " + String(bootCount) + "\n");

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  log_pref(debug_pref, "Heap libre : " + String(ESP.getFreeHeap()) + " sur " + String(ESP.getHeapSize()) + "\n");

  blink();

  WiFi.mode(WIFI_STA);
  setup_wifi();

  if (!client.connected())
  {
    reconnect();
  }

  if (client.connected())
  {
    char message[30];
    sprintf(message, "%d", adc_batterie);
    client.publish("Test_solaire", message);

    client.loop();

    char char_array[debug_pref.length() + 1];
    debug_pref.toCharArray(char_array, debug_pref.length());
    client.publish("DEBUG\\Solaire", char_array);

    preferences.putString("debug", "");
  }

  blink();
  blink();

  // Mise en veille
  go_to_sleep();
}

void loop()
{
  // Ne devrait pas arriver là : Réveil / Execution / Veille ==> pas d'utilisation de la boucle
  go_to_sleep();
}

/****************************************
 * Auxiliar Functions
 ****************************************/

void blink()
{
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);
}

void setup_wifi()
{
  int glitch = 0;
  delay(10);

  WiFi.begin(WIFISSID1, WIFIPASSWORD1);

  while (WiFi.status() != WL_CONNECTED)
  {
    if (glitch >= 10)
    {
      go_to_sleep();
    }
    if (glitch == 5)
    {
      glitch++;
    }
    else
    {
      delay(1000);
      // Serial.print(".");
      glitch++;
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  String message(p);
}

void reconnect()
{
  int glitch = 0;
  // Loop until we're reconnected
  while (!client.connected())
  {
    // Serial.println("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_NAME, mqtt_username, mqtt_password))
    {
      // Serial.println("Connected");
    }
    else
    {
      if (glitch >= 10)
      {
        go_to_sleep();
      }
      else
      {
        if (WiFi.status() != WL_CONNECTED)
        {
          WiFi.disconnect(true);
          delay(100);
          setup_wifi();
        }
        else
        {
        }
        delay(2000);
        glitch++;
      }
    }
  }
}

void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason)
  {
  case ESP_SLEEP_WAKEUP_EXT0:
    // Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case ESP_SLEEP_WAKEUP_EXT1:
    // Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case ESP_SLEEP_WAKEUP_TIMER:
    // Serial.println("Wakeup caused by timer");
    break;
  case ESP_SLEEP_WAKEUP_TOUCHPAD:
    // Serial.println("Wakeup caused by touchpad");
    break;
  case ESP_SLEEP_WAKEUP_ULP:
    // Serial.println("Wakeup caused by ULP program");
    break;
  default:
    // Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason);
    break;
  }
}

void go_to_sleep()
{
  // Deconnexion MQTT
  client.disconnect();
  delay(50);
  // Deconnexion Wifi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  esp_wifi_stop();
  // Deconnexion Bluetooth
  btStop();
  esp_bt_controller_disable();
  // Serial.flush();
  esp_deep_sleep_start();
}

void IRAM_ATTR watchDogInterrupt()
{
  // Serial.println("Reboot watchdog");
  Serial.printf("WD!");
  ESP.restart();
}

void watchDogRefresh()
{
  // Mise du timer Watchdog à 0
  timerWrite(watchDogTimer, 0);
}

void log_pref(String &debug_pref, String log)
{
  // Log de debug en mémoire ESP pour transmission à la prochaine connexion MQTT
  debug_pref += log;
  preferences.putString("debug", debug_pref);
}
