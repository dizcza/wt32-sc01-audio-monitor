#pragma once

#include <Arduino.h>
#include "SDPSensors.h"
#include "SpectrogramConfig.h"


class SDPSampler {
    private:
        SDPSensor& m_sensor;
        hw_timer_t* m_timer;
        QueueHandle_t xQueueRecords;
        int16_t raw_buffer[SPECTROGRAM_WINDOW_SIZE];

    public:
        SDPSampler(SDPSensor& sensor);
        bool begin();
        void stop();
        void startTimer();
        bool readSensor();
        const int16_t* getCapturedAudioBuffer();
        bool ready();
};