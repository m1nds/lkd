#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <stdint.h>

namespace keyboard {

    constexpr uint32_t KEYBOARD_DATA_PORT = 0x60;
    constexpr uint32_t KEYBOARD_QUEUE_SIZE = 256;
    constexpr uint32_t COLUMNS = 4;

    constexpr uint32_t LSHIFT = 0x21;
    constexpr uint32_t RSHIFT = 0x36;
    constexpr uint32_t CTRL = 0x1D;
    constexpr uint32_t ALT = 0x38;

    class Keyboard {
        public:
            Keyboard();
            static void keyboard_interrupt_handler();
            static unsigned char keyboard_getchar();

        private:
            static bool process_special(unsigned char scancode);
            static void keyboard_flush();

            static bool _init;
            static uint32_t _idx;
            static uint32_t _read;
            static uint32_t _column;
            static unsigned char _queue[KEYBOARD_QUEUE_SIZE];
    };
};


#endif /* TIMER_HPP */
