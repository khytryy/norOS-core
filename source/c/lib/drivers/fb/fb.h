#pragma once

#include <stdint.h>
#include "../../multiboot2/mb2.h"

typedef struct { /* framebuffer struct info */

  uint64_t address;
  uint32_t pitch;
  uint32_t width;
  uint32_t height;
  uint8_t bpp;
  uint8_t type;

  union {

    struct {
      uint8_t r_field_pos;
      uint8_t r_mask_size;
      uint8_t g_field_pos;
      uint8_t g_mask_size;
      uint8_t b_field_pos;
      uint8_t b_mask_size;
    } rgb_dat;

    struct {
      uint16_t ncolors;
      struct multiboot_color* palette;
    } indexed;

  };

} framebuffer_t;

void fbInit(void* multiboot_info);

void putpixel(int x, int y, uint32_t color);