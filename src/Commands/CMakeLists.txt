set(CONSOLE_HEADERS 
	include/Commands/AddCommand.h
	include/Commands/Command.h
	include/Commands/enum_string_converter.h
)

add_library(Commands
	AddCommand.cpp
	enum_string_converter.cpp
	${CONSOLE_HEADERS}
)

include_directories(include/Commands)
