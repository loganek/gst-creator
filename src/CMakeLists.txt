set(UTILS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/utils)

execute_process(
	COMMAND perl
	${UTILS_DIR}/enum_string_generator.pl
	${UTILS_DIR}/../Commands/include/Commands/Command.h
	${UTILS_DIR}/include/utils/EnumUtils.h
	${UTILS_DIR}/EnumUtils.cpp
	 
)

add_subdirectory(utils)
add_subdirectory(gui)
add_subdirectory(Workspace)
add_subdirectory(ObjectInspector)
add_subdirectory(FactoryInspector)
add_subdirectory(Console)
add_subdirectory(Commands)
add_subdirectory(Logger)
add_subdirectory(Properties)
add_subdirectory(controller)

add_executable(gst-creator main.cpp)

target_link_libraries(gst-creator Console Workspace ObjectInspector FactoryInspector Logger gui ${GSTMM_LIBRARIES})

qt5_use_modules(gst-creator Widgets)
