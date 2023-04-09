#pragma once

class Palette;
class Waveform;
class GraphicEqualiser;
class Spectrogram;
class TFT_eSPI;

class UI
{
private:
  Palette *m_palette;
  Waveform *m_waveform;
  GraphicEqualiser *m_graphic_equaliser;
  Spectrogram *m_spectrogram;
  TFT_eSPI &m_display;
  unsigned long draw_time = 0;
  int draw_count = 0;

public:
  UI(TFT_eSPI &display, int window_size);
  void toggle_display();
  void update(float *samples, float *fft);
  void draw();
  friend void drawing_task(void *param);
};
