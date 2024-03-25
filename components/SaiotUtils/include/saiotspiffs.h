#ifndef SAIOTSPIFFS_H
#define SAIOTSPIFFS_H

//INCLUDES

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

/*************************************************************
 * Modulo Responsável por Cuidar do Salvamento e Leitura
 * de Dados na flash do Dispositivo. 
**************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define SPIFFS_BUFFER_SIZE 512

bool spiffs_start();
bool spiffs_end();
void spiffs_check();
char* spiffs_data_get(char* file);
bool spiffs_data_save(char* file, char* data);
bool spiffs_data_exists(char* file);
void spiffs_data_delete(char* file);

#ifdef __cplusplus
}
#endif

#endif