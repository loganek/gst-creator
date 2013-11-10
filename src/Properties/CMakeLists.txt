set(PROPERTIES_HEADERS 
	include/Properties/Property.h
	include/Properties/PropertyNumeric.h
	include/Properties/PropertyBoolean.h
	include/Properties/PropertyString.h
	include/Properties/PropertyCaps.h
	include/Properties/PropertyEnum.h
)

add_library(Properties
	Property.cpp
	PropertyBoolean.cpp
	PropertyString.cpp
	PropertyCaps.cpp
	PropertyEnum.cpp
	${PROPERTIES_HEADERS}
)

include_directories(include/Properties)

qt5_use_modules(Properties Widgets)
