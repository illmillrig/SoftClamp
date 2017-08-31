cmake_minimum_required(VERSION 2.6)
project(SoftClamp)

set(CMAKE_MODULE_PATH cmake module path goes here ) # FindMaya.cmake

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        SoftClamp/pluginMain.cpp
        SoftClamp/SoftClamp.cpp
        SoftClamp/SoftClamp.h)

find_package(Maya REQUIRED)

include_directories(${MAYA_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR})

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})

MAYA_PLUGIN(${PROJECT_NAME})
