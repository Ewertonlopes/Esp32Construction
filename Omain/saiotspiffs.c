#include "saiotspiffs.h"


/*************************************************************
 * Variaveis internas do modulo, ESPIFFS tag de erros, buffer -
 * utilização interna e conf trata das configurações dos arquivos. 
**************************************************************/

static const char *ESPIFFS = "SPIFFS";
static char spiffs_buffer[SPIFFS_BUFFER_SIZE] = {};
static esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 10,
    .format_if_mount_failed = true
};

/*************************************************************
 * @def spiffs_start
 * @brief Inicia os parametros da configuração dada acima,
 * monta o sistema de arquivos.
 * @param None
 * @return  True or False 
**************************************************************/


bool spiffs_start()
{
    ESP_LOGI(ESPIFFS, "Initializing SPIFFS");

    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(ESPIFFS, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(ESPIFFS, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(ESPIFFS, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return false;
    }
    return true;
}

/*************************************************************
 * @def spiffs_end
 * @brief Desmonta e finaliza o sistema de arquivos.
 * @param None
 * @return True or False 
**************************************************************/

bool spiffs_end()
{
    ESP_LOGI(ESPIFFS, "Unmounting SPIFFS");
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(ESPIFFS, "SPIFFS unmounted");
    return true;
}

/*************************************************************
 * @def spiffs_check
 * @brief Checa a integridade do sistema de arquivos SPIFFS
 * @param None
 * @return None
**************************************************************/

void spiffs_check()
{
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    size_t total = 0, used = 0;
    ret = esp_spiffs_info(conf.partition_label, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(ESPIFFS, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
        esp_spiffs_format(conf.partition_label);
    } else {
        ESP_LOGI(ESPIFFS, "Partition size: total: %d, used: %d", total, used);
    }
    // Check consistency of reported partiton size info.
    if (used > total) {
        ESP_LOGW(ESPIFFS, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
        ret = esp_spiffs_check(conf.partition_label);
        if (ret != ESP_OK) {
            ESP_LOGE(ESPIFFS, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
        } else {
            ESP_LOGI(ESPIFFS, "SPIFFS_check() successful");
        }
    }
}

/*************************************************************
 * @def spiffs_data_get
 * @brief Recupera os conteúdos de um arquivo SPIFFS
 * @param file, o Nome do arquivo a ser tratado
 * @return Contéudos presentes dentro de um arquivo SPIFFS
**************************************************************/

char* spiffs_data_get(char* file)
{
    // Open renamed file for reading
    ESP_LOGI(ESPIFFS, "Reading file");
    FILE* f  = fopen(file, "r");
    if (f == NULL) 
    {
        ESP_LOGE(ESPIFFS, "Failed to open file for reading");
    }
    fgets(spiffs_buffer, sizeof(spiffs_buffer), f);
    fclose(f);
    // strip newline
    char* pos = strchr(spiffs_buffer, '\n');
    if (pos) 
    {
        *pos = '\0';
    }
    return spiffs_buffer;
}

/*************************************************************
 * @def spiffs_data_save
 * @brief Salva os conteúdos em um arquivo SPIFFS
 * @param file, o Nome do arquivo a ser tratado
 * @param data, o conteúdo a ser salvo 
 * @return True or False para sucesso ou fracasso
**************************************************************/

bool spiffs_data_save(char* file, char* data)
{
    ESP_LOGI(ESPIFFS, "Opening file");
    FILE* f = fopen(file, "w");
    if (f == NULL) {
        ESP_LOGE(ESPIFFS, "Failed to open file for writing");
        return false;
    }
    fprintf(f, data);
    fclose(f);
    ESP_LOGI(ESPIFFS, "File written");
    return true;
}

/*************************************************************
 * @def spiffs_data_exists
 * @brief Verifica o status de um arquivo dentro do sistema
 * de arquivos
 * @param file, o Nome do arquivo a ser tratado
 * @return True or False para sucesso ou fracasso
**************************************************************/

bool spiffs_data_exists(char* file)
{
    struct stat st;
    return stat(file, &st) == 0;
}

/*************************************************************
 * @def spiffs_data_delete
 * @brief Deleta os arquivo dado no endereço file.
 * @attention CHECAR ANTES SE O ARQUIVO EXISTE.
 * @param file, o Nome do arquivo a ser tratado
 * @return None
**************************************************************/

void spiffs_data_delete(char* file)
{
    unlink(file);
    return;
}
