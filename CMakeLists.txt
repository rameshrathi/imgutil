cmake_minimum_required(VERSION 3.29)
project(cimg LANGUAGES CXX)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(cimg main.cpp
        src/include/cimg.h
        src/include/utils.h
        src/cimg.cpp
        src/utils.cpp
)

# link libraries
target_link_libraries(cimg ${OpenCV_LIBS})