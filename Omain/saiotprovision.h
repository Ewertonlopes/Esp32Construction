#ifndef SAIOTPROVISION_H
#define SAIOTPROVISION_H

#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>

#include <wifi_provisioning/manager.h>
#include <wifi_provisioning/scheme_softap.h>

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PROV_TRANSPORT_SOFTAP   "softap"

extern const int WIFI_CONNECTED_EVENT;

void saiot_provision_start(void);

#ifdef __cplusplus
}
#endif

#endif