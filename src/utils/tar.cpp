#include <tar.hpp>
#include <kmalloc.hpp>
#include <serial.hpp>
#include <string.h>

namespace utils {

    static unsigned int getsize(const char *in) {
        unsigned int size = 0;
        unsigned int j;
        unsigned int count = 1;

        for (j = 11; j > 0; j--, count *= 8)
            size += ((in[j - 1] - '0') * count);

        return size;

    }

    utils::Tar* parse_tar(unsigned int address)
    {
        serial::Serial s{};
        unsigned int i = 0;
        utils::Tar* out = nullptr;

        while (true)
        {
            utils::TarEntry* header = reinterpret_cast<utils::TarEntry*>(address);
            if (header->filename[i] == '\0')
                break;

            unsigned int size = getsize(header->size);

            utils::Tar* new_tar = reinterpret_cast<utils::Tar*>(kmalloc::malloc(sizeof(utils::Tar)));
            memcpy(&new_tar->header, header, sizeof(utils::Tar));

            new_tar->data = reinterpret_cast<char*>(kmalloc::malloc(size));
            memcpy(new_tar->data, reinterpret_cast<char*>(header) + 512, size);

            new_tar->next = nullptr;

            if (out == nullptr) {
                out = new_tar;
                out->next = nullptr;
            } else {
                new_tar->next = out;
                out = new_tar;
            }

            address += ((size / 512) + 1) * 512;

            if (size % 512)
                address += 512;

            i++;
        }

        return out;
    }
};
