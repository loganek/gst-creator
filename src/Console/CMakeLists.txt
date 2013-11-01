set(CONSOLE_HEADERS 
	include/Console/ConsoleView.h
	include/Console/CommandParser.h
)

add_library(Console
	ConsoleView.cpp
	CommandParser.cpp
	${CONSOLE_HEADERS}
)

include_directories(include/Console)

qt5_use_modules(Console Widgets)
