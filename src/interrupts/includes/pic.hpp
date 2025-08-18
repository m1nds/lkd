#ifndef PIC_HPP
#define PIC_HPP

#include <stdint.h>

namespace pic {

    class PIC {
        public:
            PIC();
            static void pic_send_eoi(uint32_t irq);

        private:
            static bool _init;
    };

};

#endif
