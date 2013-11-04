set(UTILS_HEADERS 
	include/utils/StringUtils.h
	include/utils/GstUtils.h
)

add_library(utils
	StringUtils.cpp
	GstUtils.cpp
	${FACTORY_INSPECTOR_HEADERS}
)

include_directories(include/utils)
