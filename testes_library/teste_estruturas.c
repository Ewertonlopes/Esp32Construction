#include "saiotstruct.h"

void app_main(void) {
    Sensor addon1 = sensor_init("addon1","simples","teste",200,0.2,sensor_number);
    float *value = (float*)malloc(sizeof(float));
    *value = 30.2f;
    sensor_change_data(addon1,value);
    *value = 11.3f;
    sensor_change_data(addon1,value);
    *value = 3.2f;
    sensor_change_data(addon1,value);

    Actuator addon2 = actuator_init("addon2","basico","teste",act_switch);
    bool *state = (bool*)malloc(sizeof(bool));
    *state = false;
    actuator_change_data(addon2,state);
    *state = true;
    actuator_change_data(addon2,state);
    *state = false;
    actuator_change_data(addon2,state);

    Device main_device = device_init("IdSimples","Principal","teste","Um dispositivo de teste","login","senha");
    device_add_sensor(main_device,addon1);
    device_add_actuator(main_device,addon2);
    
    free(state);
    free(value);
    device_end(main_device);
}