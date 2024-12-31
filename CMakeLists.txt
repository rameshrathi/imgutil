cmake_minimum_required(VERSION 3.29)
project(cimg LANGUAGES CXX)

set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Manually specify the paths to the FFmpeg libraries and include directories
set(FFMPEG_INCLUDE_DIRS /opt/homebrew/include)
set(FFMPEG_LIBRARIES /opt/homebrew/lib/libavcodec.dylib
                     /opt/homebrew/lib/libavformat.dylib
                     /opt/homebrew/lib/libavutil.dylib
                     /opt/homebrew/lib/libswscale.dylib)

include_directories(${FFMPEG_INCLUDE_DIRS})
link_directories(/opt/homebrew/lib)

add_executable(cimg 
        main.cpp
        src/include/cimg.hpp
        src/include/helpers.hpp
        src/include/stream.hpp
        src/cimg.cpp
        src/helpers.cpp
        src/stream.cpp
        src/stream.cpp
)

# Link libraries
target_link_libraries(cimg 
        ${OpenCV_LIBS} 
        ${FFMPEG_LIBRARIES}
)