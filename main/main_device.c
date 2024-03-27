#include "saiot.h"

static const char *TAG_MAIN = "MODULE_MAIN";

const char      *Email       = "teste@teste.com";
const char      *Senha       = "senha123";

const char      *Name        = "Disp Testes";
const char      *Classe      = "Testes";
const char      *Description = "Um dispositivo para fazer testes"; 

void app_main(void) {
    saiot_init(Email,Senha,Name,Classe,Description);
    double *value = saiot_add_sensor_number("sens1","Best","teste",1000,0.2);
    sensor_run(Saiot_Device->Adds[0]->sensor);
    while(1) {
        (*value)++;
        if(*value>300) {
            *value = 0;
        }
        vTaskDelay(1000);
    }
    //saiot_mqtt_callback("096a2b15-1fd5-4d8a-ae49-0296b95b1524/config","Nada");
}