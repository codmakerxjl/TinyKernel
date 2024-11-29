#ifndef VESADRV_H
#define VESADRV_H

#include <stdint.h>

extern struct mode_info_t * do_vbe (const uint8_t video_mode);

struct mode_info_t {
    uint8_t status; /* Return value from Int 10/ax=4f01 */

    /* Rest of structure from OSDev Wiki
       http://wiki.osdev.org/VESA_Video_Modes#VESA_Functions
    */
    uint16_t attributes;
    uint8_t winA,winB;
    uint16_t granularity;
    uint16_t winsize;
    uint16_t segmentA, segmentB;

    /* Real mode FAR Pointer.  Physical address
     * computed as (segment<<4)+offset
     */
    uint16_t realFctPtr_offset; /* FAR Pointer offset */
    uint16_t realFctPtr_segment;/* FAR Pointer segment */

    uint16_t pitch; /* bytes per scanline */

    uint16_t Xres, Yres;
    uint8_t Wchar, Ychar, planes, bpp, banks;
    uint8_t memory_model, bank_size, image_pages;
    uint8_t reserved0;

    uint8_t red_mask, red_position;
    uint8_t green_mask, green_position;
    uint8_t blue_mask, blue_position;
    uint8_t rsv_mask, rsv_position;
    uint8_t directcolor_attributes;

    volatile void * physbase;  /* LFB (Linear Framebuffer) address */
    uint32_t reserved1;
    uint16_t reserved2;
} __attribute__((packed));

#endif