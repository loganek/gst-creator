set(OBJECT_INSPECTOR_HEADERS 
	include/ObjectInspector/ObjectInspectorView.h
	include/ObjectInspector/ObjectInspectorModel.h
	include/ObjectInspector/ObjectInspectorItem.h
	include/ObjectInspector/ObjectInspectorFilter.h
	include/ObjectInspector/PluginItem.h
	)

add_library(ObjectInspector
	ObjectInspectorView.cpp
	ObjectInspectorModel.cpp
	ObjectInspectorItem.cpp
	ObjectInspectorFilter.cpp
	PluginItem.cpp
	${OBJECT_INSPECTOR_HEADERS}
)

include_directories(include/ObjectInspector)

qt5_use_modules(ObjectInspector Widgets)