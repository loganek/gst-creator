cmake_minimum_required(VERSION 2.8.9)

project(gst-creator)

set(CMAKE_PREFIX_PATH $ENV{QTDIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)

add_subdirectory(gui)
add_executable(gst-creator main.cpp)

target_link_libraries(gst-creator gui)

qt5_use_modules(gst-creator Widgets)
