idf_component_register(SRCS "saiotspiffs.c" "saiotjson.c" "uuidgen.c"
                       INCLUDE_DIRS "include"
                       REQUIRES spiffs
                                json
                                SaiotStruct)