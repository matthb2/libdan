cmake_minimum_required(VERSION 2.6)
file(GLOB DAN_C_FILES dan_*.c)
add_library(dan ${DAN_C_FILES})
add_executable(aa test/aa.c)
target_link_libraries(aa dan)