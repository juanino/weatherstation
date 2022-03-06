#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "XXXX"
#define WLAN_PASS       "XXXXXXXXXXX"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "XXXXXXX"
#define AIO_KEY         "aio_XXXXXXXXXXXXXXXXXXXXXXXXXXXX"

#define MQTT_CONN_KEEPALIVE 300
