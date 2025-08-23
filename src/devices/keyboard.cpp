#include <keyboard.hpp>
#include <io.hpp>
#include <string.h>
#include <serial.hpp>

namespace keyboard {

    static unsigned char scancode_columns[128][COLUMNS] = {
        {0xFF,0xFF,0xFF,0xFF},
        {0x1B,0x1B,0x1B,0x1B},
        {'1','!','1','1'},
        {'2','@','2','2'},
        {'3','#','3','3'},
        {'4','$','4','4'},
        {'5','%','5','5'},
        {'6','^','6','6'},
        {'7','&','7','7'},
        {'8','*','8','8'},
        {'9','(','9','9'},
        {'0',')','0','0'},
        {'-','_','-','-'},
        {'=','+','=','='},
        {0x08,0x08,0x7F,0x08},
        {0x09,0x09,0x09,0x09},
        {'q','Q','q','q'},
        {'w','W','w','w'},
        {'e','E','e','e'},
        {'r','R','r','r'},
        {'t','T','t','t'},
        {'y','Y','y','y'},
        {'u','U','u','u'},
        {'i','I','i','i'},
        {'o','O','o','o'},
        {'p','P','p','p'},
        {'[','{','[','['},
        {']','}',']',']'},
        {0x0A,0x0A,0x0A,0x0A},
        {0xFF,0xFF,0xFF,0xFF},
        {'a','A','a','a'},
        {'s','S','s','s'},
        {'d','D','d','d'},
        {'f','F','f','f'},
        {'g','G','g','g'},
        {'h','H','h','h'},
        {'j','J','j','j'},
        {'k','K','k','k'},
        {'l','L','l','l'},
        {';',':',';',';'},
        {0x27,0x22,0x27,0x27},
        {'`','~','`','`'},
        {0xFF,0xFF,0xFF,0xFF},
        {'\\','|','\\','\\'},
        {'z','Z','z','z'},
        {'x','X','x','x'},
        {'c','C','c','c'},
        {'v','V','v','v'},
        {'b','B','b','b'},
        {'n','N','n','n'},
        {'m','M','m','m'},
        {',','<',',',','},
        {'.','>','.','.'},
        {'/','?','/','/'},
        {0xFF,0xFF,0xFF,0xFF},
        {'*','*','*','*'},
        {0xFF,0xFF,0xFF,0xFF},
        {' ',' ',' ',' '},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {'7','7','7','7'},
        {'8','8','8','8'},
        {'9','9','9','9'},
        {'-','-','-','-'},
        {'4','4','4','4'},
        {'5','5','5','5'},
        {'6','6','6','6'},
        {'+','+','+','+'},
        {'1','1','1','1'},
        {'2','2','2','2'},
        {'3','3','3','3'},
        {'0','0','0','0'},
        {'.','.','.','.'},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF},
        {0xFF,0xFF,0xFF,0xFF}
    };


    static uint8_t special_keys[256] = {
        [LSHIFT] = 1,
        [RSHIFT] = 1,
        [CTRL] = 2,
        [ALT] = 3,
    };

    bool Keyboard::_init = false;
    uint32_t Keyboard::_idx = 0;
    uint32_t Keyboard::_read = 0;
    uint32_t Keyboard::_column = 0;

    unsigned char Keyboard::_queue[KEYBOARD_QUEUE_SIZE];

    Keyboard::Keyboard() {
        if (Keyboard::_init) {
            return;
        }

        Keyboard::_init = true;
        Keyboard::_idx = 0;
        Keyboard::_read = 0;
        Keyboard::_column = 0;
        memset(Keyboard::_queue, 0, KEYBOARD_QUEUE_SIZE);
    }

    bool Keyboard::process_special(unsigned char scancode) {
        bool clear = false;
        if (scancode > 0x80) {
            scancode -= 0x80;
            clear = true;
        }

        uint32_t special = special_keys[scancode];
        if (special != 0 && clear) {
            Keyboard::_column = 0;
            return true;
        } else if (special != 0) {
            Keyboard::_column = special;
            return true;
        }

        return false;
    }

    void Keyboard::keyboard_flush() {
        serial::Serial s{};
        s.kprintf("[KEYBOARD] Queue has been flushed!\n");
        Keyboard::_idx = 0;
        Keyboard::_read = 0;
    }

    void Keyboard::keyboard_interrupt_handler() {
        if (!Keyboard::_init) {
            return;
        }

        unsigned char scancode = io::inb(KEYBOARD_DATA_PORT);
        bool special = process_special(scancode);
        if (special) {
            return;
        }

        if (scancode < 0x80) {

            if (Keyboard::_idx >= KEYBOARD_QUEUE_SIZE) {
                keyboard_flush();
            }

            Keyboard::_queue[Keyboard::_idx++] = scancode_columns[scancode][Keyboard::_column];
        }
    }

    unsigned char Keyboard::getchar() {
        while (Keyboard::_read == Keyboard::_idx);
        return Keyboard::_queue[Keyboard::_read++];
    }
};
