#ifndef TAR_HPP
#define TAR_HPP

namespace utils {

    class TarEntry {
        public:
            char filename[100];
            char mode[8];
            char uid[8];
            char gid[8];
            char size[12];
            char mtime[12];
            char chksum[8];
            char typeflag[1];
    };

    class Tar {
        public:
            utils::TarEntry header;
            char* data;
            utils::Tar* next;
    };

    utils::Tar* parse_tar(unsigned int address);
};

#endif /* TAR_HPP */
