cmake_minimum_required(VERSION 2.6)
option(BUILD_STRUCTURES "build common data structures" ON)
option(BUILD_GEOMETRY "build the geometry system" ON)
option(BUILD_MESSAGING "build the messaging system" ON)
set(DAN_SOURCES dan.c dan_memory.c)
install(FILES dan.h dan_memory.h DESTINATION include)
if (BUILD_STRUCTURES)
    set(DAN_SOURCES ${DAN_SOURCES}
        dan_aa.c
        dan_list.c
        dan_set.c)
    install(FILES
            dan_aa.h
            dan_list.h
            dan_set.h
            DESTINATION include)
    add_executable(aa test/aa.c)
    target_link_libraries(aa dan)
    add_executable(set test/set.c)
    target_link_libraries(set dan)
endif (BUILD_STRUCTURES)
if (BUILD_GEOMETRY)
    set(DAN_SOURCES ${DAN_SOURCES}
        dan_circle.c
        dan_line.c
        dan_matrix.c
        dan_point.c
        dan_vector.c)
    install(FILES
            dan_circle.h
            dan_line.h
            dan_matrix.h
            dan_point.h
            dan_vector.h
            DESTINATION include)
endif (BUILD_GEOMETRY)
if (BUILD_MESSAGING)
    set(DAN_SOURCES ${DAN_SOURCES}
        dan_mpi.c
        dan_bsp.c
        dan_neighbor.c)
    install(FILES
            dan_mpi.h
            dan_bsp.h
            dan_neighbor.h
            DESTINATION include)
    add_executable(ibarrier test/mpi_ibarrier.c)
    target_link_libraries(ibarrier dan)
    add_executable(bsp test/bsp.c)
    target_link_libraries(bsp dan)
endif (BUILD_MESSAGING)
add_library(dan ${DAN_SOURCES})
install(TARGETS dan LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
