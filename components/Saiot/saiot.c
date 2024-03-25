#include "saiot.h"

static const char *TAG_SAIOT = "MODULE_SAIOT";

Device Saiot_Device = NULL;
bool isidentified = false;
bool islogged = false;

/*
 * @brief
 * @param 
 */

esp_err_t saiot_init(const char      *Email        ,
                     const char      *Senha        ,
                     const char      *Name         ,
                     const char      *Classe       ,
                     const char      *Description  ) {

    spiffs_start();
    spiffs_check();

    if(spiffs_data_exists("/spiffs/config.txt"))
    {
        saiot_device_get();
        ESP_LOGI(TAG_SAIOT,"Device Created Sucesfully");
        isidentified = true;
    }
    else{
        char returnUUID[37];
        UUIDGen(returnUUID);
        Saiot_Device = device_init(returnUUID,Email,Name,Classe,Description,Senha);
    }
    
    wifi_init();

    while(!isconnectedwifi) vTaskDelay(150);
    
    mqtt_init(Saiot_Device->Email,Saiot_Device->Password,Saiot_Device->Id);

    while(!isconnectedMQTT) vTaskDelay(150);

    saiot_subscribe_basic();

    spiffs_end();

    return  ESP_OK;
}

static esp_err_t saiot_device_get() {

    if(Saiot_Device != NULL) device_end(Saiot_Device);

    ESP_LOGI(TAG_SAIOT,"Device Exists in Memory!!! Recovering from memory!!!");
    
    char *config_spiffs = spiffs_data_get("/spiffs/config.txt"); 
    char *password = "senha123";//spiffs_data_get("/spiffs/password.txt");

    ESP_LOGD(TAG_SAIOT,"%s", config_spiffs);

    cJSON *Jsonob = json_parse_object(config_spiffs);
      
    char *prev_id = (cJSON_GetObjectItemCaseSensitive(Jsonob, "id"))->valuestring;
    char *prev_email = (cJSON_GetObjectItemCaseSensitive(Jsonob, "email"))->valuestring;
    char *prev_name = (cJSON_GetObjectItemCaseSensitive(Jsonob, "name"))->valuestring;
    char *prev_class = (cJSON_GetObjectItemCaseSensitive(Jsonob, "class"))->valuestring;
    char *prev_desc = (cJSON_GetObjectItemCaseSensitive(Jsonob, "description"))->valuestring;

    char *id = (char *)malloc(strlen(prev_id) + 1);
    char *email = (char *)malloc(strlen(prev_email) + 1);
    char *name = (char *)malloc(strlen(prev_name) + 1);
    char *class = (char *)malloc(strlen(prev_class) + 1);
    char *desc = (char *)malloc(strlen(prev_desc) + 1);

    if (id == NULL || email == NULL || name == NULL || class == NULL || desc == NULL) {
        ESP_LOGE(TAG_SAIOT,"Failed to allocate memory for device!!!");
    }
    
    strcpy(id, prev_id);
    strcpy(email, prev_email);
    strcpy(name, prev_name);
    strcpy(class, prev_class);
    strcpy(desc, prev_desc);

    Saiot_Device = device_init(id,email,name,class,desc,password);

    cJSON *sensors = (cJSON_GetObjectItemCaseSensitive(Jsonob, "sensors"));

    if( sensors ) {
        cJSON *sen = sensors->child;
        while( sen ) {
            char *sensor_id_prev = sen->string;
            char *sensor_name_prev = (cJSON_GetObjectItemCaseSensitive(sen, "name"))->valuestring;
            char *sensor_type_prev = (cJSON_GetObjectItemCaseSensitive(sen, "type"))->valuestring;
            int sensor_timeout = (cJSON_GetObjectItemCaseSensitive(sen, "timeout"))->valueint;
            double sensor_deadband = (cJSON_GetObjectItemCaseSensitive(sen, "deadband"))->valuedouble;
            
            char *sensor_id = (char *)malloc(strlen(sensor_id_prev) + 1);
            char *sensor_name = (char *)malloc(strlen(sensor_name_prev) + 1);
            char *sensor_type = (char *)malloc(strlen(sensor_type_prev) + 1);

            if (sensor_id == NULL || sensor_name == NULL || sensor_type == NULL) {
                ESP_LOGE(TAG_SAIOT,"Failed to allocate memory for sensor!!!");
            }
            
            strcpy(sensor_id, sensor_id_prev);
            strcpy(sensor_name, sensor_name_prev);
            strcpy(sensor_type, sensor_type_prev);

            Sensor sensor_new = sensor_init(sensor_id,sensor_name,sensor_type,sensor_timeout,sensor_deadband,sensor_number);
            device_add_sensor(Saiot_Device,sensor_new);
            
            sen = sen->next;
        }
    }

    cJSON *actuators = (cJSON_GetObjectItemCaseSensitive(Jsonob, "actuators"));

    if( actuators ) {
        cJSON *act = actuators->child;
        while( act ) {
            char *act_id_prev = act->string;
            char *act_name_prev = (cJSON_GetObjectItemCaseSensitive(act, "name"))->valuestring;
            char *act_type_prev = (cJSON_GetObjectItemCaseSensitive(act, "type"))->valuestring;

            char *act_id = (char *)malloc(strlen(act_id_prev) + 1);
            char *act_name = (char *)malloc(strlen(act_name_prev) + 1);
            char *act_type = (char *)malloc(strlen(act_type_prev) + 1);

            if (act_id == NULL || act_name == NULL || act_type == NULL) {
                ESP_LOGE(TAG_SAIOT,"Failed to allocate memory for actuator!!!");
            }
            
            strcpy(act_id, act_id_prev);
            strcpy(act_name, act_name_prev);
            strcpy(act_type, act_type_prev);

            Actuator act_new = actuator_init(act_id,act_name,act_type,act_switch);
            device_add_actuator(Saiot_Device,act_new);
            
            act = act->next;
        }
    }

    json_delete_object(Jsonob);
    return  ESP_OK;
}

static esp_err_t saiot_device_save() {
    
    spiffs_start();
    spiffs_check();
    
    if(spiffs_data_exists("/spiffs/config.txt")) spiffs_data_delete("/spiffs/config.txt");

    char *config = json_create_config(Saiot_Device);
    
    spiffs_data_save("/spiffs/config.txt",config);
    ESP_LOGI(TAG_SAIOT, "Saved Configs");

    spiffs_data_save("/spiffs/senha.txt",Saiot_Device->Password);
    ESP_LOGI(TAG_SAIOT, "Saved Passwords");

    json_free_buffer(config);
    
    return ESP_OK;
}

static esp_err_t saiot_subscribe_basic() {
    
    if(!isconnectedMQTT || !isconnectedwifi) return ESP_FAIL;

    char message_topic[50];
    char config_topic[50];
    char act_topic[50];

    strcpy(message_topic,Saiot_Device->Id);
    strcat(message_topic,"/message");

    strcpy(config_topic,Saiot_Device->Id);
    strcat(config_topic,"/config");

    strcpy(act_topic,Saiot_Device->Id);
    strcat(act_topic,"/act");

    if(xClientMQTT != NULL)
    {
        xSemaphoreTake( xMutexMQTT, portMAX_DELAY );

        int msg_id = esp_mqtt_client_subscribe(xClientMQTT, message_topic, 0);
        ESP_LOGI(TAG_SAIOT, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(xClientMQTT, config_topic, 0);
        ESP_LOGI(TAG_SAIOT, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(xClientMQTT, act_topic, 0);
        ESP_LOGI(TAG_SAIOT, "sent subscribe successful, msg_id=%d", msg_id);
        
        xSemaphoreGive( xMutexMQTT );
    }
    return ESP_OK;
}

