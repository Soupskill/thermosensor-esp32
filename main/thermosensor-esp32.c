#include <stdio.h>
#include "esp_stdio.h"
#include "mb_types.h"
#include "soc/gpio_num.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "dht.h"
#include "mqtt_connection.h"
#include "sensor.h"
#include "network.h"
//#include "esp_modbus_common.h"
//#include "esp_modbus_slave.h"



void app_main(void)
{
    wifi_init_sta();
    sensor_start();
    mqtt_app_start();

}
