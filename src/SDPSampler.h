#include <Arduino.h>
#include "SDPSensors.h"


typedef struct __attribute__ ((__packed__)) {
   int64_t systime;                            /*!< System imprecise time, us    */
   int16_t diff_pressure_raw;                  /*!< Sensor measurement unscaled  */
} SDPRecord;


class SDPSampler {
    private:
        SDP8XX m_sensor = SDP8XX(Address5, DiffPressure, Wire1);
        hw_timer_t* m_timer;
        QueueHandle_t xQueueRecords;

    public:
        TaskHandle_t m_processing_task_handle;
        int m_window_size;

        SDPSampler(int window_size, TaskHandle_t processing_task_handle);
        void startTimer();
        esp_err_t readSensor();
        int16_t* getCapturedAudioBuffer();
        bool ready();
};