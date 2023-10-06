#pragma once

#include <Arduino.h>
#include "SDPSensors.h"


typedef enum { MANUFACTURER_NONE, MANUFACTURER_SDP, MANUFACTURER_DLVR } Manufacturer;


class PressureSensor {
    private:
        Manufacturer mfr = MANUFACTURER_NONE;
        TwoWire &wire;
        SDP8XX *sdp_sensor;
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
