#include <mmio.hpp>
#include <uart_method.hpp>

constexpr int PL011_BASE = 0x09000000;
constexpr int UART0_DR = 0x0;
constexpr int UART0_FR = 0x18;

void ArchUART::init() { return; }

int ArchUART::is_transmit_empty() {
    return MMIO::mmio_read(PL011_BASE + UART0_FR) & (1 << 5);
}

void ArchUART::write_char(unsigned char ch) {
    while (this->is_transmit_empty())
        ;
    MMIO::mmio_write(PL011_BASE + UART0_DR, ch);
}
