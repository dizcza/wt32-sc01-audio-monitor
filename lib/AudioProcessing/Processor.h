#pragma once

#include <stdint.h>
#include "kiss_fftr.h"
#include "HammingWindow.h"


class Processor
{
private:
  HammingWindow m_hamming_window;
  int m_window_size;
  float *fft_input;
  kiss_fftr_cfg m_cfg;
  kiss_fft_cpx *m_fft_output;
  const float scale_factor;

public:
  int energy_size;
  float *energy;

  Processor(int window_size, uint8_t pressure_scale);
  void update(const float *samples, int size);
};