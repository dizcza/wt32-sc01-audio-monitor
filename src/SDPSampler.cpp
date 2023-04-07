#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "esp_task_wdt.h"
#include "esp_ota_ops.h"

#include "SDPSampler.h"

#define RECORD_READ_SENSOR_PRIORITY  (configMAX_PRIORITIES - 1)



static void sdprecord_read_sensor_task(void*);


static TaskHandle_t m_task_read_handle;
static const char *TAG = "sdprecord";



void ARDUINO_ISR_ATTR onTimer()
{
    static uint32_t timer_counter = 0;
    xTaskNotifyFromISR(m_task_read_handle, timer_counter++, eSetValueWithOverwrite, NULL);
}



static void sdprecord_read_sensor_task(void* args) {
    SDPSampler* sdp_sampler = (SDPSampler*) args;
    int64_t rtc_counter = 0;

    esp_task_wdt_add(NULL);
    sdp_sampler->startTimer();

    // Skip the first sample.
    // The first sample period is less than expected.
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	while (1) {
	    esp_task_wdt_reset();
	    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        sdp_sampler->readSensor();
        if (sdp_sampler->ready()) {
            xTaskNotify(sdp_sampler->m_processing_task_handle, 1, eIncrement);
        }
	}
}


bool SDPSampler::ready() {
    return uxQueueMessagesWaiting(xQueueRecords) >= m_window_size;
}


int16_t* SDPSampler::getCapturedAudioBuffer() {
    if (!ready()) {
        return NULL;
    }
    SDPRecord record;
    int16_t* buff = (int16_t*) malloc(sizeof(int16_t) * m_window_size);
    for (int i = 0; i < m_window_size; i++) {
        xQueueReceive(xQueueRecords, &record, portMAX_DELAY);
        buff[i] = record.diff_pressure_raw;
    }
    return buff;
}



esp_err_t SDPSampler::readSensor() {
    const int64_t systick = esp_timer_get_time();
    int16_t dp_raw = 0;
    if (0) {
        static int16_t dp = 0;
        const SDPRecord record = {
                .systime = systick,
                .diff_pressure_raw = dp++
        };
        xQueueSend(xQueueRecords, &record, 0);
        return ESP_OK;
    }
    if (m_sensor.readMeasurement(&dp_raw, NULL, NULL)) {
        const SDPRecord record = {
                .systime = systick,
                .diff_pressure_raw = dp_raw
        };
        xQueueSend(xQueueRecords, &record, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}



void SDPSampler::startTimer()
{
    m_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(m_timer, &onTimer, true);

    // Set alarm to call onTimer function every second (value in microseconds).
    // Repeat the alarm (third parameter)
    timerAlarmWrite(m_timer, 1000000 / 1024, true);

    // Start an alarm
    timerAlarmEnable(m_timer);
}

SDPSampler::SDPSampler(int window_size, TaskHandle_t processing_task_handle) : m_window_size(window_size), m_processing_task_handle(processing_task_handle)
{
    m_sensor.begin();
    m_sensor.startContinuous(false);
    xQueueRecords = xQueueCreate(1000, sizeof(SDPRecord));
    assert(xQueueRecords != NULL);
    xTaskCreatePinnedToCore(sdprecord_read_sensor_task, "sdp_read", 4096, this, RECORD_READ_SENSOR_PRIORITY, &m_task_read_handle, APP_CPU_NUM);
}