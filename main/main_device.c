
#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>

#include "saiotprovision.h"

#define PROV_TRANSPORT_SOFTAP   "softap"

static const char *TAG = "MAIN";

void app_main(void)
{
    saiot_provision_start();

    while (1) {
         ESP_LOGI(TAG, "Hello World!");
         vTaskDelay(1000 / portTICK_PERIOD_MS);
     }
}