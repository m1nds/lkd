#pragma once

#include <stddef.h>
#include <stdint.h>

constexpr size_t STACK_SIZE = 256;

using ssize_t = ptrdiff_t;

class UARTInterface {
  public:
    virtual void init() = 0;
    virtual int is_transmit_empty() = 0;
    virtual void write_char(unsigned char ch) = 0;
};

template <typename T = UARTInterface> class UART {
  public:
    static ssize_t write_str(const char* buf);
    static ssize_t write(const char* buf, size_t count);
    static void kprintf(const char* format, ...);

  private:
    static void print_dec(uint32_t value);
    static void print_hex(uint64_t value, uint32_t bits);

    static T backend;

    static bool init;
    static char stack[STACK_SIZE];
    static size_t stack_idx;
};

class ArchUART;

using UARTG = UART<ArchUART>;
