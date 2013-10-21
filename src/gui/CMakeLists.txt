qt5_wrap_ui(UIS_HDRS MainWindow.ui)
add_subdirectory(blocks)
add_library(gui
	MainWindow.cpp
	blocks
	${UIS_HDRS})

target_link_libraries(gui blocks)
qt5_use_modules(gui Widgets)