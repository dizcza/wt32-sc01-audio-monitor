#include "PressureSensor.h"


PressureSensor::PressureSensor(TwoWire &wirePort) : wire(wirePort) {};

bool PressureSensor::ping(uint8_t addr) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    wire.beginTransmission(addr);
    uint8_t error = wire.endTransmission();
    return error == 0;
}

PressureRange PressureSensor::begin() {
    if (this->ping(DLVR_I2C_ADDR)) {
        log_i("Found DLVR sensor");
        mfr = MANUFACTURER_DLVR;
        dlvr_sensor = new DLVR(wire);
        return SDP_125;
    }
    sdp_sensor = new SDP8XX(Address5, DiffPressure, wire);
    PressureRange prange = sdp_sensor->begin();
    if (prange == SDP_NA) {
        log_e("No sensor found");
    } else {
        mfr = MANUFACTURER_SDP;
        log_i("Found SDP8xx sensor");
    }
    return prange;
}


PressureSensor::~PressureSensor() {
    if (sdp_sensor != NULL) {
        delete sdp_sensor;
    }
    if (dlvr_sensor != NULL) {
        delete dlvr_sensor;
    }
}


bool PressureSensor::startContinuous() {
    bool res = true;
    if (sdp_sensor != NULL) {
        res = sdp_sensor->startContinuous(false);
    }
    return res;
}


bool PressureSensor::stopContinuous() {
    bool res = true;
    if (sdp_sensor != NULL) {
        res = sdp_sensor->stopContinuous();
    }
    return res;
}


bool PressureSensor::readPressure(int16_t *pressure) {
    bool res;
    switch (mfr)
    {
    case MANUFACTURER_SDP:
        res = sdp_sensor->readPressure(pressure);
        break;
    case MANUFACTURER_DLVR:
        res = dlvr_sensor->readPressure(pressure);
        break;
    default:
        res = false;
        break;
    }
    return res;
}


uint8_t PressureSensor::getPressureScale() {
    uint8_t scale = 0;
    switch (mfr)
    {
    case MANUFACTURER_SDP:
        scale = sdp_sensor->getPressureScale();
        break;
    case MANUFACTURER_DLVR:
        scale = dlvr_sensor->getPressureScale();
        break;
    default:
        break;
    }
    return scale;
}


int PressureSensor::getSamplingFreq() {
    return mfr == MANUFACTURER_SDP ? 1024 : 256;
}
