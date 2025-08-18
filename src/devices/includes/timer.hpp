#ifndef TIMER_HPP
#define TIMER_HPP

#include <stdint.h>

namespace timer {
    class PIT {
        public:
            PIT(uint32_t freq_hz);
            static void pit_interrupt_handler();
        private:
            static bool _init;
            static uint32_t _ticks;
    };
};


#endif /* TIMER_HPP */
