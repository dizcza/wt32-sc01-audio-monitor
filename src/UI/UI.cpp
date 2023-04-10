#include <Arduino.h>
#include <M5StX.h>
#include "UI.h"
#include "UI/Waveform.h"
#include "UI/Palette.h"
#include "UI/GraphicEqualiser.h"
#include "UI/Spectrogram.h"


UI::UI(TFT_eSPI &display, int window_size) : m_display(display),
  m_waveform(display, 0, 0, display.width(), display.height() / 2, window_size),
  m_graphic_equaliser(m_palette, 0, display.height() / 2, display.width(), display.height() / 2, window_size),
  m_spectrogram(m_palette, 0, 0, display.width(), display.height())
{
  log_d("Display is %d x %d\n", display.width(), display.height());
  m_waveform.visible = false;
  m_graphic_equaliser.visible = false;
  m_spectrogram.visible = true;
}

void UI::toggle_display()
{
  m_waveform.visible = !m_waveform.visible;
  m_graphic_equaliser.visible = !m_graphic_equaliser.visible;
  m_spectrogram.visible = !m_spectrogram.visible;
}

void UI::update(float *samples, float *fft)
{
  m_waveform.update(samples);
  m_graphic_equaliser.update(fft);
  m_spectrogram.update(fft);
  draw();
}

void UI::draw()
{
  auto start = millis();
  m_spectrogram.draw(m_display);
  m_graphic_equaliser.draw(m_display);
  m_waveform.draw(m_display);
  auto end = millis();
  draw_time += end - start;
  draw_count++;
  if (draw_count == 20)
  {
    log_d("Drawing time: %ld ms", draw_time / 20);
    draw_count = 0;
    draw_time = 0;
  }
}