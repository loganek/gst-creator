#include "gui/MainWindow.h"
#include "ObjectInspector/ObjectInspectorModel.h"
#include "ObjectInspector/ObjectInspectorFilter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	Gst::init(argc, argv);
	QApplication a(argc, argv);
	MainWindow w;

	ObjectInspectorFilter* filter = new ObjectInspectorFilter();
	filter->setSourceModel(new ObjectInspectorModel());
	w.set_model(filter);

	w.show();

	return a.exec();
}
