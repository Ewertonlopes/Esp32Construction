#ifndef SAIOTSTRUCT_h
#define SAIOTSTRUCT_h

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdlib.h>
#include <stdio.h>
#include "freertos/semphr.h"

typedef void* (*sensorCallback)(void);
typedef void  (*ActuatorCallback)(void*);


enum dispType {
    sensor,
    actuator
};

enum sensorType {
    sensor_number,
    sensor_matrix
};

enum ActuatorType {
    act_number,
    act_matrix
};

typedef struct {
    const char *fId;
    const char *Id;
    const char *Name;
    const char *type;
    enum sensorType internal_type;
    enum dispType disp;

    // esp_mqtt_client_handle_t mqttclient;
    
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

    // esp_mqtt_client_handle_t mqttclient;
    void *(*Disps);
} Dev, *Device;

void sensor_print(Sensor sens);
void base_sensor_task(void * pvParams);
Sensor sensor_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *type         ,
                    int             timeout       , 
                    enum sensorType internal_type);
void sensor_change_data(Sensor sens,void *data);
void sensor_run(Sensor sens);
void sensor_end(Sensor sens);

Device device_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *Classe       ,
                    const char      *Description  ,
                    const char      *Login        ,
                    const char      *Password     );
void device_add_sensor(Device devi,Sensor sens);
void device_run(Device devi);

#ifdef __cplusplus
}
#endif


#endif
