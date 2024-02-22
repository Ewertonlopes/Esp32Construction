#include "saiotclock.h"

static const char *timeTAG = "RTC Module";

RTC_DATA_ATTR int boot_count = 0;

static time_t saiotnow;
static struct tm saiottimeinfo;

void sntp_sync_time(struct timeval *tv)
{
   settimeofday(tv, NULL);
   ESP_LOGI(timeTAG, "Time is synchronized from custom code");
   sntp_set_sync_status(SNTP_SYNC_STATUS_COMPLETED);
}

void time_sync_notification_cb(struct timeval *tv)
{
    ESP_LOGI(timeTAG, "Notification of a time synchronization event");
}

void clock_start(void)
{
    ++boot_count;
    ESP_LOGI(timeTAG, "Boot count: %d", boot_count);
    
    
    time(&saiotnow);
    localtime_r(&saiotnow, &saiottimeinfo);
    // Is time set? If not, tm_year will be (1970 - 1900).
    if (saiottimeinfo.tm_year < (2022 - 1900)) {
        ESP_LOGI(timeTAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
        clock_obtain_time();
        time(&saiotnow);
    }

    setenv("TZ", "<-03>3", 1);
    tzset();
}

bool clock_get_time(char *time_buf)
{
    time(&saiotnow);
    localtime_r(&saiotnow, &saiottimeinfo);
    strftime(time_buf, sizeof(time_buf), "%d-%m-%Y %H-%M-%S", &saiottimeinfo);
    return true;
}

void clock_obtain_time(void)
{
    clock_initialize_sntp();

    // wait for time to be set
    time_t now = 0;
    struct tm timeinfo = { 0 };
    int retry = 0;
    const int retry_count = 10;
    while (sntp_get_sync_status() == SNTP_SYNC_STATUS_RESET && ++retry < retry_count) {
        ESP_LOGI(timeTAG, "Waiting for system time to be set... (%d/%d)", retry, retry_count);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    time(&now);
    localtime_r(&now, &timeinfo);
}

static void clock_initialize_sntp(void)
{
    ESP_LOGI(timeTAG, "Initializing SNTP");
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_set_time_sync_notification_cb(time_sync_notification_cb);

    esp_sntp_init();
}