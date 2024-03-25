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

extern Device Saiot_Device;

esp_err_t saiot_init(const char      *Email        ,
                     const char      *Senha        ,
                     const char      *Name         ,
                     const char      *Classe       ,
                     const char      *Description  ){};

#ifdef __cplusplus
}
#endif

#endif