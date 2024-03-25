#ifndef SAIOTJSON_H
#define SAIOTJSON_H

//INCLUDES

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"
#include "saiotstruct.h"

/*************************************************************
 * Modulo Responsável pelo desenvolvimento dos Jsons 
 * enviados pela biblioteca.
**************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

char* json_create_config(Device main_dev);
cJSON* json_create_sensor(Sensor main_sensor);
cJSON* json_create_actuator(Actuator main_actuator);
char* json_create_payload_float(char *ID,float value);
cJSON* json_parse_object(char* Json);
bool json_delete_object(cJSON* Json);
bool json_free_buffer(char* Json);


#ifdef __cplusplus
}
#endif

#endif