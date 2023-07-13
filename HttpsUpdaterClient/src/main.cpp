#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <esp_https_ota.h>
#include <esp_http_client.h>

#define FIRMWARE_VERSION "1.0.0"

#define SSID ""
#define PASSWORD ""
const char ENDPOINT_ADDRESS[] = "https://192.168.100.117:3000/update/test_firmware_name?cver=" \
                                 FIRMWARE_VERSION;

const char server_certificate[] = "-----BEGIN CERTIFICATE-----\n" \
"MIIE+zCCAuOgAwIBAgIUDpVPXNwLFsmcETUDoHAJYjHws1cwDQYJKoZIhvcNAQEL\n" \
"BQAwDTELMAkGA1UEBhMCQ1owHhcNMjMwNzEzMjIyMTMzWhcNMjQwNzEyMjIyMTMz\n" \
"WjANMQswCQYDVQQGEwJDWjCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIB\n" \
"AKmHGipylPsf6livsJnYZDW0Mh9J+dYdNNtcM8VMQzqkplUTp6LfjwtmXW0LKzEn\n" \
"jHoSka1B3goi74VJATcjhVVGkdJ1rVLYk5ye2JRsEDAtwfiI+0RQMiZxJgmcr/JJ\n" \
"le0pwkDRuaBhtD84LN633mqiwVZ/O4dA12WXa9jcKsnGDCmpmwX/ii/FOi8IBSuJ\n" \
"p13I23pKhHT9+LtHdn8fMsH8Z3mB68H8y5cxWEuMsfgsag5koJWFHRP0oyEdfCLN\n" \
"A99AcEoR8goYToMHnRkCYhPcJi7pn+en0PUIQDJERgHs9J3pPxFyjzAxhACrY0Vv\n" \
"WrH3oGwEi8W8B2PF6i4vUoQpzMbXkU33IK2B7f8jUszv6UXztls2825KKxT+FpZq\n" \
"7LVQE4RjdydmuNVqqLicE7+D1mG1KVY0BTXdM7+W4WP/xT2/BiFX3xFWzDPJM9O6\n" \
"WD3ZGUV14zJwnq/g3FP4g4u5pynvdHjILYuZ6k2Lc3xL5xTG6lyfX3sARN1w6TSB\n" \
"SX1WAmTjZPhgjUc6g2lZF31smb+AWMvXzgcjwJ9yT1zT04TWbIyaB/KWbmqX4tC/\n" \
"iVzPav9nglyzbGp3DcoBJV3Gmsi6vPFcT/JpgpsYeMupQdqJdh4N0f2EF9kAhGMa\n" \
"ct52vV9usEuUhZVEIDOTJuSTEEmKcg3g2rW6AdAvFKmZAgMBAAGjUzBRMB0GA1Ud\n" \
"DgQWBBTImmKxgKxxvEVpxoqO7yK4bU+a0TAfBgNVHSMEGDAWgBTImmKxgKxxvEVp\n" \
"xoqO7yK4bU+a0TAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEBCwUAA4ICAQAu\n" \
"IWZw9GDskWz7n5un6Em+9GTYTjiYSY7aZcVk71xrdzy0hTlFRYhQlFZDqoKn40nB\n" \
"lt/GOdAzEphnBmHLPvq2wMVH+Swnomo4zJjdp9XL4nSHd45e513dkPvpHQOdR7uW\n" \
"XmlaUBllRDfW9Yq3u08WPWJPCq9EyESizXiNy2TNe7dFW5Gyj9FiMFOpIBC9R1CI\n" \
"IRo1dwNrjsfD4T2c5uZKcsm2ASgLdMxwsyK3WTkdzSpCE1crWXbQsMdFkOpWO2Cp\n" \
"NWXQO96FGKiM1Zlv/I/9K+jcY0uYIe+utdSfA6slSrfMGz2RWWQNd4vnpJna1PMx\n" \
"Wdog/ZYPFFB/EQW9xIaJGUUTIIUnaY0WQ84e7RYXrV5WgbiyqOjL3AaBlRp7CFCs\n" \
"rprwDIe+GH8IdSTGBw94JvARv/c6rL+ecw9dgIJ7qBYLGdDgqXNeG/PxxUOwYp8r\n" \
"9lUua406wnBWcp3gLMJU5nRehIALlYpdPZZVcAcDMmGT6JOjG18+uU4bnBARbaXk\n" \
"8llJOGs8XF7AS3Jrodn4jB35QxnrSs4ti9rIIyjD/UDMQznkLwJJMxZZIf40Uxdp\n" \
"Vozyvhzm7E0IBS6WUOONw38D/WB/Hy+vnJvVia7LQWhRWpOW3f4tZXL7geBCHdDJ\n" \
"63YAHoY2L7ap8fBophJQDLGJtKdZqZ6655j0PNcwfQ==\n" \
"-----END CERTIFICATE-----";

WiFiMulti wifiMulti;
const esp_http_client_config_t ota_config = { .url=ENDPOINT_ADDRESS, .cert_pem=server_certificate, .skip_cert_common_name_check=true };

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  delay(4000);

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(SSID, PASSWORD);

  Serial.println("Initialization successful");
  Serial.printf("Firmware version: %s\r\n", FIRMWARE_VERSION);
}

void loop()
{
  if (wifiMulti.run() == WL_CONNECTED)
  {
    Serial.println("Wifi connected");
    Serial.println("Checking for updates");

    esp_err_t ota_ret = esp_https_ota((const esp_http_client_config_t *)(&ota_config));
    switch (ota_ret)
    {
    case ESP_OK:
      Serial.println("Update successful, restarting...");
      delay(1000);
      ESP.restart();
      break;

    case ESP_FAIL:
      Serial.println("No update available");
      break;

    case ESP_ERR_NO_MEM:
      Serial.println("Not enough memory for firmware");
      break;

    case ESP_ERR_FLASH_OP_TIMEOUT:
    case ESP_ERR_FLASH_OP_FAIL:
      Serial.println("Failed to write firmware");
      break;
    
    default:
      break;
    }
  }
  else
  {
    Serial.println("Wifi not connected");
  }

  delay(20000);
}
