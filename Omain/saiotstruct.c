
#include <saiotstruct.h>

static const char *sTAG = "Struct Module";

SemaphoreHandle_t xMutex = NULL;

void sensor_print(Sensor sens){
    printf("Id:%s\nNome:%s\nType:%s\nData:%d\n\n",sens->Id,
                                                    sens->Name,
                                                    sens->type,
                                                    *(int*)sens->data);
}

/******************************************
*****************  Task  ******************
******************************************/

void base_sensor_task(void * pvParams){
  TickType_t xLastWakeTime;
  
  Sensor Sens = (Sensor) pvParams;
//   TickType_t xFrequency = pdMS_TO_TICKS(Sens->timeout);
  xLastWakeTime = xTaskGetTickCount();
  char *payload = json_create_payload(Sens->Id,0);
  
//   char topic[50];
//   sprintf(topic,"%s%s","",Sens->fId);
  int teste = 0;
  xMutex = xSemaphoreCreateMutex();
  while (1){
    if(xMutex != NULL)
    {
            xSemaphoreTake( xMutex, portMAX_DELAY );
    {
        int msg_id = esp_mqtt_client_publish(Sens->mqttclient, "measurements/273e255d-125b-47ca-979c-66b29263fd35", payload, 0, 0, 0);
    }
    xSemaphoreGive( xMutex );
        if(teste++>30)
        {
            teste = 0;
        }
        // printf("%u\n",teste);
        // sensor_print(Sens);
        json_free_buffer(payload);
        payload = json_create_payload(Sens->Id,teste);
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Sens->timeout));
    }
  }
}

/******************************************
***************** Sensor ******************
******************************************/

Sensor sensor_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *type         ,
                    int             timeout       , 
                    enum sensorType internal_type ,
                    sensorCallback  callback){

    Sensor base = (Sensor)malloc(sizeof(Sen));

    base->Id = Id;
    base->Name = Name;
    base->type = type;
    base->timeout = timeout;
    base->callback = callback;

    switch(internal_type)
    {
        case sensor_number:
            base->data = malloc(sizeof(float));
            *(float*)base->data = 0.0f;
            break;
        default:
            break;
    }
    return base;
}

void sensor_change_data(Sensor sens,void *data){

    switch(sens->internal_type)
    {
        case sensor_number:
            sens->data = data;
            break;
        default:
            break;
    }
}

void sensor_add_mqtt_client(Sensor sens, esp_mqtt_client_handle_t mqttclient)
{
    sens->mqttclient = mqttclient;
}

void sensor_run(Sensor sens){
    xTaskCreate(&base_sensor_task, sens->Id, 4096, sens, 1, NULL);
}

void sensor_end(Sensor sens){

    free(sens->data);
    free(sens);
}

/******************************************
***************** Device ******************
******************************************/

Device device_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *Classe       ,
                    const char      *Description  ,
                    const char      *Login        ,
                    const char      *Password     ){
    
    Device base = (Device)malloc(sizeof(Dev));

    base->Id = Id;
    base->Name = Name;
    base->Classe = Classe;
    base->Description = Description;
    base->Login = Login;
    base->Password = Password;
    base->dispnumb = 0;
    base->Disps = malloc(1 * sizeof(void *));

    return base;
}

void device_add_sensor(Device devi,Sensor sens){

    sensor_add_mqtt_client(sens,devi->mqttclient);
    sens->fId = devi->Id;
    devi->dispnumb += 1;
    void **ndisps = malloc(devi->dispnumb * sizeof(void *));

    for(int i =0; i<(devi->dispnumb - 1);i++)
    {
        ndisps[i] = devi->Disps[i];
    }

    free(devi->Disps);

    ndisps[devi->dispnumb - 1] = sens;
    devi->Disps = ndisps;
}

void device_run(Device devi){
    for(int i =0;i<devi->dispnumb;i++)
    {
        sensor_run((Sensor)(devi->Disps[i]));
    }
}