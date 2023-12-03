add_library(input)
target_sources(input PUBLIC input.cpp)

add_executable(day03 day03.cpp)
target_link_libraries(day03 input)

