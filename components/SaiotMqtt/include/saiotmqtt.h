#ifndef SAIOTMQTT_H
#define SAIOTMQTT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "esp_check.h"
#include "esp_log.h"
#include "mqtt_client.h"

#define BROKER_ADDR "mqtt://dev.saiot2.ect.ufrn.br:8000"

/*
@brief Definição da inicialização do sistema e o client mqtt global.
*/

extern esp_mqtt_client_handle_t xClientMQTT;
extern SemaphoreHandle_t xMutexMQTT;
esp_err_t saiot_mqtt_app_start(const char *email, const char *password, const char* id);

#ifdef __cplusplus
}
#endif

#endif