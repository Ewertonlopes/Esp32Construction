#include "saiotwifi.h"

static const char *TAG_WIFI = "SUBMODULE_WIFI";

static TaskHandle_t xHandleWIFI;

bool isconnectedwifi = false;
const char *wifi_ssid = "brisa-1835136";
const char *wifi_pass = "osaxzp72";

wifi_config_t wifi_configuration = {
    .sta = {
        .ssid = "",
        .password = "",
        }
};

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id,void *event_data){

    switch(event_id){
        case WIFI_EVENT_STA_START:
            ESP_LOGI(TAG_WIFI,"Starting Conection...");
            break;
        case WIFI_EVENT_STA_CONNECTED:
            isconnectedwifi = true;
            vTaskSuspend( xHandleWIFI );
            ESP_LOGI(TAG_WIFI,"Device Conected!!!");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            isconnectedwifi = false;
            vTaskResume( xHandleWIFI );
            ESP_LOGI(TAG_WIFI,"Device Lost Connection!!!");
            break;
        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI(TAG_WIFI,"Connection Got IP!!!");
            break;
        default:
        break;
    }
}

void wifi_reconnect_task(void * pvParams){
    
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t wifi_initiation = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_initiation);

    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handler, NULL);
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_event_handler, NULL);

    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_configuration);
    esp_wifi_start();
    esp_wifi_set_mode(WIFI_MODE_STA);

    strcpy((char*)wifi_configuration.sta.ssid, wifi_ssid);
    strcpy((char*)wifi_configuration.sta.password, wifi_pass);  

    while(true){
        ESP_LOGI(TAG_WIFI,"Trying to reconnect...");
        if(!isconnectedwifi) esp_wifi_connect();
        vTaskDelay(5000);
    }
}

void wifi_init() {
    xTaskCreate(&wifi_reconnect_task, "wifi_reconnecter", 2048, NULL, 13, &xHandleWIFI);
}