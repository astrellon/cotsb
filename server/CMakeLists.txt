cmake_minimum_required(VERSION 2.6)

project(cotsb_server)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -O0 -I../ -I../../common/")

file(GLOB_RECURSE cotsb_headers "cotsb/*.h")
file(GLOB_RECURSE cotsb_sources "cotsb/*.cpp")

file(GLOB_RECURSE common_headers "../common/*.h")
file(GLOB_RECURSE common_sources "../common/*.cpp")
file(GLOB_RECURSE common_c_sources "../common/*.c")

set(cotsb_include_dirs "")
foreach (_file ${cotsb_headers})
    get_filename_component(_dir ${_file} PATH)
    list (APPEND cotsb_include_dirs ${_dir})
endforeach()

foreach (_file ${common_headers})
    get_filename_component(_dir ${_file} PATH)
    list (APPEND cotsb_include_dirs ${_dir})
endforeach()
list(REMOVE_DUPLICATES cotsb_include_dirs)

include_directories(${cotsb_include_dirs})

add_executable(server "main.cpp" ${cotsb_sources} ${common_sources} ${common_c_sources})
target_link_libraries(server sfml-system sfml-network sfml-graphics sfml-window sfml-audio pthread)

