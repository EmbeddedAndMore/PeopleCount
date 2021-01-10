cmake_minimum_required(VERSION 3.17)
project(BlobDetection)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

find_package( OpenCV REQUIRED )
#include_directories( "/usr/local/include/opencv4" )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(BlobDetection main.cpp ConcurrentQueue.h)

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS}) # link OpenCV libraries , hightgui.so not found by cmake so this hack
MESSAGE("OpenCV_LIBS: " ${OpenCV_LIBS} )  #display opencv libs found