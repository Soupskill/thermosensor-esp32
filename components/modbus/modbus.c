// components/modbus_slave/modbus_slave.c
#include "modbus.h"
#include "esp_err.h"
#include "esp_modbus_common.h"
#include "hal/uart_types.h"
#include "lwip/ip_addr.h"
#include "mb_port_types.h"
#include "mb_types.h"
#include "esp_log.h"
#include "esp_modbus_slave.h"
#include "driver/uart.h"

static const char *TAG = "MODBUS_SLAVE";

void init_modbus(){
    
    mb_tcp_opts_t tcp_opts = {
        .mode = MB_TCP,
        .addr_type = MB_IPV4,
        .port = MB_PORT_TCP_SLAVE,
        .dns_name = "",
        .uid = 2,
        .ip_addr_table = "10.182.6.201",
    };
     
    mb_communication_info_t conn = {
        .mode = MB_TCP,
    };
    conn.tcp_opts = tcp_opts;

    ESP_ERROR_CHECK(mbc_slave_create_tcp(&conn, NULL));
    ESP_LOGI(TAG, "Modbus initialized");

}
