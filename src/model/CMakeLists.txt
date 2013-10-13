add_library(model
	ElementFactoryItem.cpp
	FactoryItem.cpp
	FactoryTreeProxyFilter.cpp
	PluginItem.cpp
	PluginListModel.cpp
	TypeFindFactoryItem.cpp)

qt5_use_modules(model Widgets)
