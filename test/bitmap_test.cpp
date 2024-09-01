#include <gtest/gtest.h>
#include <stdint.h>
#include <stddef.h>

#include <utils/bitmap.hpp>

TEST(BitmapTests, SimpleGetBit) {
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

    EXPECT_EQ(bm.get_bit(5), true);
}

TEST(BitmapTests, GetBitOutOfBounds) {
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

    EXPECT_EQ(bm.get_bit(32), false);
}

TEST(BitmapTests, ClearBitReturnValueFalse) {
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

    EXPECT_EQ(bm.clear_bit(32), false);
}

TEST(BitmapTests, ClearBitReturnValueTrue) {
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

    EXPECT_EQ(bm.clear_bit(31), true);
}

TEST(BitmapTests, SetBitReturnValueFalse) {
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

    EXPECT_EQ(bm.set_bit(32), false);
}

TEST(BitmapTests, SetBitReturnValueTrue) {
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

    EXPECT_EQ(bm.set_bit(31), true);
}

TEST(BitmapTests, ClearBitSimple) {
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

    bm.clear_bit(3);
    bm.clear_bit(18);

    for (uint32_t i = 0; i < 32; i++) {
        if (i != 3 && i != 18) {
            EXPECT_EQ(bm.get_bit(i), true);
        } else {
            EXPECT_EQ(bm.get_bit(i), false);
        }
    }
}

TEST(BitmapTests, ClearArea) {
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

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
    uint32_t data = 0;
    size_t size = 32;

    uint8_t* ptr = (uint8_t*) &data;

    utils::Bitmap bm{ptr, size};

    bm.clear_bit(15);
    bm.set_bit(15);

    EXPECT_EQ(bm.get_bit(15), true);
}
