qt5_wrap_ui(UIS_HDRS
	ui_files/GstLogger.ui
	ui_files/GstLoggerProperties.ui
)

set(LOGGER_HEADERS 
	include/Logger/LoggerView.h
	include/Logger/GstLogger.h
	include/Logger/GstLoggerProperties.h
)

add_library(Logger
	LoggerView.cpp
	GstLogger.cpp
	GstLoggerProperties.cpp
	${LOGGER_HEADERS}
	${UIS_HDRS}
)

include_directories(include/Logger)

qt5_use_modules(Logger Widgets)
