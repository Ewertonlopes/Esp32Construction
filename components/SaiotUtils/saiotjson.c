#include "saiotjson.h"

/*************************************************************
 * Variaveis internas a biblioteca, ESPIFFS tag de erros, buffer -
 * utilização interna e conf trata das configurações dos arquivos. 
**************************************************************/

static const char *TAG_JSON = "SUBMODULE_JSON";

static char *JSON_Types(int type) {
	if (type == cJSON_Invalid) return ("cJSON_Invalid");
	if (type == cJSON_False) return ("cJSON_False");
	if (type == cJSON_True) return ("cJSON_True");
	if (type == cJSON_NULL) return ("cJSON_NULL");
	if (type == cJSON_Number) return ("cJSON_Number");
	if (type == cJSON_String) return ("cJSON_String");
	if (type == cJSON_Array) return ("cJSON_Array");
	if (type == cJSON_Object) return ("cJSON_Object");
	if (type == cJSON_Raw) return ("cJSON_Raw");
	return NULL;
}


/*************************************************************
 * @def json_create_config
 * @brief Cria um dispositivo Json com as configurações
 * do dispositivo
 * @param Device
 * @return cJson das configurações
**************************************************************/

char* json_create_config(Device main_dev)
{
    cJSON *root, *sensors, *actuators;
    root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "id", main_dev->Id);
    cJSON_AddStringToObject(root, "email", main_dev->Email);
    cJSON_AddStringToObject(root, "name", main_dev->Name);
    cJSON_AddStringToObject(root, "class", main_dev->Classe);
    cJSON_AddStringToObject(root, "description", main_dev->Description);
    cJSON_AddItemToObject(root, "sensors", sensors=cJSON_CreateObject());
    for(int i=0;i<main_dev->dispnumb;i++){
        bool isActuator = main_dev->Adds[i]->disp; 
        if(!isActuator){
            cJSON* newSensor = json_create_sensor(main_dev->Adds[i]->sensor);
            cJSON_AddItemToObject(sensors, main_dev->Adds[i]->sensor->Id, newSensor);
        }
    }    
    cJSON_AddItemToObject(root, "actuators", actuators=cJSON_CreateObject());
    for(int i=0;i<main_dev->dispnumb;i++){
        bool isActuator = main_dev->Adds[i]->disp; 
        if(isActuator) {
            cJSON* newact = json_create_actuator(main_dev->Adds[i]->actuator);
            cJSON_AddItemToObject(actuators, main_dev->Adds[i]->actuator->Id, newact);
        }
    }    
	
	char *my_json_string = cJSON_Print(root);
    cJSON_Delete(root);
    return my_json_string;
}

cJSON* json_create_sensor(Sensor main_sensor) {
    cJSON *root;
    root = cJSON_CreateObject();
    // intern=cJSON_CreateObject();
    // cJSON_AddStringToObject(intern,"type",		"rect");
	// cJSON_AddNumberToObject(intern,"width",		1920);
	// cJSON_AddNumberToObject(intern,"height",		1080);

    cJSON_AddStringToObject(root, "name", main_sensor->Name);
    cJSON_AddStringToObject(root, "type", main_sensor->type);

    switch(main_actuator->internal_type) {
        case sensor_number:
            cJSON_AddNumberToObject(root, "value", *(float*)main_sensor->data);
            break;
        default:
            ESP_LOGE(TAG_STRUCT, "INVALID SENSOR TYPE FOR PARSING JSON!!!");
            cJSON_AddNumberToObject(root, "value", 0);
            break;
    }

    cJSON_AddNumberToObject(root, "value", *(float*)main_sensor->data);


    cJSON_AddNumberToObject(root, "timeout", main_sensor->timeout);
    cJSON_AddNumberToObject(root, "deadband", main_sensor->deadband);
    // cJSON_AddFalseToObject(root, "Boolean_2");

    return root;
}

cJSON* json_create_actuator(Actuator main_actuator) {
    cJSON *root;
    root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "name", main_actuator->Name);
    cJSON_AddStringToObject(root, "type", main_actuator->type);
    switch(main_actuator->internal_type) {
        case act_switch:
            cJSON_AddNumberToObject(root, "value", *(bool*)main_actuator->data);
            break;
        default:
            ESP_LOGE(TAG_STRUCT, "INVALID ACTUATOR TYPE FOR PARSING JSON!!!");
            cJSON_AddNumberToObject(root, "value", 0);
            break;
    }
    
    // cJSON_AddFalseToObject(root, "Boolean_2");

    return root;
}

char* json_create_payload_float (char *ID,float value)
{
    cJSON *root;
    root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "id", ID);
    cJSON_AddNumberToObject(root, "value", value);
	
	char *my_json_string = cJSON_Print(root);
    cJSON_Delete(root);
    return my_json_string;
}

bool json_free_buffer(char* Json)
{
    cJSON_free(Json);
    return true;
}

cJSON* json_parse_object(char* Json)
{
    return cJSON_Parse(Json);
}

bool json_delete_object(cJSON* Json)
{
    cJSON_Delete(Json);
    return true;
}
