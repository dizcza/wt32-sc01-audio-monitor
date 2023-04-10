#include <Arduino.h>
#include <M5Display.h>
#include "Spectrogram.h"
#include "Bitmap.h"
#include "Palette.h"


Spectrogram::Spectrogram(Palette *palette, int x, int y, int width, int height) : Component(x, y, width, height), m_palette(palette), bitmap(width, height)
{
}

void Spectrogram::update(const float *mag)
{
  bitmap.scroll_left();
  for (int i = 0; i < bitmap.height; i++)
  {
    bitmap.rows[bitmap.height - i - 1][bitmap.width - 1] = m_palette->get_color(mag[i]);
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