/*
 * Welford's Online algorithm to estimate population mean and variance.
 *
 *  Created on: Feb 6, 2019
 *      Author: Danylo Ulianych
 */

#ifndef ONLINEMEAN_H_
#define ONLINEMEAN_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Set to 1 to return unbiased (sample) variance
 * rather than a population variance */
#define UNBIASED_ESTIMATOR  0

#include <stdint.h>

typedef struct OnlineMean {
    uint32_t count;
    float mean;
#ifdef ONLINEMEAN_WITH_STD
    float varsum;  // variance sum
#endif  /* ONLINEMEAN_WITH_STD */
} OnlineMean;

typedef struct OnlineRMS {
    uint32_t count;
    uint32_t count_reset;
    float squared;    // RMS squared mean
    float mean;       // Raw data mean
    float max;
} OnlineRMS;

void OnlineMean_Init(OnlineMean *oMean);
void OnlineMean_Update(OnlineMean *oMean, float newValue);
float OnlineMean_GetMean(const OnlineMean *oMean);
void OnlineMean_Reset(OnlineMean *oMean);

#ifdef ONLINEMEAN_WITH_STD
float OnlineMean_GetStd(const OnlineMean *oMean);
#endif  /* ONLINEMEAN_WITH_STD */

void OnlineRMS_Init(OnlineRMS *orms, uint32_t count_reset);
void OnlineRMS_Reset(OnlineRMS *orms);
void OnlineRMS_Update(OnlineRMS *orms, float newValue);
float OnlineRMS_GetMax(const OnlineRMS *orms);


#ifdef __cplusplus
}
#endif

#endif /* ONLINEMEAN_H_ */
