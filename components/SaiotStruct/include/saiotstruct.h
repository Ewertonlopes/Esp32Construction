#ifndef SAIOTSTRUCT_h
#define SAIOTSTRUCT_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#include "esp_check.h"
#include "esp_log.h"

#define MAX_ADDONS_SIZE 20

enum dispType {
    sensor,
    actuator
};

enum sensorType {
    sensor_number
};

enum ActuatorType {
    act_switch
};

typedef struct {
    const char *Id;
    const char *Name;
    const char *type;
    enum sensorType internal_type;

    int timeout;
    float deadband;

    void *data;
} Sen, *Sensor;

typedef struct {
    const char *Id;
    const char *Name;
    const char *type;

    bool changeflag;

    enum ActuatorType internal_type;
} Act, *Actuator;

typedef struct {
    enum dispType disp;

    union {
        Sensor sensor;
        Actuator actuator;
    };
    
} Add, *Addon;

typedef struct {
    const char *Id;

    const char *Name;
    const char *Classe;
    const char *Description;

    const char *Login;
    const char *Password;

    int dispnumb;

    Addon Adds[MAX_ADDONS_SIZE];
} Dev, *Device;

Sensor sensor_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *type         ,
                    int             timeout       ,
                    float           deadband      ,   
                    enum sensorType internal_type);

void sensor_change_data(Sensor sens,void *data);
esp_err_t sensor_end(Sensor sens);

Actuator actuator_init( const char      *Id           ,
                        const char      *Name         ,
                        const char      *type         ,   
                        enum ActuatorType internal_type);

void actuator_change_data(Actuator act,void *data);
esp_err_t actuator_end(Actuator act);

Device device_init( const char      *Id           ,
                    const char      *Name         ,
                    const char      *Classe       ,
                    const char      *Description  ,
                    const char      *Login        ,
                    const char      *Password     );

void device_add_sensor(Device devi,Sensor sens);
void device_add_actuator(Device devi,Actuator act);

#ifdef __cplusplus
}
#endif


#endif
