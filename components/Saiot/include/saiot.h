#ifndef SAIOT_H
#define SAIOT_H

#ifdef __cplusplus
extern "C" {
#endif

/********************
 ****  Estrutura ****
 ********************/

#include "saiotstruct.h"

/********************
 ***** Conexão ******
 ********************/

#include "saiotwifi.h"
#include "saiotmqtt.h"

/********************
 ****** Extras ******
 ********************/

#include "saiotjson.h"
#include "saiotspiffs.h"
#include "uuidgen.h"

extern Device Saiot_Device;

esp_err_t saiot_init(const char      *Email        ,
                     const char      *Senha        ,
                     const char      *Name         ,
                     const char      *Classe       ,
                     const char      *Description  );

static esp_err_t saiot_device_get(char *config, char *password);
static esp_err_t saiot_device_save();
static esp_err_t saiot_subscribe_basic();

void saiot_mqtt_callback(char *topic,char *data);

static esp_err_t saiot_mqtt_topic_message(char *data);
static esp_err_t saiot_mqtt_topic_config(char *data);
static esp_err_t saiot_mqtt_topic_act(char *data);

#ifdef __cplusplus
}
#endif

#endif