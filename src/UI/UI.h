#pragma once

#include "Palette.h"
#include "Waveform.h"
#include "Spectrogram.h"
#include "GraphicEqualiser.h"
#include "Processor.h"
#include "RatioMetric.h"

class TFT_eSPI;

class UI
{
private:
  Palette m_palette;
  Waveform m_waveform;
  Waveform m_stalta_ratio_window;
  GraphicEqualiser m_graphic_equaliser;
  Spectrogram m_spectrogram;
  TFT_eSPI &m_display;
  RatioMetric m_stalta_ratio;
  unsigned long draw_time = 0;
  int draw_count = 0;
  int m_curr_window_id = 0;
  std::vector<std::vector<std::reference_wrapper<Component>>> m_windows;

  void set_curr_window_visible(bool visible);

public:
  UI(TFT_eSPI &display, int window_size);
  void toggle_display();
  void update(Processor &processor, const float *samples, int size);
  void draw();
  friend void drawing_task(void *param);
};
