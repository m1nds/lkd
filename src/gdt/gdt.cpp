#include <stdint.h>

#define GDT_ENTRIES 5

#define GDT_SEGMENT_DESCRIPTOR(Base, Limit, Access, Flags) \
    .limit_low = ((Limit) & 0xFFFF), \
    .base_low = ((Base) & 0xFFFF), \
    .base_mid = (((Base) >> 16) & 0xFF), \
    .access = ((uint8_t) (Access)), \
    .flags = (((((uint8_t) (Flags)) & 0xF) << 4) | (((Limit) >> 16) & 0xF)), \
    .present = ((Base) >> 24)

static struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t flags;
    uint8_t present;
} __attribute__((packed)) gdt[GDT_ENTRIES] = {
    {
        // Null Segment Descriptor
        GDT_SEGMENT_DESCRIPTOR(0, 0, 0, 0)
    },
    {
        // Kernel Code Segment Descriptor
        GDT_SEGMENT_DESCRIPTOR(0, 0xFFFFF, 0x9A, 0xC)
    },
    {
        // Kernel Data Segment Descriptor
        GDT_SEGMENT_DESCRIPTOR(0, 0xFFFFF, 0x92, 0xC)
    },
    {
        // Userland Code Segment Descriptor
        GDT_SEGMENT_DESCRIPTOR(0, 0xFFFFF, 0xFA, 0xC)
    },
    {
        // Userland Data Segment Descriptor
        GDT_SEGMENT_DESCRIPTOR(0, 0xFFFFF, 0xF2, 0xC)
    }
};

extern "C" {
    struct {
        uint16_t limit;
        void *base;
    } __attribute__((packed)) gdtr;
}

extern "C" void gdt_load(void);

void gdt_init(void) {
    gdtr.limit = sizeof(gdt) - 1;
    gdtr.base = (uint32_t*) gdt;

    gdt_load();
}
