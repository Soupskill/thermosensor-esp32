// components/modbus_slave/include/modbus_slave.h
#pragma once

#include <stdint.h>
#include "esp_modbus_common.h"

// Определение структуры данных устройства. Вы можете менять её по своему усмотрению.
#pragma pack(push, 1)
typedef struct {
    uint16_t temperature;   // Значение температуры, целое (напр., 23.5*10 = 235)
    uint16_t humidity;      // Значение влажности, целое (напр., 654 = 65.4%)
    uint16_t relay_status;  // Статус реле (0/1)
    uint16_t system_uptime; // Время работы системы, сек
} device_data_t;
#pragma pack(pop)

void init_modbus(void);

