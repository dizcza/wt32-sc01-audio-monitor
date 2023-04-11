#pragma once

#include <stdint.h>
#include "tools/kiss_fftr.h"
#include "HammingWindow.h"


class Processor
{
private:
  HammingWindow m_hamming_window;
  int m_window_size;
  kiss_fftr_cfg m_cfg;
  kiss_fft_cpx *m_fft_output;

public:
  int energy_size;
  float *energy;
  float *fft_input;

  Processor(int window_size);
  void update(const int16_t *samples);
};