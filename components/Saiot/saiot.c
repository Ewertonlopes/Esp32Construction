#include "saiot.h"

static const char *TAG_SAIOT = "MODULE_SAIOT";

Device Saiot_Device = NULL;

static bool is_created = false;

/*
 * @brief
 * @param 
 */
esp_err_t saiot_init(const char      *Email        ,
                     const char      *Senha        ,
                     const char      *Name         ,
                     const char      *Classe       ,
                     const char      *Description  ) {
    /*
     * Fazer recuperação Id/dados internos;
     */
    spiffs_start();
    spiffs_check();
    if(spiffs_data_exists("/spiffs/config.txt"))
    {
        char *config_spiffs = spiffs_data_get("/spiffs/config.txt"); 
        ESP_LOGI(TAG_SAIOT,"SPIFFS BEFORE INIT");
        ESP_LOGI(TAG_SAIOT,"%s", config_spiffs);
        spiffs_data_delete("/spiffs/config.txt");
    }
    else{
        const char *Id = "DummyID";

        Saiot_Device = device_init(Id,Email,Name,Classe,Description,Senha);

        ESP_LOGI(TAG_SAIOT,"Device Created Sucesfully");

        char *config = json_create_config(Saiot_Device);

        spiffs_data_save("/spiffs/config.txt", config);
        char *config_spiffs = spiffs_data_get("/spiffs/config.txt");
        ESP_LOGI(TAG_SAIOT,"%s", config); 
        ESP_LOGI(TAG_SAIOT,"%s", config_spiffs);

        json_free_buffer(config);
    }
    spiffs_end();
    return  ESP_OK;
}

