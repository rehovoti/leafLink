#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#define T_SURFACE_PIN 13 // GPIO pin connected to the DS18B20
#define T_WATER_IN_PIN 12 // GPIO pin connected to the DS18B20
#define T_WATER_OUT_PIN 14 // GPIO pin connected to the DS18B20

void initMeasurements();
void measurementsTask(void* parameters);

#endif