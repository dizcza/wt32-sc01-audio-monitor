#include "Processor.h"
#include "HammingWindow.h"
#include "tools/kiss_fftr.h"

Processor::Processor(int window_size, int n_chunks) : m_window_size(window_size),
  m_window_size_full(window_size * n_chunks),
  energy_size(window_size / 2 * n_chunks),
  m_hamming_window_chunk(window_size),
  m_hamming_window_full(m_window_size_full)
{
  // initialise kiss fftr
  m_cfg = kiss_fftr_alloc(m_window_size_full, false, 0, 0);

  fft_input = static_cast<float *>(calloc(m_window_size, sizeof(float)));
  m_fft_input_full = static_cast<float *>(calloc(m_window_size_full, sizeof(float)));

  m_fft_output = static_cast<kiss_fft_cpx *>(malloc(sizeof(kiss_fft_cpx) * energy_size));
  energy = static_cast<float *>(malloc(sizeof(float) * energy_size));
}

void Processor::update(const int16_t *samples)
{
  for (int i = 0; i < m_window_size; i++)
  {
    fft_input[i] = samples[i] / 30.0f;
  }

  memmove(m_fft_input_full, &m_fft_input_full[m_window_size], (m_window_size_full - m_window_size) * sizeof(float));
  memmove(&m_fft_input_full[m_window_size_full - m_window_size], fft_input, m_window_size * sizeof(float));

  // apply the hamming window
  m_hamming_window_chunk.applyWindow(fft_input);
  m_hamming_window_full.applyWindow(m_fft_input_full);

  // do the fft
  kiss_fftr(
      m_cfg,
      m_fft_input_full,
      reinterpret_cast<kiss_fft_cpx *>(m_fft_output));

  for (int i = 0; i < energy_size; i++)
  {
    const float real = m_fft_output[i].r;
    const float imag = m_fft_output[i].i;
    energy[i] = sqrt((real * real) + (imag * imag));
  }
}