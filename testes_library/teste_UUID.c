#include "uuidgen.h"

static const char *TAG_MAIN = "MAIN_MODULE";

void app_main(void) {
    char returnUUID[37];
    UUIDGen(returnUUID);
    ESP_LOGI(TAG_MAIN,"%s",returnUUID);
}