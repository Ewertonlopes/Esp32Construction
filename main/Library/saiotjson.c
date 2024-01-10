#include "saiotjson.h"


/*************************************************************
 * Variaveis internas a biblioteca, ESPIFFS tag de erros, buffer -
 * utilização interna e conf trata das configurações dos arquivos. 
**************************************************************/

static const char *EJSON = "JSON";

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
 * @param None
 * @return cJson das configurações
**************************************************************/

char* json_create_config(char *ID,char *email,char *name,char *class,char *description,char* sensorid,cJSON *Sensor)
{
    cJSON *root, *sensors;
    root = cJSON_CreateObject();

    cJSON_AddStringToObject(root, "id", ID);
    cJSON_AddStringToObject(root, "email", email);
    cJSON_AddStringToObject(root, "name", name);
    cJSON_AddStringToObject(root, "class", class);
    cJSON_AddStringToObject(root, "description", description);
    cJSON_AddItemToObject(root, "sensors", sensors=cJSON_CreateObject());
    cJSON_AddItemToObject(sensors, sensorid, Sensor);

	
	char *my_json_string = cJSON_Print(root);
    cJSON_Delete(root);
    return my_json_string;
}

cJSON* json_create_sensor(char *name,char *type,int value,int timeout,int deadband)
{
    cJSON *root;
    root = cJSON_CreateObject();
    // intern=cJSON_CreateObject();
    // cJSON_AddStringToObject(intern,"type",		"rect");
	// cJSON_AddNumberToObject(intern,"width",		1920);
	// cJSON_AddNumberToObject(intern,"height",		1080);

    cJSON_AddStringToObject(root, "name", name);
    cJSON_AddStringToObject(root, "type", type);
    cJSON_AddNumberToObject(root, "value", value);
    cJSON_AddNumberToObject(root, "timeout", timeout);
    cJSON_AddNumberToObject(root, "deadband", deadband);
    // cJSON_AddFalseToObject(root, "Boolean_2");

    return root;
}

char* json_create_payload(char *ID,int value)
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
