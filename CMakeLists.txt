cmake_minimum_required(VERSION 2.6)
option(BUILD_STRUCTURES "build common data structures" ON)
option(BUILD_GEOMETRY "build the geometry system" ON)
option(BUILD_MESSAGING "build the messaging system" ON)
set(DAN_SOURCES dan_check.c)
if (BUILD_STRUCTURES)
    set(DAN_SOURCES ${DAN_SOURCES} dan_aa.c dan_list.c)
    add_executable(aa test/aa.c)
    target_link_libraries(aa dan)
endif (BUILD_STRUCTURES)
if (BUILD_GEOMETRY)
    set(DAN_SOURCES ${DAN_SOURCES}
        dan_line.c
        dan_matrix.c
        dan_point.c
        dan_vector.c)
endif (BUILD_GEOMETRY)
if (BUILD_MESSAGING)
    set(DAN_SOURCES ${DAN_SOURCES})
endif (BUILD_MESSAGING)
add_library(dan ${DAN_SOURCES})
