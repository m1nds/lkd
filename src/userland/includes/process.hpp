#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <interrupt_state.hpp>
#include <loader.hpp>
#include <paging.hpp>

namespace user {

    class Process {
        public:
            static void create_process(Elf32* elf, vmm::Page* kernel_pd);
            static void switch_process(struct interrupt_state* state);
            static void enable_scheduling();
            vmm::Page* page_directory;

        private:
            struct interrupt_state regs;
            void* entrypoint;
            Process* next;
            uint32_t kernel_stack_top;
            static bool _init;
    };
};

#endif
