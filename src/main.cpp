#include "gui/MainWindow.h"
#include "model/PluginListModel.h"
#include "model/FactoryTreeProxyFilter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	Gst::init(argc, argv);
	QApplication a(argc, argv);
	MainWindow w;

	FactoryTreeProxyFilter* filter = new FactoryTreeProxyFilter();
	filter->setSourceModel(new PluginListModel());
	w.set_model(filter);

	w.show();

	return a.exec();
}
