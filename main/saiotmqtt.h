#ifndef SAIOTMQTT_H
#define SAIOTMQTT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"

#define BROKER_ADDR "mqtt://192.168.0.5:1883"

static void log_error_if_nonzero(const char *message, int error_code);

static void saiot_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);

static esp_mqtt_client_handle_t saiot_mqtt_app_start(void);


#ifdef __cplusplus
}
#endif

#endif