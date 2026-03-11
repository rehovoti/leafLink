#include <Arduino.h>
#include "Pump.h"

void initPump() {
    pinMode(pump_enA, OUTPUT);
    pinMode(pump_in1, OUTPUT);
    pinMode(pump_in2, OUTPUT);
    // Pump Direction
    digitalWrite(pump_in1, HIGH);
    digitalWrite(pump_in2, LOW);
}

void pumpTask(void* parameters) {
  while(1){
    for(int pwd_val=0;pwd_val < 255; pwd_val += 5) {
      // Serial.print("Pump Task PWD value: ");
      // Serial.println(pwd_val);
      analogWrite(pump_enA,pwd_val);
      vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
  }
}