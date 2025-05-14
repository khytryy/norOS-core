#include "fb.h"

uint32_t* framebuffer = 0;
uint32_t fb_width = 0;
uint32_t fb_height = 0;
uint8_t fb_bpp = 0;
uint32_t fb_pitch = 0;

uint64_t fb_phys_addr = 0;

void fbInit(void* pointer) {
    struct multiboot_tag* tag = (struct multiboot_tag*)((uint8_t*)pointer + 8);

    while (tag->type != 0) {
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            struct multiboot_tag_framebuffer_common* fbtag =
                (struct multiboot_tag_framebuffer_common*)tag;

            framebuffer = (uint32_t*)(uint64_t)fbtag->framebuffer_addr;
            fb_phys_addr = fbtag->framebuffer_addr;
            fb_width = fbtag->framebuffer_width;
            fb_height = fbtag->framebuffer_height;
            fb_bpp = fbtag->framebuffer_bpp;
            fb_pitch = fbtag->framebuffer_pitch;

            // logging
            char buff_addr[32], buff_pitch[32], buff_width[32], buff_height[32], buff_bpp[32];
            itoa_hex((uint64_t)framebuffer, buff_addr);
            itoa_dec(fb_pitch, buff_pitch);
            itoa_dec(fb_width, buff_width);
            itoa_dec(fb_height, buff_height);
            itoa_dec(fb_bpp, buff_bpp);

            serialWrite("framebuffer initialized successfully!\n");

            serialWrite("fb :: address: 0x"); serialWrite(buff_addr); newline();
            serialWrite("fb :: width: "); serialWrite(buff_width); newline();
            serialWrite("fb :: height: "); serialWrite(buff_height); newline();
            serialWrite("fb :: pitch: "); serialWrite(buff_pitch); newline();
            serialWrite("fb :: bpp: "); serialWrite(buff_bpp); newline();

        }

        tag = (struct multiboot_tag*)((uint8_t*)tag + ((tag->size + 7) & ~7));
    }
}

void fbplot(int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= fb_width || y >= fb_height) {
        serialWrite("Error: fbplot out of bounds\n");
        return;
    }

    uint32_t offset = y * (fb_pitch / 4) + x;
    framebuffer[offset] = color;
}


void fbrect(int x, int y, int width, int height, uint32_t color) {
    for (int dy = 0; dy < height; dy++) {
        for (int dx = 0; dx < width; dx++) {
            fbplot(x + dx, y + dy, color);
        }
    }
}
