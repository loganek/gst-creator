add_library(blocks
	DraggedTreeView.cpp
	GstBlock.cpp
	WorkspaceCanvas.cpp
	GstBlockInfo.h)
	
qt5_use_modules(blocks Widgets)