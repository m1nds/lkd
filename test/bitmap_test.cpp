#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

#include <bitmap.hpp>

TEST(BitmapTests, SimpleGetBit) {
    uint8_t data;
    size_t size = 8;

    uint8_t* ptr = &data;

    utils::Bitmap bm{ptr, size};
    data = 0b100000;

    EXPECT_EQ(bm.get_bit(5), true);
}

TEST(BitmapTests, GetBitOutOfBounds) {
    uint32_t data;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};
    data = 0;

    EXPECT_EQ(bm.get_bit(32), false);
}

TEST(BitmapTests, ClearBitReturnValueFalse) {
    uint32_t data;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};
    data = 0x0FFFFFFF;

    EXPECT_EQ(bm.clear_bit(32), false);
}

TEST(BitmapTests, ClearBitReturnValueTrue) {
    uint32_t data;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};
    data = 0x0FFFFFFF;

    EXPECT_EQ(bm.clear_bit(31), true);
}

TEST(BitmapTests, SetBitReturnValueFalse) {
    uint32_t data;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};
    data = 0x0FFFFFFF;

    EXPECT_EQ(bm.set_bit(32), false);
}

TEST(BitmapTests, SetBitReturnValueTrue) {
    uint32_t data;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};
    data = 0x0FFFFFFF;

    EXPECT_EQ(bm.set_bit(31), true);
}

TEST(BitmapTests, ClearBitSimple) {
    uint32_t data;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};
    data = 0xFFFFFFFF;

    EXPECT_EQ(bm.clear_bit(3), true);
    EXPECT_EQ(bm.clear_bit(18), true);

    for (uint32_t i = 0; i < 32; i++) {
        if (i == 3 || i == 18) {
            EXPECT_EQ(bm.get_bit(i), false);
        } else {
            EXPECT_EQ(bm.get_bit(i), true);
        }
    }
}

TEST(BitmapTests, ClearArea) {
    uint32_t data;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};
    data = 0xFFFFFFFF;

    for (uint32_t i = 16; i < 32; i++) {
        bm.clear_bit(i);
    }

    for (uint32_t i = 0; i < 32; i++) {
        if (i < 16) {
            EXPECT_EQ(bm.get_bit(i), true);
        } else {
            EXPECT_EQ(bm.get_bit(i), false);
        }
    }
}

TEST(BitmapTests, InvariantBit) {
    uint32_t data;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};
    data = 0b100000000;

    bm.clear_bit(9);
    bm.set_bit(9);

    EXPECT_EQ(bm.get_bit(9), true);
}
