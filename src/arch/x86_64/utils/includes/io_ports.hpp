#pragma once

class IOPorts {
  public:
    static void outb(unsigned int port, unsigned char value);
    static unsigned char inb(unsigned int port);
};
