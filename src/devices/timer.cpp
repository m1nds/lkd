#include <timer.hpp>
#include <io.hpp>
#include <serial.hpp>
#include <stdint.h>

namespace timer {

    bool PIT::_init = false;
    uint32_t PIT::_ticks = 0;

    constexpr uint32_t PIT_COMMAND = 0x43;
    constexpr uint32_t PIT_REPEAT_MODE = 0x36;

    constexpr uint32_t PIT_DATA_CH0 = 0x40;
    constexpr uint32_t PIT_DATA_CH1 = 0x41;
    constexpr uint32_t PIT_DATA_CH2 = 0x42;

    void PIT::pit_interrupt_handler() {
        PIT::_ticks += 1;
    }

    PIT::PIT(uint32_t freq_hz) {
        if (PIT::_init) {
            return;
        }

        PIT::_ticks = 0;
        PIT::_init = true;

        uint32_t divisor = 1193180 / freq_hz;

        io::outb(PIT_COMMAND, PIT_REPEAT_MODE);
        io::outb(PIT_DATA_CH0, divisor & 0xFF);
        io::outb(PIT_DATA_CH0, (divisor >> 8) & 0xFF);
    }

};

