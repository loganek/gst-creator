#include "gui/MainWindow.h"
#include "controller/FileController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	Gst::init(argc, argv);
	QApplication a(argc, argv);
	Glib::RefPtr<Gst::Pipeline> model = Gst::Pipeline::create("main-pipeline");
	FileController file_controller(model);
	MainWindow w(&file_controller);

	w.show();

	return a.exec();
}
