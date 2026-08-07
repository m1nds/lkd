#include <stdarg.h>

#include <uart.hpp>
#include <uart_method.hpp>

template <typename T> T UART<T>::backend{};
template <typename T> bool UART<T>::init = false;
template <typename T> char UART<T>::stack[STACK_SIZE] = {};
template <typename T> size_t UART<T>::stack_idx = 0;

template <typename T> ssize_t UART<T>::write_str(const char* buf) {
    if (!init) {
        backend.init();
        init = true;
    }

    ssize_t i = 0;
    while (buf[i]) {
        backend.write_char(buf[i++]);
    }

    return i;
}

template <typename T> ssize_t UART<T>::write(const char* buf, size_t count) {
    if (!init) {
        backend.init();
        init = true;
    }

    for (size_t i = 0; i < count; i++) {
        backend.write_char(buf[i]);
    }

    return count;
}

template <typename T> void UART<T>::print_hex(uint64_t value, uint32_t bits) {
    uint32_t digits = 0;
    static const char hex[] = "0123456789ABCDEF";

    while (value && stack_idx < STACK_SIZE) {
        stack[stack_idx++] = value % 16;
        digits++;
        value /= 16;
    }

    write_str("0x");

    uint32_t len = bits / 4;
    for (uint32_t i = digits; i < len; i++) {
        backend.write_char('0');
    }

    for (uint32_t i = 0; i < digits; i++) {
        backend.write_char(hex[(unsigned char)stack[--stack_idx]]);
    }
}

template <typename T> void UART<T>::print_dec(uint32_t value) {
    uint32_t digits = 0;

    if (value == 0) {
        backend.write_char('0');
        return;
    }

    while (value && stack_idx < STACK_SIZE) {
        stack[stack_idx++] = value % 10;
        digits++;
        value /= 10;
    }

    for (uint32_t i = 0; i < digits; i++) {
        backend.write_char('0' + stack[--stack_idx]);
    }
}

template <typename T> void UART<T>::kprintf(const char* format, ...) {
    if (!init) {
        backend.init();
        init = true;
    }

    va_list args;
    va_start(args, format);

    size_t i = 0;
    while (format[i]) {
        if (format[i] != '%') {
            backend.write_char(format[i]);
        } else if (format[i + 1] == '%') {
            backend.write_char('%');
            i++;
        } else if (format[i + 1] == 'c') {
            backend.write_char((unsigned char)va_arg(args, int));
            i++;
        } else if (format[i + 1] == 's') {
            write_str(va_arg(args, const char*));
            i++;
        } else if (format[i + 1] == 'd') {
            print_dec(va_arg(args, uint32_t));
            i++;
        } else if (format[i + 1] == 'x') {
            print_hex(va_arg(args, uint32_t), 32);
            i++;
        } else if (format[i + 1] == 'X') {
            print_hex(va_arg(args, uint64_t), 64);
            i++;
        } else {
            write_str("[WARNING] Invalid printing format!\n");
        }
        i++;
    }

    va_end(args);
}

template class UART<ArchUART>;
