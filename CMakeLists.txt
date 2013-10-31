cmake_minimum_required(VERSION 2.8.9)

project(gst-creator)
enable_testing()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET(PROJECT_SOURCE_DIR "${gst-creator_SOURCE_DIR}/src")

INCLUDE(FindPkgConfig)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
ADD_DEFINITIONS(-DQT_NO_KEYWORDS)
pkg_check_modules(GSTMM REQUIRED gstreamermm-1.0) 
include_directories(${GSTMM_INCLUDE_DIRS})
link_directories(${GSTMM_LIBRARY_DIRS})

include_directories(
	${PROJECT_SOURCE_DIR}/ObjectInspector/include
	${PROJECT_SOURCE_DIR}/Workspace/include
	${PROJECT_SOURCE_DIR}/Console/include
	${PROJECT_SOURCE_DIR}/FactoryInspector/include
	${PROJECT_SOURCE_DIR}
)

add_subdirectory(src)
