#include <math.h>
#include <stdlib.h>

#include "trigger.h"


float recstalta(float a, recsta_t *rsta)
{
    const float a2 = powf(a, 2);
    rsta->sta = rsta->csta * a2 + (1.0f - rsta->csta) * rsta->sta;
    rsta->lta = rsta->clta * a2 + (1.0f - rsta->clta) * rsta->lta;
    return rsta->sta / rsta->lta;
}


void recstalta_init(recsta_t *rsta, int nsta, int nlta) {
    rsta->csta = 1.0f / nsta;
    rsta->clta = 1.0f / nlta;
    rsta->lta = rsta->sta = 0.0f;
}