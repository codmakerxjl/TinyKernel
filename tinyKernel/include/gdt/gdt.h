#ifndef GDT_H
#define GDT_H

#include <stdint.h>
#include <stdbool.h>
#define CODE32SEL 0x08
#define DATA32SEL 0x10
#define CODE16SEL 0x18
#define DATA16SEL 0x20
#define NUM_GDT_ENTRIES 5
typedef struct
{
        unsigned short limit_low;
        unsigned short base_low;
        unsigned char base_middle;
        unsigned char access;
        unsigned char flags;
        unsigned char base_high;
} __attribute__((packed)) gdt_desc_t;


/* GDT descriptor table */
static gdt_desc_t gdt[NUM_GDT_ENTRIES];


typedef struct {
    uint16_t limit;
    gdt_desc_t *gdt;
} __attribute__((packed)) gdtr_t;

extern void gdt_set_gate(gdt_desc_t gdt[], const int num, const uint32_t base,
                         const uint32_t limit, const uint8_t access,
                         const uint8_t flags);

static inline void gdt_load(gdtr_t * const gdtr, const uint16_t codesel,
                            const uint16_t datasel, const bool flush)
{    
    /* load the GDT register */
    __asm__ __volatile__ ("lgdt %[gdtr]"
                          :
                          : [gdtr]"m"(*gdtr),
                          /* Dummy constraint to ensure what gdtr->gdt points at is fully
                           * realized into memory before we issue LGDT instruction */
                            "m"(*(const gdt_desc_t (*)[]) gdtr->gdt));

    /* This flushes the selector registers to ensure the new
     * descriptors are used. */
    if (flush) {
        /* The indirect absolute jump is because we can't
         * assume that codesel is an immediate value
         * as it may be passed in a register. We build a
         * far pointer in memory and indirectly jump through
         * that pointer. This explicitly sets CS selector */
        __asm__  __volatile__ (
                 "pushl %[codesel]\n\t"
                 "pushl $1f\n\t"
                 "ljmpl *(%%esp)\n"
                 "1:\n\t"
                 "add $8, %%esp\n\t"
                 "mov %[datasel], %%ds\n\t"
                 "mov %[datasel], %%es\n\t"
                 "mov %[datasel], %%ss\n\t"
                 "mov %[datasel], %%fs\n\t"
                 "mov %[datasel], %%gs"
                 : /* No outputs */
                 : [datasel]"r"(datasel),
                   [codesel]"g"((uint32_t)codesel));
    }
    return;
}


void gdt_setup (gdt_desc_t gdt[], const int numdesc);
#endif