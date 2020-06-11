#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

#include "settings.h"

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup()
{
  // Initialize pins
  pinMode(LED_BUILTIN, OUTPUT); // Inverted on Wemos D1 Mini
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_RX_ONLY, 3, true);

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    blinkLed(20, 50); // Blink fast to indicate no WiFi connection
    delay(500);
  }

  setupOTA();
  setupDataReadout();

  mqttClient.setServer(MQTT_HOST, atoi(MQTT_PORT));
  blinkLed(5, 500); // Blink 5 times to indicate end of setup
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED) {
    blinkLed(20, 50); // Blink fast to indicate failed WiFi connection
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }

  ArduinoOTA.handle();
  long now = millis();

  if (!mqttClient.connected())
  {
    long now = millis();

    if (now - LAST_RECONNECT_ATTEMPT > 5000)
    {
      LAST_RECONNECT_ATTEMPT = now;

      if (mqttReconnect())
      {
        LAST_RECONNECT_ATTEMPT = 0;
      }
    }
  }
  else
  {
    mqttClient.loop();
  }

  if (now - LAST_UPDATE_SENT > UPDATE_INTERVAL)
  {
    readP1Serial();
  }
}
