cmake_minimum_required(VERSION 2.6)

project(cotsb_client)

set(EXECUTABLE_NAME "client")

set(CMAKE_CXX_FLAGS "-std=c++11 -g -O0 -I./ -I../common/")

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

add_executable(${EXECUTABLE_NAME} "main.cpp" ${cotsb_sources} ${common_sources} ${common_c_sources})

# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()
