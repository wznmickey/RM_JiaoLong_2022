cmake_minimum_required(VERSION 3.10)
project(Homework)

set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(HW3 HW3.cpp)
target_link_libraries(  HW3 ${OpenCV_LIBS}  )