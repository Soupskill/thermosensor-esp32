#include "sensor.h"
#include "esp_random.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "math.h"

QueueHandle_t sensor_queue = NULL;
TaskHandle_t sensor_task_handle = NULL;

void sensor_task(void*){

    Sensor_t sensor;

    for (;;){

        vTaskDelay( pdMS_TO_TICKS(INTERVAL * 1000));
        sensor.temperature = (float)esp_random() / (float)UINT32_MAX;
        sensor.ticks = xTaskGetTickCount();
        xQueueSend(sensor_queue, &sensor, portMAX_DELAY);
    }

}

void sensor_start(){
    sensor_queue = xQueueCreate(SENSOR_QUEUE_LEN, sizeof(Sensor_t));
    xTaskCreate(&sensor_task, "sensor_task", 4096, NULL, 4, &sensor_task_handle);
}
