#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>

// const char* ssid = "Kasper";
// const char* password = "505903471";
const char* url = "https://script.google.com/macros/s/AKfycbyiQR_APn3cssUSozT4v3cKFFYgDrS986e-hDbXtyMnfawHdUiRY1zuq0oU05WpPc4aSw/exec?tin=12&tout=23&ttank=34&voc=0.45&At=56&Ah=67&lux=78&pump=1";
// https://script.googleusercontent.com/macros/echo?user_content_key=AehSKLgYADu4vAqr3pIiKAoYSLCcKnQ9CxRhGHkBQWsEtPKMLF3fTOVMOPLaya4HsV08P_Hnpc-wNMnX1xgmH12zvRs5wic4j7FTEOD85rQQOYXT34V9Okwzlcwtw_pysUPohgXWRyL0CnaDVH4R4UFipTo4Je1Pu-8uws9oHIFgfkz48KW_ET0C7ykXWVFxJlV9r-gtmmXqhq9KvsNjDU1XXIVvdfvyAyFHJdepNUmx6zOUCA6XKOKSnrCxVHt3uk_onwT1Ehe3IWDsvEOq7_w7mV5Yq9TCO6u8fFAuBZ8FXctG4Ww6fE7t-6Zi1gJxuY-Bq60Kisr-Qs91EbDpjr5DTJn5XmDAesfpFf7OBUii3hEwBG_VoJQ&lib=MU0_TlFkHOzupdqRbCVzZCcvjUuzmcpRp
void TelemetryTask(void* pvParameters) {
  WiFiManager wm;
  HTTPClient http;
  while(1) {
    if(WiFi.status() != WL_CONNECTED) {
    String ssid = wm.getWiFiSSID();
    String password = wm.getWiFiPass();
    WiFi.begin(ssid.c_str(), password.c_str());

      while (WiFi.status() != WL_CONNECTED) {
          vTaskDelay(5000 / portTICK_PERIOD_MS);
          Serial.print(".");
      }
      Serial.println("\nConnected to Wi-Fi!");
    }
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
        Serial.printf("HTTP Response code: %d\n", httpCode);
        String payload = http.getString();
        Serial.println("Response: " + payload);
    } else {
        Serial.printf("Error code: %d\n", httpCode);
    }

    http.end();
    vTaskDelay(30000 / portTICK_PERIOD_MS);  // Delay before repeating (optional)
  }
    // vTaskDelete(NULL);  // Delete the task when done
}

// void setup() {
//     xTaskCreate(httpsGetTask, "HTTPS GET Task", 8192, NULL, 1, NULL);
// }

// void loop() {
//     // Nothing to do here for this example
// }
