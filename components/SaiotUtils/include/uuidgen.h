#ifndef UUIDGEN_H
#define UUIDGEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <esp_random.h>
#include <esp_log.h>

void UUIDGen(char *returnUUID);

#ifdef __cplusplus
}
#endif

#endif