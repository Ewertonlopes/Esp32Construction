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

#define ESP_MAXIMUM_RETRY 5
#define ESP_WIFI_SSID "brisa-1835136"
#define ESP_WIFI_PASS "osaxzp72"

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

extern bool isconnectedwifi;
extern const char *wifi_ssid;
extern const char *wifi_pass;

void wifi_init();

#ifdef __cplusplus
}
#endif


#endif
