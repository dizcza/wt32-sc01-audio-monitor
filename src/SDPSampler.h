#pragma once

#include <Arduino.h>
#include "SDPSensors.h"
#include "AppConfig.h"


class SDPSampler {
    private:
        SDP8XX m_sensor = SDP8XX(Address5);
        hw_timer_t* m_timer;
        QueueHandle_t xQueueRecords;
        int16_t raw_buffer[SPECTROGRAM_WINDOW_SIZE];

    public:
        SDPSampler();
        bool begin();
        void stop();
        void startTimer();
        bool readSensor();
        const int16_t* getCapturedAudioBuffer();
        bool ready();
};