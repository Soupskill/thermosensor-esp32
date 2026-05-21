#pragma once

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

// Бит-маски для событий Wi-Fi
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

// Глобальные переменные (объявлены как extern)
extern EventGroupHandle_t s_wifi_event_group;
extern int s_retry_num;

// Прототип функции инициализации Wi-Fi
void wifi_init_sta(void);
