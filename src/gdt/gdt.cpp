#include <stdint.h>
#include <string.h>

#include <gdt.hpp>
#include <serial.hpp>

#define GDT_SEGMENT_DESCRIPTOR(Base, Limit, Access, Flags) \
    .limit_low = (uint16_t)((Limit) & 0xFFFF), \
    .base_low = (uint16_t)(((uint32_t)Base) & 0xFFFF), \
    .base_mid = (uint8_t)((((uint32_t)Base) >> 16) & 0xFF), \
    .access = ((uint8_t)(Access)), \
    .flags = (uint8_t)(((((Flags)) & 0xF) << 4) | (((Limit) >> 16) & 0xF)), \
    .base_high = (uint8_t)((((uint32_t)Base) >> 24) & 0xFF)

extern uint32_t stack_top;
extern uint32_t stack_bottom;

namespace gdt {

    constexpr int GDT_ENTRIES = 6;

    extern "C" void gdt_load(void);
    extern "C" void tss_load(void);

    extern "C" struct tss_entry tss = {};

    extern "C" {
        struct {
            uint16_t limit;
            void *base;
        } __attribute__((packed)) gdtr;
    }

    extern "C" struct {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_mid;
        uint8_t access;
        uint8_t flags;
        uint8_t base_high;
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
        },
        {
            // TSS Descriptor (Address depends on runtime)
        }
    };

    void gdt_init(void) {
        serial::Serial s{};
        memset(&tss, 0, sizeof(struct tss_entry));

        // TSS Descriptor initialization
        gdt[5] = { GDT_SEGMENT_DESCRIPTOR(&tss, sizeof(tss) - 1, 0x89, 0x00) };

        s.kprintf("[GDT] Entry: GDT address : %x\n", &gdt);

        gdtr.limit = sizeof(gdt) - 1;
        gdtr.base = &gdt;

        tss.cs = 0x0B;
        tss.ss = 0x13;
        tss.es = 0x13;
        tss.ds = 0x13;
        tss.fs = 0x13;
        tss.gs = 0x13;

        tss.ss0 = 0x10;
        tss.esp0 = (uint32_t) &stack_top;

        uint64_t* gdt_value_ptr = (uint64_t*) &gdt;
        s.kprintf("[GDT] Entry: NULL Descriptor : %X\n", gdt_value_ptr[0]);
        s.kprintf("[GDT] Entry: Kernel Code Descriptor : %X\n", gdt_value_ptr[1]);
        s.kprintf("[GDT] Entry: Kernel Data Descriptor : %X\n", gdt_value_ptr[2]);
        s.kprintf("[GDT] Entry: User Code Descriptor : %X\n", gdt_value_ptr[3]);
        s.kprintf("[GDT] Entry: User Data Descriptor : %X\n", gdt_value_ptr[4]);
        s.kprintf("[GDT] Entry: TSS Descriptor : %X\n", gdt_value_ptr[5]);

        gdt_load();
        tss_load();
    }
}
