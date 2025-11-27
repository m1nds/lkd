#include <process.hpp>
#include <paging.hpp>
#include <kmalloc.hpp>
#include <string.hpp>
#include <interrupt_state.hpp>
#include <pmm.hpp>

namespace user {
    static Process* processes = nullptr;
    static Process* current = nullptr;
    bool Process::_init = false;

    void Process::create_process(Elf32* elf, vmm::Page* kernel_pd) {
        vmm::Page* pd = kernel_pd->copy();
        uint32_t stack_ptr = reinterpret_cast<uint32_t>(pmm::PMM::getInstance().allocate_frame());
        uint32_t stack_ptr_after = reinterpret_cast<uint32_t>(pmm::PMM::getInstance().allocate_frame());

        pd->map_page(stack_ptr, 0xbfffe000, 0x7);
        pd->map_page(stack_ptr_after, 0xbffff000, 0x7);

        pd->update_cr3();
        elf->load(*pd);
        kernel_pd->update_cr3();

        Process* new_process = reinterpret_cast<Process*>(kmalloc::calloc(1, sizeof(class Process)));
        new_process->regs.eip = reinterpret_cast<uint32_t>(elf->entry_point);
        new_process->regs.user_esp = 0xbffff000;
        new_process->regs.ebp = 0xbfffe000;
        new_process->page_directory = pd;

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

        if (!Process::_init) {
            return;
        }

        memcpy(&current->regs, state, sizeof(struct interrupt_state));

        if (current->next != nullptr) {
            current = current->next;
        } else {
            current = processes;
        }

        memcpy(state, &current->regs, sizeof(struct interrupt_state));
    }

    void Process::enable_scheduling() {
        Process::_init = true;
    }
};
