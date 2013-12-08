qt5_wrap_ui(UIS_HDRS ui_files/MainWindow.ui)

set(GUI_HEADERS 
	include/gui/MainWindow.h
)

add_library(gui
	MainWindow.cpp
	${GUI_HEADERS}
	${UIS_HDRS})
include_directories(include/gui)
target_link_libraries(gui Console Workspace ObjectInspector Logger controller)
qt5_use_modules(gui Widgets)