#include <string.hpp>

#include <stdint.h>
#include <stddef.h>

extern "C" void memcpy(void *dst, const void *src, size_t size) {
    uint8_t *c_dst = (uint8_t*) dst;
    uint8_t *c_src = (uint8_t*) src;

    for (size_t i = 0; i < size; i++) {
        c_dst[i] = c_src[i];
    }
}

extern "C" void memset(void *dst, uint8_t val, size_t size) {
    uint8_t *c_dst = (uint8_t*) dst;

    for (size_t i = 0; i < size; i++) {
        c_dst[i] = val;
    }
}

extern "C" void *memmove(void *dst, const void *src, size_t size)
{
   unsigned char *c_dst = (unsigned char *) dst;
   const unsigned char *c_src = (const unsigned char *) src;

   if(dst < src) {
      for(size_t i = 0; i < size; i++) {
         c_dst[i] = c_src[i];
      }
   } else {
      for(size_t i = size; i != 0; i--) {
         c_dst[i - 1] = c_src[i - 1];
      }
   }

   return dst;
}

extern "C" int strcmp(const char *one, const char *two)
{
   size_t i = 0;
   while(one[i] && one[i] == two[i]) {
      i++;
   }

   return one[i] - two[i];
}


extern "C" int memcmp(const void *one, const void *two, size_t size)
{
   const unsigned char* c_one = reinterpret_cast<const unsigned char*>(one);
   const unsigned char* c_two = reinterpret_cast<const unsigned char*>(two);

   for (size_t i = 0; i < size; i++) {
      if (c_one[i] < c_two[i]) {
         return -1;

      }

      if (c_two[i] < c_one[i]) {
         return 1;
      }
   }

   return 0;
}
