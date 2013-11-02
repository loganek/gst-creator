set(CONSOLE_HEADERS 
	include/Commands/AddCommand.h
	include/Commands/Command.h
)

add_library(Commands
	AddCommand.cpp
	${CONSOLE_HEADERS}
)

include_directories(include/Commands)
