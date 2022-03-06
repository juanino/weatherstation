/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board & Feather
  ----> https://www.adafruit.com/product/2471
  ----> https://www.adafruit.com/products/2821

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "config.h"

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiClientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed for the screen
Adafruit_MQTT_Subscribe thefeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/red1");

// testing
// char* text = "test";
// char* text2 = "xxxxxx";
// char* bogus = strcat(text2, text);
// Adafruit_MQTT_Subscribe thefeed2 = Adafruit_MQTT_Subscribe(&mqtt, bogus);


/*************************** Sketch Code ************************************/

// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() {
  boot();
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  scroll_msg(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  

  // Setup MQTT subscription for screen feed.
  mqtt.subscribe(&thefeed);
  
}


void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
 
  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &thefeed) {
      Serial.print(F("Got: "));
      Serial.println((char *)thefeed.lastread);
      // send to display
      char* msg = ((char *)thefeed.lastread);
      Serial.println("size early is:");
      size_t msglen = strlen(msg);
      Serial.println(msglen);
      scroll_msg(msg);
    }
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds

  if (! mqtt.ping()) {
    mqtt.disconnect();
  }

}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  scroll_msg("MQTT");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("GOOD");
  scroll_msg("RDY ");
}

void scroll_msg(char* msg) {
  Serial.println("scrolling message: ");
  Serial.println(msg);
  Serial.println("length of msg is:");
  size_t msglen = strlen(msg);
  Serial.println(msglen);
  for (int i = 0; i < (msglen-3); i++) {
    alpha4.writeDigitAscii(0, msg[i]);
    alpha4.writeDigitAscii(1, msg[i + 1]);
    alpha4.writeDigitAscii(2, msg[i + 2]);
    alpha4.writeDigitAscii(3, msg[i + 3]);
    alpha4.writeDisplay();
    delay(300);
  }
}

void boot() {
  alpha4.begin(0x70);  // pass in the address
  alpha4.writeDigitRaw(3, 0x0);
  alpha4.writeDigitRaw(0, 0xFFFF);
  alpha4.writeDisplay();
  delay(200);
  alpha4.writeDigitRaw(0, 0x0);
  alpha4.writeDigitRaw(1, 0xFFFF);
  alpha4.writeDisplay();
  delay(200);
  alpha4.writeDigitRaw(1, 0x0);
  alpha4.writeDigitRaw(2, 0xFFFF);
  alpha4.writeDisplay();
  delay(200);
  alpha4.writeDigitRaw(2, 0x0);
  alpha4.writeDigitRaw(3, 0xFFFF);
  alpha4.writeDisplay();
  delay(100);

  alpha4.clear();
  alpha4.writeDisplay();
  alpha4.writeDigitAscii(0, 'L');
  alpha4.writeDigitAscii(1, 'O');
  alpha4.writeDigitAscii(2, 'A');
  alpha4.writeDigitAscii(3, 'D');
  alpha4.writeDisplay();
  delay(500);
}
