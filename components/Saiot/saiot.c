#include "saiot.h"

static const char *TAG_SAIOT = "MODULE_SAIOT";

Device Saiot_Device = NULL;
bool isaproved = false;
bool islogged = false;

esp_err_t saiot_init(const char      *Email        ,
                     const char      *Senha        ,
                     const char      *Name         ,
                     const char      *Classe       ,
                     const char      *Description  ) {

    spiffs_start();
    spiffs_check();
    
    spiffs_data_delete("/spiffs/config.txt");

    if(   spiffs_data_exists("/spiffs/config.txt")
       && spiffs_data_exists("/spiffs/password.txt") 
       && spiffs_data_exists("/spiffs/links.txt")) {

        ESP_LOGI(TAG_SAIOT,"Device Exists in Memory!!! Recovering from memory!!!");

        char *config_spiffs = spiffs_data_get("/spiffs/config.txt"); 
        char *password = spiffs_data_get("/spiffs/password.txt");
        char *links = spiffs_data_get("/spiffs/links.txt");

        saiot_device_get(config_spiffs,password);
        saiot_link_get(links);
        ESP_LOGI(TAG_SAIOT,"Device Created Sucesfully");
        isaproved = true;
    }
    else{
        char returnUUID[37];
        UUIDGen(returnUUID);
        Saiot_Device = device_init(returnUUID,Email,Name,Classe,Description,Senha);
    }
    
    // wifi_init();

    // while(!isconnectedwifi) vTaskDelay(150);
    
    // mqtt_init(Saiot_Device->Email,Saiot_Device->Password,Saiot_Device->Id);

    // while(!isconnectedMQTT) vTaskDelay(150);

    // saiot_subscribe_basic();

    spiffs_end();

    return  ESP_OK;
}

double * saiot_add_sensor_number(char *Id,char *Name,char *type, int timeout, double deadband) {

    Sensor new_sensor = sensor_init(Id,Name,type,timeout,deadband,sensor_number);
    double *number_link = malloc(sizeof(double));

    device_add_sensor(Saiot_Device,new_sensor);

    new_sensor->data = number_link;
    *number_link = 0;
    ESP_LOGI(TAG_SAIOT,"Sensor %s is linked", Id);
    return number_link;
}

void sensor_run(Sensor sens){
    xTaskCreate(&base_sensor_task, sens->Id, 4096, sens, 12, NULL);
}

static esp_err_t saiot_device_get(char *config, char *password) {

    if(Saiot_Device != NULL) device_end(Saiot_Device);

    ESP_LOGD(TAG_SAIOT,"%s", config);

    cJSON *Jsonob = json_parse_object(config);
      
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

static esp_err_t saiot_link_get(char *links) {
    char *token;
    int i = 0;
    
    token = strtok(links, " ");
    while (token != NULL && i < Saiot_Device->dispnumb) {
        bool isactuator = Saiot_Device->Adds[i]->disp;
        if(isactuator) {
            Saiot_Device->Adds[i]->actuator->data = (void *)strtol(token, NULL, 16);
            ESP_LOGI(TAG_SAIOT,"Actuator %s is linked",Saiot_Device->Adds[i]->actuator->Id );
        }
        else {
            Saiot_Device->Adds[i]->sensor->data = (void *)strtol(token, NULL, 16);
            ESP_LOGI(TAG_SAIOT,"Sensor %s is linked",Saiot_Device->Adds[i]->sensor->Id );
        }

        token = strtok(NULL, " ");
        i++;
    }
    return ESP_OK;
}

static esp_err_t saiot_device_save() {
    
    spiffs_start();
    spiffs_check();
    
    if(spiffs_data_exists("/spiffs/config.txt")) spiffs_data_delete("/spiffs/config.txt");
    saiot_device_save_config();

    if(spiffs_data_exists("/spiffs/password.txt")) spiffs_data_delete("/spiffs/password.txt");
    saiot_device_save_senha();

    if(spiffs_data_exists("/spiffs/link.txt")) spiffs_data_delete("/spiffs/link.txt");
    saiot_device_save_link();

    spiffs_end();
    
    return ESP_OK;
}

static esp_err_t saiot_device_save_config() {
    char *config = json_create_config(Saiot_Device);
    
    spiffs_data_save("/spiffs/config.txt",config);
    ESP_LOGI(TAG_SAIOT, "Saved Configs");
    
    json_free_buffer(config);
    return ESP_OK;
}

static esp_err_t saiot_device_save_senha() {
    char *senha = Saiot_Device->Password;
    spiffs_data_save("/spiffs/senha.txt",senha);
    ESP_LOGI(TAG_SAIOT, "Saved Passwords");
    return ESP_OK;
}

static esp_err_t saiot_device_save_link() {
    char buffer[200];
    buffer[0] = '\0'; 
    char temp[20];
    
    for (int i = 0; i < Saiot_Device->dispnumb; i++) {
        bool isactuator = Saiot_Device->Adds[i]->disp;
        if(isactuator) {
            sprintf(temp, "%p ", Saiot_Device->Adds[i]->actuator->data);
            strncat(buffer, temp, -1);
        }
        else {
            sprintf(temp, "%p ", Saiot_Device->Adds[i]->sensor->data);
            strncat(buffer, temp, -1);
        }
    }

    ESP_LOGI(TAG_SAIOT,"Links saved: %s", buffer);
    spiffs_data_save("/spiffs/links.txt",buffer);
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

void saiot_mqtt_callback(char *topic,char *data) {
    
    size_t length = strlen(topic);

    char last_letter = topic[length-1];

    switch(last_letter) {
        case 'e':
            saiot_mqtt_topic_message(data);      
            break;
        case 'g':
            saiot_mqtt_topic_config(data);
            break;
        case 't':
            saiot_mqtt_topic_act(data);
            break;
        default:
            ESP_LOGD(TAG_SAIOT,"Topico não reconhecido... %s",topic);
           break;
    }
}

static esp_err_t saiot_mqtt_topic_message(char *data) {
/*
Ca[d]astre-se 
Ap[r]ovado
Lo[g]ado
Re[c]usado
De[l]etado
Ag[u]ardando Aprovação
*/
    char third_letter = data[2];
    
    switch(third_letter) {
        case 'd':
            isaproved = false;
            islogged = false;
            if(xClientMQTT != NULL) {
        
                xSemaphoreTake( xMutexMQTT, portMAX_DELAY );

                char *config = json_create_config(Saiot_Device);
                esp_mqtt_client_publish(xClientMQTT, "register-device", config, 0, 0, 0);    
                
                json_free_buffer(config);

                xSemaphoreGive( xMutexMQTT );
            }
            break;
        case 'r':
            isaproved = true;
            islogged = false;            
            break;
        case 'g':
            isaproved = false;
            islogged = true;
            break;
        case 'c':
            isaproved = false;
            islogged = false;
            break;
        case 'l':
            spiffs_data_delete("/spiffs/config.txt");
            spiffs_data_delete("/spiffs/senha.txt");
            isaproved = false;
            islogged = false;
            break;
        case 'u':
            isaproved = false;
            islogged = false;
            break;
        default:
            ESP_LOGD(TAG_SAIOT,"Topico não reconhecido...");
            break;
    }
    return ESP_OK;
}

static esp_err_t saiot_mqtt_topic_config(char *data) {
    spiffs_start();
    spiffs_check();

    char *password = spiffs_data_get("/spiffs/password.txt");

    saiot_device_get(data,password);
    spiffs_end();
    return ESP_OK;
}

static esp_err_t saiot_mqtt_topic_act(char *data) {
    return ESP_OK;
}

static void base_sensor_task(void * pvParams) {
    
    TickType_t xLastWakeTime;
  
    Sensor Sens = (Sensor) pvParams;
    ESP_LOGI(TAG_SAIOT,"%p",Sens->data); 
    double *valuenow = ((double*)Sens->data);
    xLastWakeTime = xTaskGetTickCount();
    
    while (1){
        ESP_LOGI(TAG_SAIOT,"%lf",*valuenow);   
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Sens->timeout));
    }
}