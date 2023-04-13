#pragma once

#include <stdint.h>
#include "trigger.h"


class RatioMetric {
    private:
        recsta_t rsta;
    public:
        RatioMetric() {
            recstalta_init(&rsta, TRIGGER_RECSTA_NSTA, TRIGGER_RECSTA_NLTA);
        }
        bool process(const float *samples, float *ratios_out, int size) {
            bool detected = false;
            for (int i = 0; i < size; i++) {
                ratios_out[i] = recstalta(samples[i], &rsta);
                detected |= ratios_out[i] > TRIGGER_RECSTA_THRESHOLD;
            }
            return detected;
        }
};