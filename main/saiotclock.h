#ifndef SAIOTCLOCK_h
#define SAIOTCLOCK_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "time.h"
#include "esp_sntp.h"


extern int boot_count;

void clock_sntp_sync_time(struct timeval *tv);

void clock_start(void);
bool clock_get_time(char *time_buf);

#ifdef __cplusplus
}
#endif


#endif
