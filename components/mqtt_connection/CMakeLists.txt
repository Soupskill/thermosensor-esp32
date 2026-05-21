idf_component_register(
    SRCS "mqtt_connection.c"
    INCLUDE_DIRS "include"
    PRIV_REQUIRES espressif__cjson mqtt sensor
)
