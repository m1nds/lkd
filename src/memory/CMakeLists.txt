target_include_directories(lkd PRIVATE includes)
target_include_directories(lkd PRIVATE ${CMAKE_SOURCE_DIR}/src/uart/includes)

target_sources(lkd PUBLIC memory_map.cpp)
