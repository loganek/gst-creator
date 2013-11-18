set(WORKSPACE_HEADERS 
	include/Workspace/WorkspaceWidget.h
	include/Workspace/WorkspaceController.h
	include/Workspace/GstBlock.h
	include/Workspace/GstBlockInfo.h
	include/Workspace/GstPadWidget.h
)

add_library(Workspace
	WorkspaceWidget.cpp
	WorkspaceController.cpp
	GstBlock.cpp
	GstPadWidget.cpp
	${WORKSPACE_HEADERS}
)

include_directories(include/Workspace)

qt5_use_modules(Workspace Widgets)
