idf_component_register(SRCS "saiotmqtt.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_event
                                mqtt
                                Saiot)