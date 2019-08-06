cmake_minimum_required(VERSION 2.8.3 FATAL_ERROR)
project(single_camera_calibrate LANGUAGES CXX)

add_definitions(-DCURRENT_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

find_package(OpenCV REQUIRED)

include_directories(
    include
    ${OpenCV_INCLUDE_DIRS}
)

link_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

add_library(${PROJECT_NAME}_lib SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/src/single_camera_calibrate.cpp
)

target_link_libraries(${PROJECT_NAME}_lib
    ${OpenCV_LIBRARIES}
    -lqy_util_lib
)

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${PROJECT_NAME}_lib
)