#include "saiotwifi.h"
#include "uuidgen.h"

static const char *TAG_MAIN = "MAIN_MODULE";

void app_main(void) {
    nvs_flash_init();
    wifi_connection();
}