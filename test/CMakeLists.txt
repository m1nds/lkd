set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(bitmap_test "")

target_sources(bitmap_test PUBLIC
    ../src/utils/bitmap.cpp
    bitmap_test.cpp
)

target_include_directories(bitmap_test PRIVATE "../src/includes")
target_link_libraries(bitmap_test PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(bitmap_test)
