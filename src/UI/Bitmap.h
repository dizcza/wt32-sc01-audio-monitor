#pragma once
#include <stdint.h>

class Bitmap
{
private:
  uint16_t* alloc_half_size();
public:
  uint16_t width;
  uint16_t height;
  uint16_t *pixels1;
  uint16_t *pixels2;
  uint16_t **rows;
  Bitmap(uint16_t a_width, uint16_t a_height);
  void scroll_left();
};