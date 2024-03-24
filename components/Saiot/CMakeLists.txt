idf_component_register(SRCS "saiot.c"
                       INCLUDE_DIRS "include"
                       REQUIRES SaiotMqtt
                                SaiotStruct
                                SaiotUtils
                                SaiotWifi)