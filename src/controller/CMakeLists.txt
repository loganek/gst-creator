set(CONTROLLER_HEADERS 
	include/controller/FileWriter.h
	include/controller/FileLoader.h
	include/controller/MainController.h
	include/controller/PluginWizard/PluginCodeGenerator.h
	include/controller/PluginWizard/FactoryInfo.h
	include/controller/PluginWizard/PluginInfo.h
)

add_library(controller
	FileWriter.cpp
	MainController.cpp
	FileLoader.cpp
	CodeGenerator.cpp
	PluginWizard/PluginCodeGenerator.cpp
	PluginWizard/FactoryInfo.cpp
	PluginWizard/PluginInfo.cpp
	${CONTROLLER_HEADERS}
)

qt5_use_modules(controller Widgets)

include_directories(include/controller)
