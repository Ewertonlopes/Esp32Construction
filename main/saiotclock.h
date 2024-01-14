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


extern RTC_DATA_ATTR int boot_count;

void sntp_sync_time(struct timeval *tv);
void time_sync_notification_cb(struct timeval *tv);

void clock_start(void)
char *clock_get_time(void)

static void clock_obtain_time(void);

static void clock_initialize_sntp(void)

#ifdef __cplusplus
}
#endif


#endif
