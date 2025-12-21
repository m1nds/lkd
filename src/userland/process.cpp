#include <process.hpp>
#include <paging.hpp>
#include <kmalloc.hpp>
#include <string.hpp>
#include <gdt.hpp>
#include <utils.hpp>
#include <interrupt_state.hpp>
#include <pmm.hpp>

namespace user {
    static Process* processes = nullptr;
    static Process* current = nullptr;
    bool Process::_init = false;

    void Process::create_process(Elf32* elf, vmm::Page* kernel_pd) {
        vmm::Page* pd = kernel_pd->copy();
        uint32_t stack_ptr = reinterpret_cast<uint32_t>(pmm::PMM::getInstance().allocate_frame());
        uint32_t kernel_ptr = reinterpret_cast<uint32_t>(pmm::PMM::getInstance().allocate_frame());

        pd->map_page(stack_ptr, 0xbfff0000, 0x7);
        pd->map_page(stack_ptr + 0x1000, 0xbfff1000, 0x7);

        pd->update_cr3();
        elf->load(*pd);
        kernel_pd->update_cr3();

        Process* new_process = reinterpret_cast<Process*>(kmalloc::calloc(1, sizeof(class Process)));
        new_process->regs.eip = reinterpret_cast<uint32_t>(elf->entry_point);
        new_process->regs.cs = 0x1B;
        new_process->regs.user_esp = 0xbfff1000;
        new_process->regs.user_ss = 0x23;
        new_process->regs.ebp = 0xbfff0000;
        new_process->regs.eflags = 0x202;

        new_process->page_directory = pd;
        new_process->kernel_stack_top = P2V(kernel_ptr + 0x1000);

        Process* last = processes;
        if (!last) {
            processes = new_process;
            current = new_process;
            return;
        }

        while (last->next != nullptr) {
            last = last->next;
        }

        last->next = new_process;
    }

    void Process::switch_process(struct interrupt_state* state) {

        if (!Process::_init || !current) {
            return;
        }

        memcpy(&current->regs, state, sizeof(interrupt_state));

        current = current->next ? current->next : processes;
        current->page_directory->update_cr3();

        memcpy(state, &current->regs, sizeof(interrupt_state));
    }

    void Process::enable_scheduling() {
        Process::_init = true;
    }
};
