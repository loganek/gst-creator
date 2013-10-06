#include "gui/MainWindow.h"
#include "model/PluginListModel.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;

	w.set_model(new PluginListModel());

	w.show();

	return a.exec();
}
