/*
 * Welford's Online algorithm to estimate population mean and variance.
 *
 *  Created on: Feb 6, 2019
 *      Author: Danylo Ulianych
 */

#include <math.h>
#include "onlinemean.h"

void OnlineMean_Init(OnlineMean *oMean) {
    OnlineMean_Reset(oMean);
}

void OnlineMean_Update(OnlineMean *oMean, float newValue) {
    oMean->count++;
    if (oMean->count > 1) {
        float delta = newValue - oMean->mean;
        oMean->mean += delta / oMean->count;
#ifdef ONLINEMEAN_WITH_STD
        oMean->varsum += delta * (newValue - oMean->squared);
#endif  /* ONLINEMEAN_WITH_STD */
    } else {
        oMean->mean = newValue;
    }
}

float OnlineMean_GetMean(const OnlineMean *oMean) {
    return oMean->mean;
}


#ifdef ONLINEMEAN_WITH_STD
float OnlineMean_GetStd(const OnlineMean *oMean) {
#if UNBIASED_ESTIMATOR
    return sqrtf(oMean->varsum / (oMean->count - 1));
#else
    return sqrtf(oMean->varsum / oMean->count);
#endif  /* UNBIASED_ESTIMATOR */
}
#endif  /* ONLINEMEAN_WITH_STD */


void OnlineMean_Reset(OnlineMean *oMean) {
    oMean->count = 0;
    oMean->mean = 0.f;
#ifdef ONLINEMEAN_WITH_STD
    oMean->varsum = 0.f;
#endif  /* ONLINEMEAN_WITH_STD */
}


void OnlineRMS_Init(OnlineRMS *orms, uint32_t count_reset) {
    OnlineRMS_Reset(orms);
    orms->count_reset = count_reset;
}


void OnlineRMS_Reset(OnlineRMS *orms) {
    orms->count = 0;
    orms->squared = orms->max = orms->mean = 0.f;
}


void OnlineRMS_Update(OnlineRMS *orms, float newValue) {
    if (orms->count == orms->count_reset) {
        OnlineRMS_Reset(orms);
    }
    orms->count++;
    if (orms->count > 1) {
        float delta = newValue - orms->mean;
        orms->mean += delta / orms->count;
        delta = delta * delta - orms->squared;
        orms->squared += delta / orms->count;
    } else {
        orms->mean = newValue;
        orms->squared = 0;
    }
    if (orms->count > 5 && orms->squared > orms->max) {
        // skip first several samples (burnout)
        orms->max = orms->squared;
    }
}


float OnlineRMS_GetMax(const OnlineRMS *orms) {
    return sqrtf(orms->max);
}

