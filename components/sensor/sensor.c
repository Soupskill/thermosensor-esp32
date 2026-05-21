#include "sensor.h"
#include "esp_random.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

#include "math.h"
#include "dht.h"

QueueHandle_t sensor_queue = NULL;
TaskHandle_t sensor_task_handle = NULL;

void sensor_task(void*){

    Sensor_t sensor_data;
    float temperature;
    float humidity;
    

    for (;;){

        vTaskDelay( pdMS_TO_TICKS(INTERVAL * 1000));

        dht_read_float_data(DHT_TYPE_AM2301, GPIO_NUM_3, &humidity, &temperature);

        sensor_data.temperature = temperature;
        sensor_data.humidity = humidity;
        sensor_data.ticks = xTaskGetTickCount();
        xQueueSend(sensor_queue, &sensor_data, portMAX_DELAY);
    }

}

void sensor_start(){
    sensor_queue = xQueueCreate(SENSOR_QUEUE_LEN, sizeof(Sensor_t));
    xTaskCreate(&sensor_task, "sensor_task", 4096, NULL, 4, &sensor_task_handle);
}
