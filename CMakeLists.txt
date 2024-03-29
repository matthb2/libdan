cmake_minimum_required(VERSION 2.6)
find_package(MPI) 
include_directories(${MPI_C_INCLUDE_PATH})

if(CMAKE_C_COMPILER_ID MATCHES GNU OR CMAKE_C_COMPILER_ID MATCHES Intel)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endif(CMAKE_C_COMPILER_ID MATCHES GNU OR CMAKE_C_COMPILER_ID MATCHES Intel)
if(CMAKE_C_COMPILER_ID MATCHES PGI)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -c99")
endif(CMAKE_C_COMPILER_ID MATCHES PGI)


option(BUILD_STRUCTURES "build common data structures" ON)
option(BUILD_GEOMETRY "build the geometry system" ON)

if(MPI_C_FOUND)
option(BUILD_MESSAGING "build the messaging system" ON)
else()
option(BUILD_MESSAGING "build the messaging system" OFF)
endif(MPI_C_FOUND)

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
        dan_pmsg.c
        dan_neighbor.c
        PCU.c)
    install(FILES
            dan_mpi.h
            dan_pmsg.h
            dan_neighbor.h
            PCU.h
            DESTINATION include)
    add_executable(ibarrier test/mpi_ibarrier.c)
    target_link_libraries(ibarrier dan ${MPI_C_LIBRARIES})
    add_executable(pmsg test/pmsg.c)
    target_link_libraries(pmsg dan ${MPI_C_LIBRARIES})
endif (BUILD_MESSAGING)
add_library(dan ${DAN_SOURCES})
install(TARGETS dan LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
