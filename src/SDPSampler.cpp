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


static TaskHandle_t m_task_read_handle = NULL;
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
	}
}


const float* SDPSampler::getCapturedAudioBuffer(int *size) {
    int16_t dp_raw;
    int i = 0;
    for (; (i < SDPSAMPLER_BUFFER_MAX_SIZE) && uxQueueMessagesWaiting(xQueueRecords); i++) {
        xQueueReceive(xQueueRecords, &dp_raw, portMAX_DELAY);
        diff_pressure_buffer[i] = ((float) dp_raw) / pressure_scale;
    }
    *size = i;
    return diff_pressure_buffer;
}


bool SDPSampler::readSensor() {
    int16_t dp_raw = 0;
    if (m_sensor.readPressure(&dp_raw)) {
        xQueueSend(xQueueRecords, &dp_raw, 0);
        return true;
    }
    return false;
}


/**
 * Better to call this function from the core
 * on which the sampler is running.
*/
void SDPSampler::startTimer()
{
    m_timer = timerBegin(0, 80, true);

    // Edge interrupt (3rd parameter) is not supported.
    // Setting to false.
    timerAttachInterrupt(m_timer, &onTimer, false);

    // Set alarm to call onTimer function every second (value in microseconds).
    // Repeat the alarm (third parameter)
    timerAlarmWrite(m_timer, 1000000 / 1024, true);

    // Start an alarm
    timerAlarmEnable(m_timer);
}

bool SDPSampler::begin() {
    if (!m_sensor.startContinuous(false)) {
        log_e("SDPSensor::startContinuous failed");
        return false;
    }
    xTaskCreatePinnedToCore(sdprecord_read_sensor_task, "sdp_read", 4096, this, RECORD_READ_SENSOR_PRIORITY, &m_task_read_handle, APP_CPU_NUM);
    return true;
}

void SDPSampler::stop() {
    m_sensor.stopContinuous();
    if (m_timer) {
        timerStop(m_timer);
        m_timer = NULL;
    }
    if (m_task_read_handle) {
        esp_task_wdt_delete(m_task_read_handle);
        vTaskDelete(m_task_read_handle);
        m_task_read_handle = NULL;
    }
}

SDPSampler::SDPSampler(SDPSensor& sensor) : m_sensor(sensor)
{
    m_sensor.begin();
    pressure_scale = m_sensor.getPressureScale();
    log_d("SDP pressure scale: %d", pressure_scale);
    xQueueRecords = xQueueCreate(5000, sizeof(int16_t));
    assert(xQueueRecords != NULL);
}