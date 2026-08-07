#pragma once

#include <uart.hpp>

class ArchUART : public UARTInterface {
  public:
    void init() override;
    int is_transmit_empty() override;
    void write_char(unsigned char ch) override;
};
