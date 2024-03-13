#include "saiotmqtt.h"

static const char *MQTTAG = "MQTT101";

static char read_topic[150] = {0};
static char read_data[150] = {0};
long velocity,velocity2;
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(MQTTAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void saiot_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(MQTTAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(MQTTAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, "273e255d-125b-47ca-979c-66b29263fd35/message", 0);
        ESP_LOGI(MQTTAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "273e255d-125b-47ca-979c-66b29263fd35/config", 0);
        ESP_LOGI(MQTTAG, "sent subscribe successful, msg_id=%d", msg_id);
        msg_id = esp_mqtt_client_subscribe(client, "273e255d-125b-47ca-979c-66b29263fd35/act", 0);
        ESP_LOGI(MQTTAG, "sent subscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(MQTTAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(MQTTAG, "Subscribed");
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(MQTTAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(MQTTAG, "%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        // ESP_LOGI(MQTTAG, "MQTT_EVENT_DATA");
        // printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        // printf("DATA=%.*s\r\n", event->data_len, event->data);

        // sprintf(read_topic, "%.*s", event->topic_len, event->topic);
        // sprintf(read_data, "%.*s", event->data_len, event->data);

        // if(strcmp(read_topic, "/topic/qos0") == 0) 
        // {
        //     long receive = atol(read_data);
        //     velocity = receive;
        // }
        // else if(strcmp(read_topic, "/topic/qos1") == 0)
        // {
        //     long receive = atol(read_data);
        //     velocity2 = receive;

        // }
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(MQTTAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(MQTTAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));
        }
        break;
    default:
        ESP_LOGI(MQTTAG, "Other event id:%d", event->event_id);
        break;
    }
}

esp_mqtt_client_handle_t saiot_mqtt_app_start(const char *email, const char *password, const char* id){
	esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = BROKER_ADDR,
        .credentials.username = email,
        .credentials.authentication.password = password,
        .credentials.client_id=id,
        .task.priority=10
    };


    esp_mqtt_client_handle_t assclient = esp_mqtt_client_init(&mqtt_cfg);

    esp_mqtt_client_register_event(assclient, ESP_EVENT_ANY_ID, saiot_mqtt_event_handler, NULL);
    esp_mqtt_client_start(assclient);   
    return assclient;
}