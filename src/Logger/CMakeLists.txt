set(LOGGER_HEADERS 
	include/Logger/LoggerView.h
)

add_library(Logger
	LoggerView.cpp
	${LOGGER_HEADERS}
)

include_directories(include/Logger)

qt5_use_modules(Logger Widgets)
