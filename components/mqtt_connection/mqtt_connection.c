#include "cJSON.h"
#include "freertos/idf_additions.h"
#include "mqtt_connection.h"
#include "freertos/projdefs.h"
#include "mqtt_client.h"
#include "sensor.h"
#include "math.h"

QueueHandle_t mqtt_msg_queue = NULL;
esp_mqtt_event_handle_t event = NULL;
esp_mqtt_client_handle_t client = NULL;

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAGMQTT, "Last error %s: 0x%x", message, error_code);
    }
}

static void mqtt_publisher_task(void *pvParameters)
{
    Sensor_t sensor_data;
    cJSON *root = NULL;
    char *json_str = NULL;

    while (1) {
        // Ждём данные из очереди сенсора (блокируемся, пока не появится элемент)
        if (xQueueReceive(sensor_queue, &sensor_data, portMAX_DELAY) == pdTRUE) {
            // Формируем JSON
            root = cJSON_CreateObject();
            if (root != NULL) {
                cJSON_AddNumberToObject(root, "temperature", round(sensor_data.temperature * 100) / 100);
                cJSON_AddNumberToObject(root, "ticks", sensor_data.ticks);
                cJSON_AddNumberToObject(root, "humidity", round(sensor_data.humidity * 100) / 100);
                json_str = cJSON_PrintUnformatted(root);
                cJSON_Delete(root);
            }

            if (json_str != NULL && client != NULL) {
                int msg_id = esp_mqtt_client_publish(client, "api/test/", json_str, 0, 1, 0);
                if (msg_id == -1) {
                    ESP_LOGE(TAGMQTT, "Failed to publish sensor data");
                } else {
                    ESP_LOGI(TAGMQTT, "Published: %s", json_str);
                }
                free(json_str); // или cJSON_free, если использовали cJSON_Print
            } else if (client == NULL) {
                ESP_LOGW(TAGMQTT, "MQTT client not ready yet");
            }
        }
    }
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGI(TAGMQTT, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;


    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAGMQTT, "MQTT_EVENT_CONNECTED");
            
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAGMQTT, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAGMQTT, "MQTT_EVENT_SUBSCRIBED, msg_id=%d, return code=0x%02x ", event->msg_id, (uint8_t)*event->data);
            msg_id = esp_mqtt_client_publish(client, "topic/qos0", "data", 0, 0, 0);
            ESP_LOGI(TAGMQTT, "sent publish successful, msg_id=%d", msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAGMQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAGMQTT, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAGMQTT, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAGMQTT, "MQTT_EVENT_ERROR");

            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAGMQTT, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }

            break;

        default:
            ESP_LOGI(TAGMQTT, "Other event id:%d", event->event_id);
            break;
        }
}

void mqtt_app_start(void)
{
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = CONFIG_BROKER_URL,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
    xTaskCreate(mqtt_publisher_task, "mqtt_pub", 4096, NULL, 5, NULL);
}
