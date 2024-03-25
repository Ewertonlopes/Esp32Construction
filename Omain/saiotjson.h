#ifndef SAIOTJSON_H
#define SAIOTJSON_H

//INCLUDES

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "cJSON.h"

/*************************************************************
 * Modulo Responsável pelo desenvolvimento dos Jsons 
 * enviados pela biblioteca.
**************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

char* json_create_config(char *ID,char *email,char *name,char *class,char *description,char* sensorid,cJSON *Sensor);
cJSON* json_create_sensor(char *name,char *type,int value,int timeout,int deadband);
char* json_create_payload(char *ID,int value);
cJSON* json_parse_object(char* Json);
bool json_delete_object(cJSON* Json);
bool json_free_buffer(char* Json);


#ifdef __cplusplus
}
#endif

#endif