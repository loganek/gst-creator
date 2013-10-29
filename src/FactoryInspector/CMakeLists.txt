set(FACTORY_INSPECTOR_HEADERS 
	include/FactoryInspector/FactoryInspectorView.h
)

add_library(FactoryInspector
	FactoryInspectorView.cpp
	${FACTORY_INSPECTOR_HEADERS}
)

include_directories(include/FactoryInspector)
qt5_use_modules(FactoryInspector Widgets)
