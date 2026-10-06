cmake_minimum_required(VERSION 4.3)
project(bftp C)
set(CMAKE_C_STANDARD 11)
find_library(BLUETOOTH_LIB bluetooth REQUIRED)

add_executable(bftp simplescan.c)

target_link_libraries(bftp PRIVATE ${BLUETOOTH_LIB})