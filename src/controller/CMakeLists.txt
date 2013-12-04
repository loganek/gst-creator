set(CONTROLLER_HEADERS 
	include/controller/FileWriter.h
	include/controller/FileLoader.h
	include/controller/MainController.h
)

add_library(controller
	FileWriter.cpp
	MainController.cpp
	FileLoader.cpp
	${CONTROLLER_HEADERS}
)


qt5_use_modules(controller Widgets)

include_directories(include/controller)
