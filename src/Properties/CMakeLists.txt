set(PROPERTIES_HEADERS 
	include/Properties/Property.h
	include/Properties/PropertyNumeric.h
)

add_library(Properties
	Property.cpp
	${PROPERTIES_HEADERS}
)

include_directories(include/Properties)

qt5_use_modules(Properties Widgets)
