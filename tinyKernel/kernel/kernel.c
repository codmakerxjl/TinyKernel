#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "video.h"
#include "string.h"

#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

#if !defined(__i386__)
#error "need to be compiled with a ix86-elf compiler"
#endif








void kernel_early_main(int argc,char**argv){
    video_init();
    uint8_t white=0xEE;
    uint8_t black =0x0;

    //drawchar('A',0,0,white,blue);
    drawstring("222221",0,0,white,black);
    strlen("2222");
        
}

int kmain()
{
   

    return 0;
}