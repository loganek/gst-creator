set(CONSOLE_HEADERS 
	include/Commands/AddCommand.h
	include/Commands/StateCommand.h
	include/Commands/Command.h
	include/Commands/enum_string_converter.h
)

add_library(Commands
	AddCommand.cpp
	StateCommand.cpp
	enum_string_converter.cpp
	${CONSOLE_HEADERS}
)

include_directories(include/Commands)
