cmake_minimum_required(VERSION 3.10)
project(MatCo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3  -march=native -w -pthread  ")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ")

include_directories(${CMAKE_SOURCE_DIR})

#utility.o
set(UTIL_SRC 
    utils/globals.cpp
    )
add_library(utility STATIC ${UTIL_SRC})

#graph.o
set(GRAPH_SRC
    graph/graph.cpp
    )
add_library(graph STATIC ${GRAPH_SRC})


#matching.o
set(MATCHING_SRC
    matching/matching.cpp
    matching/MatchCover.cpp
    )
add_library(matching STATIC ${MATCHING_SRC})


add_executable(MatCo main.cpp)
target_link_libraries(MatCo utility graph matching)


