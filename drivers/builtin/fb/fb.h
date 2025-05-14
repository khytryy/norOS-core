#pragma once

#include <stdint.h>
#include <multiboot2/mb2.h>
#include <serial/serial.h>
#include <stddef.h>


extern uint32_t* framebuffer;
extern uint32_t fb_width;
extern uint32_t fb_height;
extern uint8_t fb_bpp;
extern uint32_t fb_pitch;

void fbInit(void* pointer);
void fbplot(int x, int y, uint32_t color);
void test_framebuffer_access();
uint32_t fbread(int x, int y);