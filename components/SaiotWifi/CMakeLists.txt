idf_component_register(SRCS "saiotwifi.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_wifi
                                nvs_flash)