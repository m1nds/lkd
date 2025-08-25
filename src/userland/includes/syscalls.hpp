#ifndef SYSCALLS_HPP
#define SYSCALLS_HPP

#include <interrupt_state.hpp>

namespace syscalls {
    void syscall_handler(struct interrupt_state* state);
};

#endif /* SYSCALLS_HPP */
