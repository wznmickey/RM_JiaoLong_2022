cmake_minimum_required(VERSION 3.10)
project(Homework)

set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(HW2 HW2.cpp)
target_link_libraries(  HW2 ${OpenCV_LIBS}  )
target_link_libraries( HW2 MVSDK)