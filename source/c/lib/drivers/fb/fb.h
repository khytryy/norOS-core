#pragma once

#include <stdint.h>
#include "../../multiboot2/mb2.h"

typedef struct framebuffer __attribute__((packed)) {
  uint32_t type;
  uint32_t size;
  uint64_t fb_addr;
  uint32_t fb_pitch;
  uint32_t fb_width;
  uint32_t fb_height;
  uint8_t fb_bpp;
  uint8_t fb_type;
  uint8_t reserved[6];
};
