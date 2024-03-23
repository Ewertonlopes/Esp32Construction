#include "saiotstruct.h"


static const char *TAG = "MAIN";

int dia = 0;

void app_main(void)
{
    Sensor Teste = sensor_init("SupaSens","Sensor de Teste","number",500,sensor_number);
}