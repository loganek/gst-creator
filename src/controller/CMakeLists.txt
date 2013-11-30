set(CONTROLLER_HEADERS 
	include/controller/FileController.h
)

add_library(controller
	FileController.cpp
	${CONTROLLER_HEADERS}
)


qt5_use_modules(controller Widgets)

include_directories(include/controller)
