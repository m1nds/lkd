#include <serial.hpp>
#include <interrupt_state.hpp>

namespace syscalls {

    int syscall_test() {
        serial::Serial s{};
        s.write_str("DUMMY SYSCALL] This is a dummy syscall.\n");
        return 0;
    }

    void* syscalls_buffer[] = {
        (void*) &syscall_test
    };

    void syscall_handler(struct interrupt_state* state) {
        void* syscall_ptr = syscalls_buffer[state->eax];
        int ret_value;

         asm volatile ("push %1;   \
                        push %2;   \
                        push %3;   \
                        push %4;   \
                        push %5;   \
                        call *%6;  \
                        pop %%ebx; \
                        pop %%ebx; \
                        pop %%ebx; \
                        pop %%ebx; \
                        pop %%ebx; \
                        " : "=a" (ret_value) :
                        "r" (state->edi), "r" (state->esi),
                        "r" (state->edx), "r" (state->ecx), 
                        "r" (state->ebx), "r" (syscall_ptr));

        state->eax = ret_value;
    }
};
