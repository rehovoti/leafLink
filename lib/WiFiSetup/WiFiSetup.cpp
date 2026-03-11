#include <Arduino.h>
#include <WiFiManager.h>   //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <WiFiSetup.h>

void initWiFiSetup() {
  WiFi.mode(WIFI_STA);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

}

void wifiSetupTask(void* parameters) {
  while(1) {
    if ( digitalRead(TRIGGER_PIN) == LOW) {
      WiFiManager wm;    
      // WiFiManager on 192.168.4.1
  
      //reset settings - for testing
      //wm.resetSettings();
    
      // set configportal timeout
      wm.setConfigPortalTimeout(TIMEOUT);
  
      if (!wm.startConfigPortal("Kesha")) {
        Serial.println("failed to connect and hit TIMEOUT");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        //reset and try again, or maybe put it to deep sleep
        ESP.restart();
        vTaskDelay(5000 / portTICK_PERIOD_MS);
      }
  
      //if you get here you have connected to the WiFi
      Serial.println("connected...yeey :)");
  
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS);
  }
}