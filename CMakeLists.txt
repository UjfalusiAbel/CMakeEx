cmake_minimum_required(VERSION 3.12)
project(robot VERSION 1.0.0 LANGUAGES CXX)

file(GLOB ROBOT_SRC
"*/*.h"
"*/*.cpp"
)

set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)

include_directories(${GPIOD_INCLUDE_DIRS})
link_directories(${GPIOD_LIBRARY_DIRS})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(robot ${ROBOT_SRC})
target_link_libraries(robot ${GPIOD_LIBRARIES})