#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPUpdate.h>

#define FIRMWARE_VERSION "1.0.0"

#define SSID ""
#define PASSWORD ""
#define ENDPOINT_ADDRESS "http://192.168.100.117:3000/update/test_firmware_name"

WiFiMulti wifiMulti;

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(4000);

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(SSID, PASSWORD);

  httpUpdate.rebootOnUpdate(false);

  Serial.println("Initialization successful");
  Serial.printf("Firmware version: %s\r\n", FIRMWARE_VERSION);
}

void loop()
{
  if (wifiMulti.run() == WL_CONNECTED)
  {
    Serial.println("Wifi connected");
    Serial.println("Checking for updates");

    WiFiClient client;
    t_httpUpdate_return ret = httpUpdate.update(client, ENDPOINT_ADDRESS, FIRMWARE_VERSION);

    switch (ret)
    {
      case HTTP_UPDATE_FAILED:
        Serial.printf("Update failed - Error (%d): %s\n", httpUpdate.getLastError(), httpUpdate.getLastErrorString().c_str());
        break;

      case HTTP_UPDATE_NO_UPDATES:
        Serial.println("No updates available");
        break;

      case HTTP_UPDATE_OK:
        Serial.println("Update successful, restarting...");
        delay(1000);
        ESP.restart();
        break;

      default:
        break;
    }
  }
  else
  {
    Serial.println("Wifi not connected");
  }

  delay(10000);
}
