#ifndef UI_BITMAP_H
#define UI_BITMAP_H

#include <stdint.h>

struct UIBitmap {
  uint32_t offset;
  uint32_t size;
  uint16_t width;
  uint16_t height;
};

#endif
