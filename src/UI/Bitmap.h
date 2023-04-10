#pragma once
#include <stdint.h>

#include <M5StX.h>

class Bitmap
{
public:
  static constexpr uint16_t width = TFT_HEIGHT;
  static constexpr uint16_t height = TFT_WIDTH / 2;
  uint16_t pixels[width * height];
  uint16_t *rows[height];
  Bitmap();
  void scroll_left();
};