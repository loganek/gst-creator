add_library(model FactoryItem.cpp ElementFactoryItem.cpp TypeFindFactoryItem.cpp PluginItem.cpp PluginListModel.cpp)

qt5_use_modules(model Widgets)
