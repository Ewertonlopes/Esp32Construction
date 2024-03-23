#include "saiotmqtt.h"

static const char *TAG_MQTT = "SUBMODULE_MQTT";

esp_mqtt_client_handle_t xClientMQTT = NULL;
SemaphoreHandle_t xMutex = NULL;

/* 
 * @brief Função para filtrar mensagens e printar aquelas com código de erro
 *
 * @param message A mensagem a ser checada
 * @param error_code O código de erro a se analisar
 */
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG_MQTT, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Handler de eventos do MQTT
 *
 *  Função utilizada no loop de client do MQTT.
 *
 * @param handler_args os dados do usuario no evento.
 * @param base Base do evento.
 * @param event_id Id do evento.
 * @param event_data Dados presentes no evento.
 */

static void saiot_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG_MQTT, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    
    if(xClientMQTT != NULL) xSemaphoreTake( xMutexMQTT, portMAX_DELAY );

    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_CONNECTED");
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG_MQTT, "Subscribed");
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG_MQTT, "%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG_MQTT, "MQTT_EVENT_ERROR");
            if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
                log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
                log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
                log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
                ESP_LOGI(TAG_MQTT, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
            }
            break;
        default:
            ESP_LOGI(TAG_MQTT, "Other event id:%d", event->event_id);
            break;
    }
    xSemaphoreGive( xMutexMQTT );
}

/*
 * @brief Função de inicialização do client GLOBAL do mqtt
 *
 *  O cliente MQTT só deve ser utilizado após essa função ser chamada.
 *
 * @param email é o email utilizado na plataforma SAIOT
 * @param password é a senha utilizada com o mesmo e-mail da plataforma SAIOT
 * @param id é a identificação única do dispositivo.
 */

 esp_err_t saiot_mqtt_app_start(const char *email, const char *password, const char* id){

	esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = BROKER_ADDR,
        .credentials.username = email,
        .credentials.authentication.password = password,
        .credentials.client_id=id,
        .task.priority=24
    };

    esp_err_t ret = ESP_OK;

    xClientMQTT = esp_mqtt_client_init(&mqtt_cfg);

    if(xClientMQTT == NULL) return ESP_FAIL;

    xMutexMQTT = xSemaphoreCreateMutex();
    
    if(xMutexMQTT == NULL) return ESP_FAIL;

    ret = esp_mqtt_client_register_event(xClientMQTT, ESP_EVENT_ANY_ID, saiot_mqtt_event_handler, NULL);
    ESP_RETURN_ON_ERROR(ret, TAG_MQTT, "Failed to register Event");
    ret = esp_mqtt_client_start(xClientMQTT);
    ESP_RETURN_ON_ERROR(ret, TAG_MQTT, "Failed to Start Client");
    
    return ret;
}