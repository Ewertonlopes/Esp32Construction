
#include <stdio.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>

#include "saiotstruct.h"
#include "saiotprovision.h"
#include "saiotclock.h"
#include "saiotmqtt.h"

#define PROV_TRANSPORT_SOFTAP   "softap"

static const char *TAG = "MAIN";

int dia = 0;

void *callback()
{
    int *teste;

    teste = (int*)malloc(sizeof(int));

    *teste = dia;

    dia = dia + 1;

    if(dia>30)
    {
        dia = 0;
    }

    return teste;     
}

void app_main(void)
{
    /************************
        Create Every Struct
    ************************/

    Sensor Teste = sensor_init("SupaSens","Sensor de Teste","number",300,sensor_number,callback);
    Device B_Teste = device_init("273e255d-125b-47ca-979c-66b29263fd35","Main Device","Teste","Um device de testes","saiotect@gmail.com","@2345678");
    //273e255d-125b-47ca-979c-66b29263fd35
    device_add_sensor(B_Teste,Teste);

    /************************
        Provision WIFI
    ************************/

    saiot_provision_start();

    //TODO PROVISION CREDENTIALS TOO
    
    /************************
            Get Time
    ************************/

    // clock_start();
    

    /************************
        Connect to MQTT
    ************************/

    esp_mqtt_client_handle_t mclient = saiot_mqtt_app_start(B_Teste->Login,B_Teste->Password,B_Teste->Id);
    sensor_add_mqtt_client(Teste,mclient);

    /************************
        INTEGRATION HELL
    ************************/
    int msg_id = 0;
    msg_id = esp_mqtt_client_subscribe(mclient, "273e255d-125b-47ca-979c-66b29263fd35/message", 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
    msg_id = esp_mqtt_client_subscribe(mclient, "273e255d-125b-47ca-979c-66b29263fd35/config", 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
    msg_id = esp_mqtt_client_subscribe(mclient, "273e255d-125b-47ca-979c-66b29263fd35/act", 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
    // device_run(B_Teste);
    // char tempoaa[60];
    // while(1)
    // {
    //     clock_get_time(tempoaa);
        
    //     ESP_LOGI(TAG, "%s",tempoaa);
    //     vTaskDelay(500);
    // }
}