set(FACTORY_INSPECTOR_HEADERS 
	include/FactoryInspector/FactoryInspectorView.h
	include/FactoryInspector/PropertyInspectorView.h
	include/FactoryInspector/VariousPropertyInspectorView.h
)

add_library(FactoryInspector
	FactoryInspectorView.cpp
	PropertyInspectorView.cpp
	VariousPropertyInspectorView.cpp
	${FACTORY_INSPECTOR_HEADERS}
)

include_directories(include/FactoryInspector)
qt5_use_modules(FactoryInspector Widgets)
