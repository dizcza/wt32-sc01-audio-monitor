#pragma once

#include <Arduino.h>
#include "PressureSensor.h"
#include "SpectrogramConfig.h"
#include "onlinemean.h"


#define SDPSAMPLER_BUFFER_MAX_SIZE   512


class SDPSampler {
    private:
        PressureSensor m_sensor;
        OnlineMean oMean;
        hw_timer_t* m_timer;
        QueueHandle_t xQueueRecords;
        float diff_pressure_buffer[SDPSAMPLER_BUFFER_MAX_SIZE];

    public:
        uint8_t pressure_scale;

        SDPSampler();
        bool begin();
        void stop();
        void startTimer();
        bool readSensor();
        const float* getCapturedAudioBuffer(int *size);
};