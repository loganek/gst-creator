set(UTILS_HEADERS 
	include/utils/StringUtils.h
	include/utils/GstUtils.h
	include/utils/EnumUtils.h
)

add_library(utils
	StringUtils.cpp
	EnumUtils.cpp
	GstUtils.cpp
	${FACTORY_INSPECTOR_HEADERS}
)

include_directories(include/utils)
