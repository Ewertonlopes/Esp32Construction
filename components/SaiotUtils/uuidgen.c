#include "uuidgen.h"

static const char *TAG_UUID = "SUBMODULE_UUIDGENERATOR";

/*
 * @brief Função auxiliar para transformar inteiros em HEX
 *
 * @param inInt Valor em inteiro
 * @returnVar Valor em Hexadecimal
 */

static void IntToHex(const unsigned int inInt, char *returnVar)
{
  const char *HEXMAP = "0123456789abcdef";
  for (int i = 0; i < 8; i++){
    returnVar[7 - i] = HEXMAP[(inInt >> (i * 4)) & 0b1111];
  }
  ESP_LOGI(TAG_UUID,"Created HEXvalues");
}

/*
 * @brief Função que retorna uma UUID v4 válida
 *
 * @param returnUUID o valor de retorno da uuid
 */

void UUIDGen(char *returnUUID)
{
  for (int i = 0; i < 4; i++)
  {
    unsigned int chunk = esp_random();
    if (i == 1)
    {
      chunk &= 0xFFFF0FFF;
      chunk |= 0x00004000;
    }
    if (i == 2)
    {
      chunk &= 0b00111111111111111111111111111111;
      chunk |= 0b10000000000000000000000000000000;
    }
    char chunkChars[8];
    IntToHex(chunk, chunkChars);
    for (int p = 0; p < 8; p++)
    {
      returnUUID[p + 8 * i] = chunkChars[p];
    }
  }
  int dashOffset = 4;
  const int UUID_NUM_DIGITS = 32;
  for (int i = UUID_NUM_DIGITS - 1; i >= 0; i--)
  {
    if (i == 7 || i == 11 || i == 15 || i == 19)
    {
      returnUUID[i + dashOffset--] = '-';
    }
    returnUUID[i + dashOffset] = returnUUID[i];
  }
  returnUUID[36] = 0;
  ESP_LOGI(TAG_UUID,"UUID FINISHED... Returning String!!!");
}