#ifndef SENSOR_h
#define SENSOR_h

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define SENSOR_QUEUE_LEN 10
#define INTERVAL 5

typedef struct {
    TickType_t ticks;
    float temperature;
} Sensor_t;

extern QueueHandle_t sensor_queue;
extern TaskHandle_t sensor_task_handle;


void sensor_start();
#endif


