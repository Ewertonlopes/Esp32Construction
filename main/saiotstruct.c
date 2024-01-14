
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "SStruct.h"

static const char *TAG = "MAIN";

float dia = 0;
float dia2 = 0;
float dia3 = 0;

void *callback()
{
    float *teste;

    teste = (float*)malloc(sizeof(float));

    *teste = dia;

    dia = dia + 1.0f;

    // if(dia>30)
    // {
    //     dia = 0.0f;
    // }

    return teste;     
}

void *callback2()
{
    float *teste;

    teste = (float*)malloc(sizeof(float));

    *teste = dia2;

    dia2 = dia2 + 1.0f;

    // if(dia2>30)
    // {
    //     dia2 = 0.0f;
    // }

    return teste;     
}

void *callback3()
{
    float *teste;

    teste = (float*)malloc(sizeof(float));

    *teste = dia3;

    dia3 = dia3 + 1.0f;

    // if(dia3>30)
    // {
    //     dia3 = 0.0f;
    // }

    return teste;     
}



void app_main(void)
{
    int period = 1;
    int period2 = 1;
    int period3 = 1;

    Sensor Teste = sensor_init("SupaSens","Sensor de Teste","Teste",period,number,callback);
    Sensor Teste2 = sensor_init("Supa2","Sensor de Teste","Teste",period2,number,callback2);
    Sensor Teste3 = sensor_init("Supa2","Sensor de Teste","Teste",period3,number,callback3);

    // sensor_run(Teste);
    Device B_Teste = device_init("ID0001","Main Device","Teste","Um device de testes","teste@teste.com","0123456789");

    printf("Id:%s\nNome:%s\nClasse:%s\nDesc:%s\nLogin:%s\nSenha:%s\nN Disps:%u\n\n",B_Teste->Id,
                                                                                    B_Teste->Name,
                                                                                    B_Teste->Classe,
                                                                                    B_Teste->Description,
                                                                                    B_Teste->Login,
                                                                                    B_Teste->Password,
                                                                                    B_Teste->dispnumb);


    device_add_sensor(B_Teste,Teste);
    device_add_sensor(B_Teste,Teste2);
    //device_add_sensor(B_Teste,Teste3);

    printf("Id:%s\nNome:%s\nClasse:%s\nDesc:%s\nLogin:%s\nSenha:%s\nN Disps:%u\n\n",B_Teste->Id,
                                                                                    B_Teste->Name,
                                                                                    B_Teste->Classe,
                                                                                    B_Teste->Description,
                                                                                    B_Teste->Login,
                                                                                    B_Teste->Password,
                                                                                    B_Teste->dispnumb);

    TickType_t xLastWakeTime;

    device_run(B_Teste);
    
    const TickType_t xFrequency = pdMS_TO_TICKS(1000);
    xLastWakeTime = xTaskGetTickCount();

    while (1) 
    {
        printf("Frequency 1 Expected:%.0f\nFrequency 1 obtain:%.0f\n",(1.0f/(((float)period/1000.0f))),dia);
        printf("Frequency 2 Expected:%.0f\nFrequency 2 obtain:%.0f\n",(1.0f/(((float)period2/1000.0f))),dia2);
        //printf("Frequency 3 Expected:%.0f\nFrequency 3 obtain:%.0f\n\n",(1.0f/(((float)period3/1000.0f))),dia3);

        // period += 10;
        // Teste->timeout=period;
        // if(period > 1000)
        // {
        //     period = 1;
        // }

        dia = 0.0f;
        dia2 = 0.0f;
        dia3 = 0.0f;

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}