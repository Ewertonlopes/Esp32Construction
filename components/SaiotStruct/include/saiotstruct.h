#ifndef SAIOTSTRUCT_h
#define SAIOTSTRUCT_h

#ifdef __cplusplus
extern "C" {
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include <stdlib.h>
#include <stdio.h>

// typedef void* (*sensorCallback)(void);
// typedef void  (*ActuatorCallback)(void*);


enum dispType {
    sensor,
    actuator
};

enum sensorType {
    sensor_number,
    sensor_matrix
};

// enum ActuatorType {
//     act_number,
//     act_matrix
// };

typedef struct {
    const char *fId;
    const char *Id;
    const char *Name;
    const char *type;
    enum sensorType internal_type;
    enum dispType disp;

    int timeout;

    void *data;
} Sen, *Sensor;

// typedef struct {
//     const char *Id;
//     const char *Name;
//     const char *type;

//     enum ActuatorType internal_type;
//     enum dispType disp;

// } Act, *Actuator;

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

Sensor sensor_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *type         ,
                    int             timeout       , 
                    enum sensorType internal_type);

Device device_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *Classe       ,
                    const char      *Description  ,
                    const char      *Login        ,
                    const char      *Password     );
void device_add_sensor(Device devi,Sensor sens);

#ifdef __cplusplus
}
#endif


#endif
