#ifndef VIDEO_H
#define VIDEO_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "vesadrv.h"
#include "gdt.h"
#define VESA_BUFFER ((uint8_t*)0XA0000)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 200
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16
extern unsigned char font_bitmap[];
static unsigned char * font;

/* You can get this structure from GRUB's multiboot.h if needed
 * https://www.gnu.org/software/grub/manual/multiboot/html_node/multiboot_002eh.html
 */
struct multiboot_info;

/* Values made available by the linker script */
extern void *__realmode_lma_start;
extern void *__realmode_lma_end;
extern void *__realmode_vma_start;

/* Pointer to graphics memory.Mark as volatile since
 * video memory is memory mapped IO. Certain optimization
 * should not be performed. */
static volatile uint32_t * video_gfx_ptr;

void video_init();
void put_pixel(int x,int y,uint32_t color);
void realmode_setup (void);
void init_font();
void drawchar(unsigned char c ,int x,int y,int fgcolor,int bgcolor);
void drawstring(char* input,int Xray_start,int Yray_start,int fgcolor,int bgcolor);


#endif