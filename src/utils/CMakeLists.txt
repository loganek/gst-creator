set(UTILS_HEADERS 
	include/utils/StringUtils.h
)

add_library(utils
	StringUtils.cpp
	${FACTORY_INSPECTOR_HEADERS}
)

include_directories(include/utils)
