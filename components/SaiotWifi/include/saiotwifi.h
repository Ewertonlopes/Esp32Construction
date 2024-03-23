#ifndef SAIOTWIFI_h
#define SAIOTWIFI_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h" 
#include "esp_wifi.h"

#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

extern bool isconnectedwifi;
extern const char *wifi_ssid;
extern const char *wifi_pass;

void wifi_connection();

#ifdef __cplusplus
}
#endif


#endif
