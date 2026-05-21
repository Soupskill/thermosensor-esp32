#ifndef MQTT_CONNECTION_h
#define MQTT_CONNECTION_h

#include <stdio.h>
#include "esp_stdio.h"
#include "esp_log.h"
#include "freertos/idf_additions.h"
#include "mqtt_client.h"
#include "sensor.h"
#include <stdio.h>


#define TAGMQTT "MQTT"
#define CONFIG_BROKER_URL "mqtt://10.182.6.201:1883"
#define TOPIC_TEMP "api/test/"


typedef struct {
    char topic[128];
    char payload[256];
    int qos;
    int retain;
} mqtt_msg_t;

extern QueueHandle_t mqtt_msg_queue;

extern esp_mqtt_event_handle_t event;
extern esp_mqtt_client_handle_t client;
void mqtt_app_start(void);

#endif 
