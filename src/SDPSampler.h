#pragma once

#include <Arduino.h>
#include "SDPSensors.h"


class SDPSampler {
    private:
        int m_window_size;
        SDP8XX m_sensor = SDP8XX(Address5);
        hw_timer_t* m_timer;
        QueueHandle_t xQueueRecords;

    public:
        SDPSampler(int window_size);
        bool begin();
        void stop();
        void startTimer();
        bool readSensor();
        int16_t* getCapturedAudioBuffer();
        bool ready();
};