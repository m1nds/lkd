#ifndef PMM_HPP
#define PMM_HPP

#include <multiboot.h>
#include <bitmap.hpp>
#include <stdint.h>

namespace pmm {
    class PMM {
        public:
            static PMM& getInstance();
            void init(multiboot_info_t* mbd);

            void set_used(uint32_t regionAddr, uint32_t regionSize);
            void set_unused(uint32_t regionAddr, uint32_t regionSize);
            void* allocate_frame();
            void free_frame(void* addr);

            PMM(PMM const&) = delete;
            void operator=(PMM const&) = delete;

        private:
            PMM() {};

        public:
            uint32_t _nbFrames = 0;
            utils::Bitmap _bm = utils::Bitmap{};
    };
}

#endif /* ! PMM_HPP */
