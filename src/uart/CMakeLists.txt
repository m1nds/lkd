target_include_directories(lkd PRIVATE includes)
target_include_directories(lkd PRIVATE ${CMAKE_SOURCE_DIR}/src/arch/${ARCH}/uart/includes)
target_sources(lkd PUBLIC uart.cpp)
