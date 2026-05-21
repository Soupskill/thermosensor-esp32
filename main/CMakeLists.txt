idf_component_register(SRCS "thermosensor-esp32.c"
                        INCLUDE_DIRS "."
                        PRIV_REQUIRES sensor mqtt_connection network modbus)

