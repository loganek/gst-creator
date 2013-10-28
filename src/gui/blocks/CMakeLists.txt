add_library(blocks
	GstBlock.cpp
	WorkspaceCanvas.cpp
	GstBlockInfo.h)
	
qt5_use_modules(blocks Widgets)