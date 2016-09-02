#include <Arduino.h>

#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
extern "C" {
#include "user_interface.h"
}

#define USE_SERIAL Serial

//ESP8266WiFiMulti WiFi;

HTTPClient http;
const char *ssid = "ESPap";
const char *password = "thereisnospoon";

//const char *ssid = "AUT-Test";
//const char *password = "@ppl3Wir3l3ss";

boolean lastButton = LOW;
boolean currentButton = LOW;
const int SW_PIN = 12;

void setup() {

  USE_SERIAL.begin(115200);
  // USE_SERIAL.setDebugOutput(true);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  //wifi_set_phy_mode(PHY_MODE_11G);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // allow reuse (if server supports it)
  http.setReuse(true);
  WiFi.printDiag(Serial);

  //pinMode(LED, OUTPUT);

  pinMode(SW_PIN, INPUT);
  //digitalWrite(SW_PIN, HIGH);
    pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

}

void sendSignal(String route) {
    digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level

  http.begin("192.168.4.1", 80, route);

  int httpCode = http.GET();
  if (httpCode > 0) {
    USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      //http.writeToStream(&USE_SERIAL);
    }
  } else {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

}

void loop() {
  // wait for WiFi connection
  // if((WiFiMulti.run() == WL_CONNECTED)) {
  // }


  if (WiFi.status() != WL_CONNECTED) {

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print("C");
    }

  } else {

    currentButton = debounce(lastButton);
    USE_SERIAL.printf("currentButton %d. lastb %d\n",currentButton,lastButton);
    
    if(lastButton == LOW && currentButton == HIGH)
    {
      USE_SERIAL.printf("sending button\n");
      sendSignal("/");
    }

    lastButton = currentButton;
  }
  //delay(1000);
}

