#include "Processor.h"
#include "HammingWindow.h"
#include "tools/kiss_fftr.h"

Processor::Processor(int window_size) : m_window_size(window_size), energy_size(window_size / 2), m_hamming_window(window_size)
{
  // initialise kiss fftr
  m_cfg = kiss_fftr_alloc(m_window_size, false, 0, 0);

  fft_input = static_cast<float *>(calloc(m_window_size, sizeof(float)));
  m_fft_output = static_cast<kiss_fft_cpx *>(malloc(sizeof(kiss_fft_cpx) * energy_size));

  energy = static_cast<float *>(malloc(sizeof(float) * energy_size));
}

void Processor::update(const int16_t *samples)
{
  for (int i = 0; i < m_window_size; i++)
  {
    fft_input[i] = samples[i] / 30.0f;
  }
  // apply the hamming window
  m_hamming_window.applyWindow(fft_input);
  // do the fft
  kiss_fftr(
      m_cfg,
      fft_input,
      reinterpret_cast<kiss_fft_cpx *>(m_fft_output));

  for (int i = 0; i < energy_size; i++)
  {
    const float real = m_fft_output[i].r;
    const float imag = m_fft_output[i].i;
    energy[i] = sqrt((real * real) + (imag * imag));
  }
}