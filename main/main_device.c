#include "saiot.h"

static const char *TAG_MAIN = "MODULE_MAIN";

const char      *Email       = "teste@teste.com";
const char      *Senha       = "senha123";

const char      *Name        = "Disp Testes";
const char      *Classe      = "Testes";
const char      *Description = "Um dispositivo para fazer testes"; 

void app_main(void) {
    saiot_init(Email,Senha,Name,Classe,Description);
}