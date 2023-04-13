/*--------------------------------------------------------------------
# Filename: stalta.c
#  Purpose: Classic STA/LTA trigger, optimized C version
#   Author: Moritz Beyreuther
# Copyright (C) 2012 ObsPy-Developer-Team
#---------------------------------------------------------------------*/

#include <math.h>

#include "trigger.h"


int stalta(const float *data, float *charfct, int ndat, int nsta, int nlta)
{
    int i;
    float sta = 0.f;
    float lta;
    float buf;
    const float frac = (float) nlta / (float) nsta;

    if (ndat < nlta) {
        return 1;
    }

    for (i = 0; i < nsta; ++i) {
        sta += powf(data[i], 2);
        charfct[i] = 0.f;
    }
    lta = sta;
    for (i = nsta; i < nlta; ++i) {
        buf = powf(data[i], 2);
        lta += buf;
        sta += buf - powf(data[i - nsta], 2);
        charfct[i] = 0.f;
    }
    charfct[nlta - 1] = sta / lta * frac;
    for (i = nlta; i < ndat; ++i) {
        buf = powf(data[i], 2);
        sta += buf - powf(data[i - nsta], 2);
        lta += buf - powf(data[i - nlta], 2);
        charfct[i] = sta / lta * frac;
    }

    return 0;
}