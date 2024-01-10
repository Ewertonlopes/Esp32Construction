#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "time.h"
#include "esp_sntp.h"

#include "mwifi.h"

static const char *TAG = "RTC";

RTC_DATA_ATTR static int boot_count = 0;

static void obtain_time(void);
static void initialize_sntp(void);

void sntp_sync_time(struct timeval *tv)
{
   settimeofday(tv, NULL);
   ESP_LOGI(TAG, "Time is synchronized from custom code");
   sntp_set_sync_status(SNTP_SYNC_STATUS_COMPLETED);
}

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(TAG, "Notification of a time synchronization event");
}

void app_main(void)
{
    ++boot_count;
    ESP_LOGI(TAG, "Boot count: %d", boot_count);

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2022 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        obtain_time();
        // update 'now' variable with current time
        time(&now);
    }

    char strftime_buf[64];
    setenv("TZ", "<-03>3", 1);
    tzset();

    while(1)
    {
        time(&now);
        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%d-%m-%Y %H-%M-%S", &timeinfo);
        ESP_LOGI(TAG, "The current date/time Here: %s", strftime_buf);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

static void obtain_time(void)
{
    /* This helper function configures Wi-Fi or Ethernet, as selected in menuconfig.
     * Read "Establishing Wi-Fi or Ethernet Connection" section in
     * examples/protocols/README.md for more information about this function.
     */
    connect_wifi();

    initialize_sntp();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(TAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}

static void initialize_sntp(void)
{
    ESP_LOGI(TAG, "Initializing SNTP");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_set_time_sync_notification_cb(time_sync_notification_cb);

    esp_sntp_init();
}