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

static esp_err_t saiot_device_get();
static esp_err_t saiot_device_save();
static esp_err_t saiot_subscribe_basic();


#ifdef __cplusplus
}
#endif

#endif