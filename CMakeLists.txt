cmake_minimum_required(VERSION 3.28.3)
project(bftp C)
set(CMAKE_C_STANDARD 11)
find_library(BLUETOOTH_LIB bluetooth REQUIRED)

add_executable(bftp src/main.c
        src/server.c
        src/client.c
        src/server.h
        src/client.h)

target_link_libraries(bftp PRIVATE ${BLUETOOTH_LIB})