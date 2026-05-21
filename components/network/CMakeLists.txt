idf_component_register(SRCS "network.c"
                       INCLUDE_DIRS "include"
                        REQUIRES nvs_flash esp_wifi esp_event esp_netif freertos)
