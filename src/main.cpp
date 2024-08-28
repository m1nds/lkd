#include <io.hpp>
#include <serial.hpp>
#include <gdt.hpp>

extern "C" void kmain() {
    serial::Serial s{};
    s.write("Testing the serial!\n", 21);

    gdt_init();

    s.write("GDT was setup!\n", 15);
}
