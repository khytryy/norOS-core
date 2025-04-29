#include "fb.h"

uint32_t* framebuffer = 0;
uint32_t fb_width = 0;
uint32_t fb_height = 0;
uint8_t fb_bpp = 0;
uint32_t fb_pitch = 0;

void fbInit(void* pointer) {
    struct multiboot_tag* tag = (struct multiboot_tag*)((uint8_t*)pointer + 8);

    while (tag->type != 0) {
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            struct multiboot_tag_framebuffer_common* fbtag =
                (struct multiboot_tag_framebuffer_common*)tag;

            framebuffer = (uint32_t*)(uint64_t)fbtag->framebuffer_addr;
            fb_width = fbtag->framebuffer_width;
            fb_height = fbtag->framebuffer_height;
            fb_bpp = fbtag->framebuffer_bpp;
            fb_pitch = fbtag->framebuffer_pitch;

            // logging
            char buff_addr[32], buff_pitch[32], buff_width[32], buff_height[32], buff_bpp[32];
            itoa_hex((uint64_t)framebuffer, buff_addr);
            itoa_dec(fbtag->framebuffer_pitch, buff_pitch);
            itoa_dec(fb_width, buff_width);
            itoa_dec(fb_height, buff_height);
            itoa_dec(fb_bpp, buff_bpp);

            serialWrite(KERNEL_ICON_INFO);
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

void fbplot(struct multiboot_tag_framebuffer_common* fbtag, int x, int y, uint32_t color) {
    if (x >= fbtag->framebuffer_width || y >= fbtag->framebuffer_height) {
        serialWrite("Out of bounds\n");
        return;
    }

    // Calculate the offset (y * pitch + x * bpp)
    uint32_t offset = y * fbtag->framebuffer_pitch + x * 4;

    // Debug print to verify the offset and color
    char hex_offset[9];
    char hex_color[9];
    itoa_hex(offset, hex_offset);  // Convert offset to hex
    itoa_hex(color, hex_color);    // Convert color to hex

    serialWrite("Plotting at offset: ");
    serialWrite(hex_offset);
    serialWrite(" color: ");
    serialWrite(hex_color);
    newline();

    // Set the pixel color
    uint32_t* pixel = (uint32_t*)((uint8_t*)fbtag->framebuffer_addr + offset);
    *pixel = color;
}