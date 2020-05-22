cmake_minimum_required(VERSION 2.8)

project(my_hacking_tools C)

include_directories(
    lib/
)

add_library(my_hacking_lib STATIC lib/my_hacking_lib)

add_executable(macchanger src/macchanger)
target_link_libraries(macchanger my_hacking_lib)
