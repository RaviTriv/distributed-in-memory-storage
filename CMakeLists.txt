cmake_minimum_required(VERSION 3.5)

project (in_memory_storage)


message(STATUS ${CMAKE_SOURCE_DIR})

set(SERVER_SOURCES
    src/server.cpp
    src/key-value.cpp
    src/persistence.cpp
    src/replication.cpp
)

set(CLIENT_SOURCES
    src/client.cpp
)

add_executable(server ${SERVER_SOURCES})

add_executable(client ${CLIENT_SOURCES})

# target_include_directories(in_memory_storage
#     PRIVATE 
#         ${CMAKE_SOURCE_DIR}/include
# )
