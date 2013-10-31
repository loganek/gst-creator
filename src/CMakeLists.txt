add_subdirectory(gui)
add_subdirectory(Workspace)
add_subdirectory(ObjectInspector)
add_subdirectory(FactoryInspector)
add_subdirectory(Console)

add_executable(gst-creator main.cpp)

target_link_libraries(gst-creator Console Workspace ObjectInspector FactoryInspector gui ${GSTMM_LIBRARIES})

qt5_use_modules(gst-creator Widgets)
