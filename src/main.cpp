#include <Arduino.h>
// #include <Measurements.h>
#include <TelemetrySender.h>
#include <WiFiSetup.h>

#include <common.h>



QueueHandle_t dataQueue;

void setup() {
  bool res;
  Serial.begin(9600);

  // initPump();
  // initMeasurements();
  initWiFiSetup();
  // Serial.println("===================================");
  // Serial.println("Starting configuration process");
  // Serial.println("===================================");
  // String ssid = userInput("SSID: ");
  // String password = userInput("Password: ");
  // Serial.println("WIFI params ready: " + ssid + "/" + password);
  // xTaskCreate(
  //   measurementsTask, // func for the task
  //   "Measurements", // description
  //   2048, // stack size
  //   dataQueue, //task parameters
  //   1, // task priority
  //   NULL // task handle
  // );

  // xTaskCreate(
  //   pumpTask, // func for the task
  //   "Pump", // description
  //   1000, // stack size
  //   NULL, //task parameters
  //   1, // task priority
  //   NULL // task handle
  // );

  xTaskCreate(TelemetryTask, "TelemetryTask", 10240, NULL, 1, NULL);
  xTaskCreate(wifiSetupTask, "wifiSetupTask", 4096, NULL, 1, NULL);
}

void loop() {
}

// vTaskDelay(pdMS_TO_TICKS(100)); // Delay to prevent task hogging CPU

