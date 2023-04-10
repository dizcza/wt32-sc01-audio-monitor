#include "Arduino.h"
#include "Bitmap.h"

uint16_t* Bitmap::alloc_half_size() {
  uint16_t *pixels = (uint16_t*) calloc(width * height / 2, sizeof(uint16_t));
  assert(pixels != NULL);
  return pixels;
}

Bitmap::Bitmap(uint16_t a_width, uint16_t a_height) : width(a_width), height(a_height)
{
  pixels1 = alloc_half_size();
  pixels2 = alloc_half_size();
  rows = (uint16_t**) malloc(sizeof(uint16_t*) * height);
  for (int i = 0; i < height / 2; i++)
  {
    rows[i] = pixels1 + width * i;
    rows[i + height / 2] = pixels2 + width * i;
  }
}

void Bitmap::scroll_left()
{
  for (int y = 0; y < height; y++)
  {
    memmove(rows[y], rows[y] + 1, this->width * sizeof(uint16_t) - 1);
  }
}