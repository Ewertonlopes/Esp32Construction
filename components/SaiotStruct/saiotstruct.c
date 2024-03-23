#include "saiotstruct.h"

static const char *TAG_STRUCT = "Struct Module";

/*
 * @brief Inicialização de um Sensor
 *
 * @param Id id do dispositivo, necessario para comunicação interna.
 * @param Name Nome, necessario para exibição na plataforma.
 * @param type Tipo do dispositivo que será exibido na plataforma.
 * @param timeout Tempo em que o dispositivo gerará novos dados.
 * @param internal_type tipo de dispositivo reconhecido internamente.
 *                      -Number,
 *                      -Matrix,
 */

Sensor sensor_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *type         ,
                    int             timeout       ,
                    float           deadband      , 
                    enum sensorType internal_type){

    Sensor base = (Sensor)malloc(sizeof(Sen));
    
    if(base == NULL) ESP_LOGE(TAG_STRUCT, "FAILED TO INIT SENSOR!!!");

    base->Id = Id;
    base->Name = Name;
    base->type = type;
    base->timeout = timeout;
    base->deadband = deadband;

    switch(internal_type)
    {
        case sensor_number:
            base->data = malloc(sizeof(float));
            *(float*)base->data = 0.0f;
            break;
        default:
            ESP_LOGE(TAG_STRUCT, "SENSOR TYPE INVALID!!!");
            break;
    }
    return base;
}

/*
 * @brief Função interna para mudar valor de um sensor
 *
 * @param sens O sensor a se mudar o valor.
 * @param data O valor a se mudar no sensor
 */

void sensor_change_data(Sensor sens,void *data){

    switch(sens->internal_type)
    {
        case sensor_number:
            *(float*)sens->data = *(float*)data;
            break;
        default:
            break;
    }
}

/*
 * @brief Função para se excluir um sensor
 *
 * @param sens O sensor.
 */

esp_err_t sensor_end(Sensor sens){

    free(sens->data);
    free(sens);
    ESP_LOGI(TAG_STRUCT, "Sensor Erased");

    return ESP_OK;
}

/*
 * @brief Inicialização de um Atuador
 *
 * @param Id id do dispositivo, necessario para comunicação interna.
 * @param Name Nome, necessario para exibição na plataforma.
 * @param type Tipo do dispositivo que será exibido na plataforma.
 * @param internal_type tipo de dispositivo reconhecido internamente.
 *                      -switch,
 */

Actuator actuator_init( const char      *Id           ,
                        const char      *Name         ,
                        const char      *type         , 
                        enum ActuatorType internal_type){

    Actuator base = (Actuator)malloc(sizeof(Act));
    
    if(base == NULL) ESP_LOGE(TAG_STRUCT, "FAILED TO INIT ACTUATOR!!!");

    base->Id = Id;
    base->Name = Name;
    base->type = type;

    base->changeflag = false;

    switch(internal_type)
    {
        case act_switch:
            base->data = malloc(sizeof(bool));
            *(bool*)base->data = 0;
            break;
        default:
            ESP_LOGE(TAG_STRUCT, "INVALID ACTUATOR TYPE!!!");
            break;
    }
    return base;
}

/*
 * @brief Função interna para mudar valor de um Atuador
 *
 * @param act O Atuador a se mudar o valor.
 * @param data O valor a se mudar no Actuator;
 */

void actuator_change_data(Actuator act,void *data){

    switch(act->internal_type)
    {
        case act_switch:
            *(bool*)act->data = *(bool*)data;
            break;
        default:
            break;
    }
}

/*
 * @brief Função para se excluir um atuador
 *
 * @param act O atuador.
 */

esp_err_t actuator_end(Actuator act){

    free(act->data);
    free(act);
    ESP_LOGI(TAG_STRUCT, "Actuator Erased");

    return ESP_OK;
}

/*
 * @brief Função de inicialização principal do sistema
 *
 * @param ID id do dispositivo,
 * @param Name nome que sera utilizado na plataforma,
 * @param Classe classe utilizada pela plataforma,
 * @param Description Uma pequena descrição utilizada pela plataforma,
 * @param Login disponível na plataforma,
 * @param Password senha utilizada na plataforma.
 */
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
    for(int i=0;i<MAX_ADDONS_SIZE;i++) base->Adds[i] = NULL;

    return base;
}

/*
 * @brief Funções de Baixo nível para se adicionar um sensor e atuador
 *
 * @param devi o device adicionado,
 * @param act/sens O addon a ser adicionado.
 */

void device_add_sensor(Device devi,Sensor sens){
    Addon base = (Addon)malloc(sizeof(Add));
    base->sensor = sens;
    base->disp = sensor;
    devi->Adds[devi->dispnumb] = base;
    devi->dispnumb++;
}

/*
 * @brief Função para se excluir um atuador
 *
 * @param act O atuador.
 */

void device_add_actuator(Device devi,Actuator act){
    Addon base = (Addon)malloc(sizeof(Add));
    base->actuator = act;
    base->disp = actuator;
    devi->Adds[devi->dispnumb] = base;
    devi->dispnumb++;
}