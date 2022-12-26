cmake_minimum_required(VERSION 3.0.0)
project(Ball_control VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


add_executable(Ball_control ./src/main.cpp
./src/ImageProcessing/image_processing.cpp
./src/Control/control.cpp
./external/simple-serial-port/SimpleSerial.cpp
)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

target_include_directories(Ball_control PRIVATE 
./src/ImageProcessing
./src/Control
./external/simple-serial-port
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
