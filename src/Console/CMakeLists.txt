set(CONSOLE_HEADERS 
	include/Console/ConsoleView.h
	include/Console/ConsoleParser.h
)

add_library(Console
	ConsoleView.cpp
	ConsoleParser.cpp
	${CONSOLE_HEADERS}
)

include_directories(include/Console)

qt5_use_modules(Console Widgets)
