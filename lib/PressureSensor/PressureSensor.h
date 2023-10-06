#pragma once

#include <Arduino.h>
#include "SDPSensors.h"
#include "dlvr.h"


typedef enum { MANUFACTURER_NONE, MANUFACTURER_SDP, MANUFACTURER_DLVR } Manufacturer;


class PressureSensor {
    private:
        Manufacturer mfr = MANUFACTURER_NONE;
        TwoWire &wire;
        SDPSensor *sdp_sensor = NULL;
        DLVR *dlvr_sensor = NULL;
        bool ping(uint8_t addr);

    public:
        PressureSensor(TwoWire &wirePort = Wire);
        ~PressureSensor();
        PressureRange begin();
        bool startContinuous();
        bool stopContinuous();
        bool readPressure(int16_t *pressure);
        uint8_t getPressureScale();
        int getSamplingFreq();
};
