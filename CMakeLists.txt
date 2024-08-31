cmake_minimum_required(VERSION 3.5)

project (in_memory_storage)


message(STATUS ${CMAKE_SOURCE_DIR})

set(SOURCES
    src/server.cpp
    src/key-value.cpp
    src/persistence.cpp
)

add_executable(in_memory_storage ${SOURCES})

target_include_directories(in_memory_storage
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
)
