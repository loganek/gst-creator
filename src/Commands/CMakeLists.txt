set(CONSOLE_HEADERS 
	include/Commands/AddCommand.h
	include/Commands/StateCommand.h
	include/Commands/ConnectCommand.h
	include/Commands/Command.h
	include/Commands/PropertyCommand.h
)

add_library(Commands
	AddCommand.cpp
	PropertyCommand.cpp
	StateCommand.cpp
	ConnectCommand.cpp
	${CONSOLE_HEADERS}
)

target_link_libraries(Commands Properties)

include_directories(include/Commands)

qt5_use_modules(Commands Widgets)