#include <io.hpp>
#include <serial.hpp>

extern "C" void kmain() {
    serial::Serial s{};
    s.write("Testing the serial!\n", 21);
}
