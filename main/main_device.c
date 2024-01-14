
#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>

#include "saiotprovision.h"
#include "saiotstruct.h"

#define PROV_TRANSPORT_SOFTAP   "softap"

static const char *TAG = "MAIN";

float dia = 0;

void *callback()
{
    float *teste;

    teste = (float*)malloc(sizeof(float));

    *teste = dia;

    dia = dia + 1.0f;

    if(dia>30)
    {
        dia = 0.0f;
    }

    return teste;     
}

void app_main(void)
{
    //Create Every Struct
    Sensor Teste = sensor_init("SupaSens","Sensor de Teste","Teste",300,number,callback);

    //Provision WIFI
    saiot_provision_start();

    //TODO PROVISION CREDENTIALS TOO

    //Connect to MQTT

    //Run Every Struct

    while (1) {
         ESP_LOGI(TAG, "Hello World!");
         vTaskDelay(1000 / portTICK_PERIOD_MS);
     }
}