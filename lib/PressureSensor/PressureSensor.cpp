#include "PressureSensor.h"
#include "dlvr.h"


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
        mfr = MANUFACTURER_DLVR;
        dlvr_init((i2c_port_t) 0);
        return SDP_125;
    }
    sdp_sensor = new SDP8XX(Address5);
    PressureRange prange = sdp_sensor->begin();
    mfr = prange == SDP_NA ? MANUFACTURER_NONE : MANUFACTURER_SDP;
    return prange;
}


PressureSensor::~PressureSensor() {
    if (sdp_sensor != NULL) {
        delete sdp_sensor;
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
    if (sdp_sensor != NULL) {
        return sdp_sensor->readPressure(pressure);
    }
    return dlvr_read_pressure_sdp(pressure) == ESP_OK;
}


uint8_t PressureSensor::getPressureScale() {
    if (sdp_sensor != NULL) {
        return sdp_sensor->getPressureScale();
    }
    return DLVR_SDP_PRESSURE_SCALE;
}


int PressureSensor::getSamplingFreq() {
    return mfr == MANUFACTURER_SDP ? 1024 : 256;
}
