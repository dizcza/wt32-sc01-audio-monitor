#include <Arduino.h>
#include <M5Display.h>
#include "Spectrogram.h"
#include "Bitmap.h"
#include "Palette.h"


Spectrogram::Spectrogram(const Palette &palette, int x, int y, int width, int height) : Component(x, y, width, height),
  m_palette(palette),
  bitmap(width, height)
{
}

void Spectrogram::update(const float *magnitudes, int size)
{
  bitmap.scroll_left();
  const float fft_id_scale = ((float) size) / bitmap.height;
  for (int i = 0; i < bitmap.height; i++)
  {
    int fft_id = (int) (i * fft_id_scale);
    int color_id = (int) magnitudes[fft_id];
    bitmap.rows[bitmap.height - i - 1][bitmap.width - 1] = m_palette.get_color(color_id);
  }
}

void Spectrogram::_draw(TFT_eSPI &display)
{
  bool swap = display.getSwapBytes();
  display.setSwapBytes(true);
  display.pushImage(x, y, bitmap.width, bitmap.height / 2, bitmap.pixels1);
  display.pushImage(x, y + bitmap.height / 2, bitmap.width, bitmap.height / 2, bitmap.pixels2);
  display.setSwapBytes(swap);
}