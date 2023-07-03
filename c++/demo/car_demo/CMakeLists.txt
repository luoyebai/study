cmake_minimum_required(VERSION 3.12)
project(car_vision_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

include_directories(
        include
        # include/*.hpp
        # include/*.h
        ${OpenCV_INCLUDE_DIRS}
)

add_executable(
        ${PROJECT_NAME}
        src/main.cpp
)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS}  pthread)


