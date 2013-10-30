set(FACTORY_INSPECTOR_HEADERS 
	include/FactoryInspector/FactoryInspectorView.h
	include/FactoryInspector/PropertyInspectorView.h
)

add_library(FactoryInspector
	FactoryInspectorView.cpp
	PropertyInspectorView.cpp
	${FACTORY_INSPECTOR_HEADERS}
)

include_directories(include/FactoryInspector)
qt5_use_modules(FactoryInspector Widgets)
