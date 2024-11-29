#include "gdt.h"
/* GDT descriptor table */


/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(gdt_desc_t gdt[], const int num, const uint32_t base,
                  const uint32_t limit, const uint8_t access,
                  const uint8_t flags)
{
        /* Setup the descriptor base access */
        gdt[num].base_low = (base & 0xFFFF);
        gdt[num].base_middle = (base >> 16) & 0xFF;
        gdt[num].base_high = (base >> 24) & 0xFF;

        /* Setup the descriptor limits */
        gdt[num].limit_low = (limit & 0xFFFF);
        gdt[num].flags = ((limit >> 16) & 0x0F);

        /* Finally, set up the flags and access byte */
        gdt[num].flags |= (flags << 4);
        gdt[num].access = access;
}

void gdt_setup (gdt_desc_t gdt[], const int numdesc)
{
    gdtr_t gdtr = { sizeof(gdt_desc_t)*numdesc-1, gdt };

    /* Null descriptor */
    gdt_set_gate(gdt, 0, 0x00000000, 0x00000000, 0x00, 0x0);
    /* 32-bit Code descriptor, flat 4gb */
    gdt_set_gate(gdt, 1, 0x00000000, 0xffffffff, 0x9A, 0xC);
    /* 32-bit Data descriptor, flat 4gb */
    gdt_set_gate(gdt, 2, 0x00000000, 0xffffffff, 0x92, 0xC);
    /* 16-bit Code descriptor, limit 0xffff bytes */
    gdt_set_gate(gdt, 3, 0x00000000, 0x0000ffff, 0x9A, 0x0);
    /* 16-bit Data descriptor, limit 0xffffffff bytes */
    gdt_set_gate(gdt, 4, 0x00000000, 0xffffffff, 0x92, 0x8);

    /* Load global decriptor table, and flush the selectors */
    gdt_load(&gdtr, CODE32SEL, DATA32SEL, true);
}
