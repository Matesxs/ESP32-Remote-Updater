#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <HTTPUpdate.h>

#define FIRMWARE_VERSION "1.0.0"

#define SSID "Qocnobyt"
#define PASSWORD "antiputin"
const char ENDPOINT_ADDRESS[] = "https://192.168.100.117:3000/update/test_firmware_name";

const char server_certificate[] = "-----BEGIN CERTIFICATE-----\n" \
"MIIFFTCCAv2gAwIBAgIUDRJQTiedy8WcVGi53ZNanqiBBqEwDQYJKoZIhvcNAQEL\n" \
"BQAwGjEYMBYGA1UEAwwPMTkyLjE2OC4xMDAuMTE3MB4XDTIzMDcxNDExNTY1OVoX\n" \
"DTI0MDcxMzExNTY1OVowGjEYMBYGA1UEAwwPMTkyLjE2OC4xMDAuMTE3MIICIjAN\n" \
"BgkqhkiG9w0BAQEFAAOCAg8AMIICCgKCAgEAxrVZjxs2uJ7QQchJwfzJxkA3wgvH\n" \
"WlrGB4ogyhfOh9dlbyTL+evFqWP0Mp91XjmQdBFRbXOJKTNOlqEtYPNuD2Ka7Oub\n" \
"nTl8BCkAoqCvNPkatk2aP/A2tk/uuOUYksTg5/2D9DoEuSmvaBV0N/DdcIb6Bf1i\n" \
"WeM62I6Vzpth8d5JmV6u2rZbTVZYvj27gLoMpuheiCl1VaI5hJNfy3tj9TnqBZes\n" \
"xFHo+rNjCdJfdn1DQ2pXzEocQdKN2Lb9c2Ebt/CYdJig5hYWofz8h+vWM+4x7uYc\n" \
"mPUXNdu1FOFGEAqf0DTBlWc+okuDWi6o3hatJ7vZsEa5QdagfBWmxw1tCkydMlnK\n" \
"PqBqGdfCiLR9Wqaa316Ize/KOgH2Ks/ZzM9yt9t5sI53+sG9O7LadRTSaN7EyI+1\n" \
"/j+9/9NyTG/3isOjoR3xFJgk1q5fEg4IaWxVxS6Hakxjx4Oi48+1JsGCLsKYx3/9\n" \
"ifl3PvcwPoSUD6OiUBTX4sPDOzzHmUnxuY3sjwUo+2eVxsIE9GL+oesM0V3Qsl8h\n" \
"C9ySr0GPcu7UqgbTddkDPBGEncY57oXZ/EKIs4mRIvC8P+KmwQ/N37jgTfntvb7x\n" \
"kSrlNk7r+6arczxGMBnfzeZdCiZYYmERoj8CUoTP3a+We0Ho/Ji3eLqkb5T3YnyP\n" \
"a6O6JS+92bKZ4+8CAwEAAaNTMFEwHQYDVR0OBBYEFLjB7vZ08JsOKiXb7yT7I3mA\n" \
"jnvgMB8GA1UdIwQYMBaAFLjB7vZ08JsOKiXb7yT7I3mAjnvgMA8GA1UdEwEB/wQF\n" \
"MAMBAf8wDQYJKoZIhvcNAQELBQADggIBAHKUH1yyWX27ie5wwc7gIllnixNkiu22\n" \
"LEwxaMsHFqoCug+lAa4ItTzti8zfXlH6w/NxEdJl1jh0+QnJTAWoiINUMaJsyHy7\n" \
"l4Wbzr9aVafS2Z8iBpMrI1SCXGp8JrK5/iwILFudc7t7trRuwsOC7P5KpgqQFWww\n" \
"ZkgnMUmMxitunO2Chg3sxkIJso6/mptbx/sFHNRrKK4tS8HdQ5ZIU0VBSUMohD3p\n" \
"lt8c1c4MgMJVpRfpQ6CeEfhqSuzOl4st4fbfLhUItdMLrr0qlXe4Ikksp9FoOAH6\n" \
"MJK5tr9wJmoXHXmV0ut/whQa8YZ7FLAPfaYY/OV1nbJTnXs5CqvA8ZDTYhi/fuW6\n" \
"NsSM7MWlOLmVox/8G/vk5AOTQopENFum8avcPpvyTEDMszIi+M6Qg4NkvonlQ3oh\n" \
"rKRFow1kVbCiPV2dtJY5y9t+Asd0TIdjZ7kW0v17oc0FAAf+Sw5cof0XWQc4+Ejz\n" \
"HCFDIAW25nGRGKwCmQ1GAKX3ZgajCjY4kBIiuNPKOWhxC+KmLbwApz4A/45PTJyJ\n" \
"96ZHLvjOQ6OAJMx1rRSWpwSy8PsPITns5CZ4vAy+gEtx10e82o70thQkmmlr2JDX\n" \
"vqZHUuwKCG9n2CI6LTbe2fLhfh4urfyJadsVZEA9Fk9gVvALK6hv947jNMFKEITl\n" \
"dKdvSlITOcwT\n" \
"-----END CERTIFICATE-----";

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

    WiFiClientSecure client;
    client.setCACert(server_certificate);
    client.setTimeout(5);
    
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

  delay(30000);
}
