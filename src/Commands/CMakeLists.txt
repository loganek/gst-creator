set(CONSOLE_HEADERS 
	include/Commands/AddCommand.h
	include/Commands/StateCommand.h
	include/Commands/ConnectCommand.h
	include/Commands/Command.h
)

add_library(Commands
	AddCommand.cpp
	StateCommand.cpp
	ConnectCommand.cpp
	${CONSOLE_HEADERS}
)

include_directories(include/Commands)
