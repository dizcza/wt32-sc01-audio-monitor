/*
    SDP3x.cpp - Library for the SDP31, SDP32, SDP8xx digital pressure sensors produced by Sensirion.
    Created by Bryan T. Meyers, February 14, 2017. 
    Modified by UT2UH on May 9th, 2021 to add SDP8xx sensors.

    Copyright (c) 2018 Bryan T. Meyers

    Permission is hereby granted, free of charge, to any person obtaining a copy of this software
    and associated documentation files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies or
    substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
    BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
    DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "SDPSensors.h"

/*  Send a write command

    @param cmd - the two byte command to send
    @returns true iff all ACKs received
*/
bool SDPSensor::writeCommand(const uint8_t cmd[2]) {
    size_t written;
    uint8_t status;
    this->port->beginTransmission(this->addr);
    written = this->port->write(cmd, 2);
    status  = this->port->endTransmission();
    return (status == 0) && (written == 2);
}

/*  Read data back from the device

    @param words - the number of words to read
    @returns true iff all words read and CRC passed
*/
bool SDPSensor::readData(uint8_t words) {
    size_t read;
    uint8_t crc = 0xFF;
    uint8_t *next;
    bool success = true;
    // Clear buffer
    for (next = &this->buffer[12]; next > this->buffer; next--) {
        *next = 0;
    }
    // Each word is two bytes plus a CRC byte, ergo 3 bytes per word
    read = this->port->requestFrom(this->addr, (uint8_t)(words * 3));

    /*  We should have read the requested number of bytes.
        If not, we need to clear the bytes read anyways.
    */
    if (read != 3 * words) {
        success = false;
    }
    /*  Calculate CRC while reading bytes

        Adapted from:
        http://www.sunshine2k.de/articles/coding/crc/understanding_crc.html
    */
    next = this->buffer;
    for (; read > 0; read--) {
        // Read next available byte
        *next = this->port->read();
        // Every third byte
        if ((read % 3) == 1) {
            // Check CRC byte
            success = success && (crc == *next);
            crc     = 0xFF;
        } else {
            // Update CRC
            crc = CRC_LUT[crc ^ *next];
            // Go to next byte
            next++;
        }
    }
    return success;
}

/*  Constructor

    @param addr - the Address value for I2C
    @param comp - the Temperature Compensation Mode (Mass Flow or Differential Pressure)
    @returns a new SDP3X as configured
*/
SDPSensor::SDPSensor(const uint8_t addr, const TempCompensation comp, TwoWire &wirePort) {
    this->port = &wirePort;
    this->comp = comp;
    this->addr = addr;
}

/*  Finish Initializing the sensor object

    @returns sensor pressure range iff everything went correctly, or false iff not found
*/
PressureRange SDPSensor::begin() {
    uint32_t modelNumber;
    if (!readProductID(&modelNumber, NULL)) {
        return SDP_NA;
    }
    switch (modelNumber) {
        case SDP31_PID:
            this->number = SDP31_500;
            this->scale = DiffScale_500Pa;
            return SDP_500;
        case SDP32_PID:
            this->number = SDP32_125;
            this->scale = DiffScale_125Pa;
            return SDP_125;
        case SDP800_500_PID:
            this->number = SDP800_500;
            this->scale = DiffScale_500Pa;
            return SDP_500;
        case SDP810_500_PID:
            this->number = SDP810_500;
            this->scale = DiffScale_500Pa;
            return SDP_500;
        case SDP801_500_PID:
            this->number = SDP801_500;
            this->scale = DiffScale_500Pa;
            return SDP_500;
        case SDP811_500_PID:
            this->number = SDP811_500;
            this->scale = DiffScale_500Pa;
            return SDP_500;
        case SDP800_125_PID:
            this->number = SDP800_125;
            this->scale = DiffScale_125Pa;
            return SDP_125;
        case SDP810_125_PID:
            this->number = SDP810_125;
            this->scale = DiffScale_125Pa;
            return SDP_125;
        default:
            /* do nothing for now */
            return SDP_NA;
    }
}

/*  Begin taking continuous readings

    @param averaging - average samples until read occurs, otherwise read last value only
    @returns true, iff everything went correctly
*/
bool SDPSensor::startContinuous(bool averaging) {
    switch (this->comp) {
    case MassFlow:
        if (averaging) {
            return writeCommand(StartContMassFlowAvg);
        } else {
            return writeCommand(StartContMassFlow);
        }
    case DiffPressure:
        if (averaging) {
            return writeCommand(StartContDiffPressureAvg);
        } else {
            return writeCommand(StartContDiffPressure);
        }
    default:
        return false;
    }
}

/*  Disable continuous measurements

    This may be useful to conserve power when sampling all the time is no longer necessary.
    @returns true, iff everything went correctly
*/
bool SDPSensor::stopContinuous() {
    return writeCommand(StopCont);
}

/*  Start a one-shot reading

    @param averaging - average samples until read occurs, otherwise read last value only
    @returns true, iff everything went correctly
*/
bool SDPSensor::triggerMeasurement(bool stretching) {
    switch (this->comp) {
    case MassFlow:
        if (stretching) {
            return writeCommand(TrigMassFlowStretch);
        } else {
            return writeCommand(TrigMassFlow);
        }
    case DiffPressure:
        if (stretching) {
            return writeCommand(TrigDiffPressureStretch);
        } else {
            return writeCommand(TrigDiffPressure);
        }
    default:
        return false;
    }
}


bool SDPSensor::readPressure(int16_t *pressure) {
    uint8_t words = 1;  // pressure is one word
    if (!readData(words)) {
        return false;
    }
    
    *pressure <<= 8;
    *pressure |= (int16_t)this->buffer[0];
    *pressure <<= 8;
    *pressure |= (int16_t)this->buffer[1];

    return true;
}


/*  Get a pending reading.

    This may be used periodically or in a call-back when monitoring interrupts.

    Both "temp" and "scale" should be left NULL if not used. This will reduce read times.
    @param pressure - a pointer to store the raw pressue value
    @param temp  - a pointer to store the raw temperature value
    @param scale - a pointer to store the pressure scaling factor
    @returns the raw pressure reading (no scaling)
    @returns true, iff everything went correctly
*/
bool SDPSensor::readMeasurement(int16_t *pressure, int16_t *temp, int16_t *scale) {
    uint8_t words = 1;
    if (scale != NULL) {
        words = 3;
    } else if (temp != NULL) {
        words = 2;
    }
    if (!readData(words)) {
        return false;
    }
    /*  Data Format:
        | Byte  |  0  |  1  |  2  |  3  |  4  |  5  |
        | Value | pressure  | temp      | scale     |
    */
    switch (words) {
    case 3:
        if (scale != NULL) {
            *scale <<= 8;
            *scale |= (int16_t)this->buffer[4];
            *scale <<= 8;
            *scale |= (int16_t)this->buffer[5];
        }
    case 2:
        if (temp != NULL) {
            *temp <<= 8;
            *temp |= (int16_t)this->buffer[2];
            *temp <<= 8;
            *temp |= (int16_t)this->buffer[3];
        }
    case 1:
        if (pressure != NULL) {
            *pressure <<= 8;
            *pressure |= (int16_t)this->buffer[0];
            *pressure <<= 8;
            *pressure |= (int16_t)this->buffer[1];
        }
    }
    return true;
}

/*  Read back the sensor's internal information

    If a serial number is not needed, "serial" should be set to NULL. This will reduce read
   times.
    @param pid     - a pointer to store the 32-bit product ID
    @param serial  - if not null, a pointer to store the 64-bit manufacturer serial number
    @returns true, iff everything went correctly
*/
bool SDPSensor::readProductID(uint32_t *pid, uint64_t *serial) {
    uint8_t words = 2;
    if (serial != NULL) {
        words = 6;
    }
    // Send first command
    if (!writeCommand(ReadInfo1)) {
        return false;
    }
    // Send decond command
    if (!writeCommand(ReadInfo2)) {
        return false;
    }
    // Read back required data
    if (!readData(words)) {
        return false;
    }
    /*  Data Format:
        | Byte  | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
        | Value | pid           | serial                          |
    */
    switch (words) {
    case 6:
        // "Parse" serial number
        if (serial != NULL) {
            for (words = 0; words < 8; words++) {
                *serial <<= 8;
                *serial |= (uint64_t)this->buffer[words + 4];
            }
        }
    case 2:
        // "Parse" product identifer
        if (pid != NULL) {
            for (words = 0; words < 4; words++) {
                *pid <<= 8;
                *pid |= (uint32_t)this->buffer[words];
            }
        }
    }
    return true;
}

/*  Reset the device to default settings

    WARNING: This will reset all other I2C devices that support it.
    @returns true, iff everything went correctly
*/
bool SDPSensor::reset() {
    size_t written = 0;
    uint8_t status;
    this->port->beginTransmission(SoftReset[0]);
    written = this->port->write(SoftReset[1]);
    status  = this->port->endTransmission();
    return (written == 1) && (status == 0);
}

/*  Get the Pressure Scale for this sensor

    @returns scale in units of 1/Pa
*/
uint8_t SDPSensor::getPressureScale() {
    return this->scale;
}

/*  Get the Temperature Scale for this sensor

    @returns scale in units of 1/C
*/
uint8_t SDPSensor::getTemperatureScale() {
    return SDP3X_TempScale;
}
