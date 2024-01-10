#ifndef SSTRUCT_h
#define SSTRUCT_h

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdlib.h>
#include <stdio.h>

typedef void* (*sensorCallback)(void);
typedef void  (*ActuatorCallback)(void*);

/******************************************
****************   Enums  *****************
******************************************/

enum dispType {
    sensor,
    actuator
};

enum sensorType {
    number,
    matrix
};

enum ActuatorType {
    number,
    matrix
};


/******************************************
**************** Structs  *****************
******************************************/

typedef struct {
    const char *Id;
    const char *Name;
    const char *type;
    enum sensorType internal_type;
    enum dispType disp;
    
    int timeout;

    void *data;

    sensorCallback callback;
} Sen, *Sensor;

typedef struct {
    const char *Id;
    const char *Name;
    const char *type;

    enum ActuatorType internal_type;
    enum dispType disp;

    ActuatorCallback callback;
} Act, *Actuator;

typedef struct {
    const char *Id;

    const char *Name;
    const char *Classe;
    const char *Description;

    const char *Login;
    const char *Password;

    int dispnumb;
    void *(*Disps);
} Dev, *Device;

void sensor_print(Sensor sens){
    printf("Id:%s\nNome:%s\nType:%s\nData:%.2f\n\n",sens->Id,
                                                    sens->Name,
                                                    sens->type,
                                                    *(float*)sens->data);
}

/******************************************
*****************  Task  ******************
******************************************/

void base_sensor_task(void * pvParams){
  TickType_t xLastWakeTime;
  
  Sensor Sens = (Sensor) pvParams;
//   TickType_t xFrequency = pdMS_TO_TICKS(Sens->timeout);
  xLastWakeTime = xTaskGetTickCount();

  while (1) 
  {
    free(Sens->data);
    Sens->data = Sens->callback();
    //sensor_print(Sens);
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(Sens->timeout));
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
        case number:
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
        case number:
            sens->data = data;
            break;
        default:
            break;
    }
}

void sensor_run(Sensor sens){

    xTaskCreate(&base_sensor_task, sens->Id, 4096, sens, 2, NULL);
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


#endif
