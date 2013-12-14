qt5_wrap_ui(UIS_HDRS
	ui_files/CodeGeneratorDialog.ui
	ui_files/MainWindow.ui
	ui_files/PluginWizardDialog.ui
)

set(GUI_HEADERS 
	include/gui/MainWindow.h
	include/gui/PluginWizardDialog.h
	include/gui/CodeGeneratorDialog.h
)

add_library(gui
	MainWindow.cpp
	CodeGeneratorDialog.cpp
	PluginWizardDialog.cpp
	${GUI_HEADERS}
	${UIS_HDRS})
include_directories(include/gui)
target_link_libraries(gui Console Workspace ObjectInspector Logger controller)
qt5_use_modules(gui Widgets)