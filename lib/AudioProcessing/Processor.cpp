#include "Processor.h"
#include "HammingWindow.h"


Processor::Processor(int window_size, uint8_t pressure_scale) :
  m_window_size(window_size),
  energy_size(window_size / 2),
  m_hamming_window(window_size),
  scale_factor(pressure_scale / 30.f)  // 30 - is the magical number from the original repository
{
  // initialise kiss fftr
  m_cfg = kiss_fftr_alloc(m_window_size, false, 0, 0);

  fft_input = static_cast<float *>(calloc(m_window_size, sizeof(float)));

  m_fft_output = static_cast<kiss_fft_cpx *>(calloc(energy_size, sizeof(kiss_fft_cpx)));
  energy = static_cast<float *>(calloc(energy_size, sizeof(float)));

}

void Processor::update(const float *samples, int size)
{
  if (size < m_window_size) {
    int offset = m_window_size - size;
    memmove(fft_input, &fft_input[size], offset * sizeof(float));
    for (int i = 0; i < size; i++)
    {
      fft_input[offset + i] = samples[i] * scale_factor;
    }
  } else {
    int i_start = size - m_window_size;
    for (int i = i_start; i < size; i++)
    {
      fft_input[i - i_start] = samples[i] * scale_factor;
    }
  }

  m_hamming_window.applyWindow(fft_input);

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