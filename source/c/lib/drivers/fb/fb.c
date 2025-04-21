#include "fb.h"

framebuffer_t fb_inf;

void fbInit(void* multiboot_info) {
    struct multiboot_tag* mbtag;

    struct multiboot_tag* tag;

    for (tag = (struct multiboot_tag*)((uintptr_t)multiboot_info + 8);
         tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (struct multiboot_tag*)((uintptr_t)tag + ((tag->size + 7) & ~7))) {

        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            struct multiboot_tag_framebuffer* fb = (struct multiboot_tag_framebuffer*)tag;

            fb_inf.address = fb->common.framebuffer_addr;
            fb_inf.pitch = fb->common.framebuffer_pitch;
            fb_inf.width = fb->common.framebuffer_width;
            fb_inf.height = fb->common.framebuffer_height;
            fb_inf.bpp = fb->common.framebuffer_bpp;
            fb_inf.type = fb->common.framebuffer_type;

            if (fb_inf.type == MULTIBOOT_FRAMEBUFFER_TYPE_RGB) {

                fb_inf.rgb_dat.r_field_pos = fb->framebuffer_red_field_position;
                fb_inf.rgb_dat.r_mask_size = fb->framebuffer_red_mask_size;
                fb_inf.rgb_dat.g_field_pos = fb->framebuffer_green_field_position;
                fb_inf.rgb_dat.g_mask_size = fb->framebuffer_green_mask_size;
                fb_inf.rgb_dat.b_field_pos = fb->framebuffer_blue_field_position;
                fb_inf.rgb_dat.b_mask_size = fb->framebuffer_blue_mask_size;

            } else if (fb_inf.type == MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED) {

                fb_inf.indexed.ncolors = fb->framebuffer_palette_num_colors;
                fb_inf.indexed.palette = fb->framebuffer_palette;
            }

            break;
        }
    }

}

void putpixel(int x, int y, uint32_t color) {
    if (x < 0 || y < 0 || x >= (int)fb_inf.width || y >= (int)fb_inf.height) return;

    uint8_t* pixel_address = (uint8_t*)fb_inf.address + y * fb_inf.pitch + x * (fb_inf.bpp / 8);

    switch (fb_inf.bpp) {
        case 32:
            *(uint32_t*)pixel_address = color;
            break;
        case 24:
            pixel_address[0] = (color >> 0) & 0xFF;  // Blue
            pixel_address[1] = (color >> 8) & 0xFF;  // Green
            pixel_address[2] = (color >> 16) & 0xFF; // Red
            break;
        case 16: {
            // Convert 24-bit color to 16-bit 5:6:5
            uint16_t r = (color >> 19) & 0x1F;
            uint16_t g = (color >> 10) & 0x3F;
            uint16_t b = (color >> 3) & 0x1F;
            uint16_t pixel_color = (r << 11) | (g << 5) | b;
            *(uint16_t*)pixel_address = pixel_color;
            break;
        }
        default:
            // Unsupported format
            break;
    }
}