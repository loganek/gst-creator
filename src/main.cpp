#include "gui/MainWindow.h"
#include "controller/MainController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	Gst::init(argc, argv);
	QApplication a(argc, argv);
	Glib::RefPtr<Gst::Pipeline> model = Gst::Pipeline::create("main-pipeline");
	MainController controller(model);
	MainWindow w(&controller);
	controller.set_main_view(&w);
	w.show();

	return a.exec();
}
