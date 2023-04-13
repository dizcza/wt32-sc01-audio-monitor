#include <Arduino.h>
#include <M5Display.h>
#include "UI.h"


UI::UI(TFT_eSPI &display, int window_size) : m_display(display),
  m_waveform(display, 0, 0, display.width(), display.height() / 2, window_size, WAVEFORM_SCALE_RAW_DATA, Palette::light_blue),
  m_stalta_ratio_window(display, 0, 0, display.width(), display.height(), window_size, WAVEFORM_SCALE_TRIGGER, Palette::white),
  m_graphic_equaliser(m_palette, 0, display.height() / 2, display.width(), display.height() / 2, window_size / 2),
  m_spectrogram(m_palette, 0, 0, display.width(), display.height())
{
  log_d("Display is %d x %d\n", display.width(), display.height());

  m_windows.push_back({m_spectrogram});
  m_windows.push_back({m_waveform, m_graphic_equaliser});
  m_windows.push_back({m_stalta_ratio_window});

  set_curr_window_visible(true);
}

void UI::set_curr_window_visible(bool visible) {
  for (Component& c : m_windows[m_curr_window_id]) {
    c.visible = visible;
  }
}

void UI::toggle_display()
{
  set_curr_window_visible(false);
  m_curr_window_id = (m_curr_window_id + 1) % m_windows.size();
  set_curr_window_visible(true);
}

void UI::update(Processor &processor, const float *samples, int size)
{
  float *ratios = (float*) malloc(size * sizeof(float));
  bool detected = m_stalta_ratio.process(samples, ratios, size);
  m_stalta_ratio_window.update(ratios, size);
  free(ratios);
  m_waveform.update(samples, size);
  m_graphic_equaliser.update(processor.energy);
  m_spectrogram.update(processor.energy, processor.energy_size, detected);
  draw();
}

void UI::draw()
{
  auto start = millis();
  m_spectrogram.draw(m_display);
  m_graphic_equaliser.draw(m_display);
  m_waveform.draw(m_display);
  m_stalta_ratio_window.draw(m_display);
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