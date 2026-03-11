#ifndef COMMON_H
#define COMMON_H

typedef struct {
    float tempIn;
    float tempOut;
    float tempTank;
    float am2302_temp;
    float am2302_humidity;
    float voltage;
    unsigned char pumpStatus;
} SensorData;

#define QUEUE_LENGTH 10
#define ITEM_SIZE sizeof(SensorData)

#endif