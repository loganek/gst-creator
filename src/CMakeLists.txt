cmake_minimum_required(VERSION 2.8.9)

project(gst-creator)

INCLUDE(FindPkgConfig)

set(CMAKE_PREFIX_PATH $ENV{QTDIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
pkg_check_modules(GSTMM REQUIRED gstreamermm-1.0)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
include_directories(${GSTMM_INCLUDE_DIRS})
link_directories(${GSTMM_LIBRARY_DIRS})

add_subdirectory(gui)
add_subdirectory(model)
add_executable(gst-creator main.cpp)

target_link_libraries(gst-creator model gui ${GSTMM_LIBRARIES})

qt5_use_modules(gst-creator Widgets)
