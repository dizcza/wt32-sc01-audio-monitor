#pragma once

#include <stdint.h>
#include "tools/kiss_fftr.h"
#include "HammingWindow.h"


class Processor
{
private:
  HammingWindow m_hamming_window_chunk;
  HammingWindow m_hamming_window_full;
  int m_window_size;
  int m_window_size_full;
  kiss_fftr_cfg m_cfg;
  kiss_fft_cpx *m_fft_output;
  float *m_fft_input_full;

public:
  int energy_size;
  float *energy;
  float *fft_input;

  Processor(int window_size, int n_chunks);
  void update(const int16_t *samples);
};