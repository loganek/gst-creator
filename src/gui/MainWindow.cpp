#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Console/ConsoleView.h"
#include "Logger/LoggerView.h"
#include "controller.h"
#include "ObjectInspector/ObjectInspectorModel.h"
#include "ObjectInspector/ObjectInspectorFilter.h"
#include <QtWidgets/qmessagebox.h>
#include <gstreamermm.h>

MainWindow::MainWindow(MainController* controller, QWidget *parent)
: QMainWindow(parent),
  controller(controller),
  ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->objectInspectorFrame->layout()->addWidget(&plugins_tree);

	add_workspace_canvas();

	setAcceptDrops(false);

	ObjectInspectorFilter* filter = new ObjectInspectorFilter();
	filter->setSourceModel(new ObjectInspectorModel());

	connect(ui->factoryFilterEdit, SIGNAL(textChanged(QString)),
			filter, SLOT(setFilterFixedString(QString)));

	plugins_tree.setModel(filter);
}

void MainWindow::add_workspace_canvas()
{
	QVBoxLayout *frameLayout = new QVBoxLayout;
	QFrame* workspace_frame = new QFrame();
	workspace_frame->setLayout(new QGridLayout());
	QFrame *frame = new QFrame;
	workspace = new WorkspaceWidget(controller->get_model());
	ConsoleView* console = new ConsoleView(workspace);
	LoggerView* logger = new LoggerView();


	QSplitter* spl = new QSplitter();

	QObject::connect(console, &ConsoleView::command_added, logger, &LoggerView::add_log);
	console->set_model(controller->get_model());

	QObject::connect(workspace, &WorkspaceWidget::current_element_changed, this, &MainWindow::current_element_info);

	workspace_frame->layout()->addWidget(workspace);
	spl->setOrientation(Qt::Vertical);
	spl->addWidget(workspace_frame);
	spl->addWidget(frame);
	frame->setLayout(frameLayout);
	frameLayout->addWidget(console);
	frameLayout->addWidget(logger);
	ui->rightFrame->layout()->addWidget(spl);

	controller->get_model()->signal_element_added().connect([this](const Glib::RefPtr<Gst::Element>& e) {
		workspace->new_element_added(e);
	});

	controller->get_model()->signal_element_removed().connect([this](const Glib::RefPtr<Gst::Element>& e) {
		workspace->element_removed(e);
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionAbout_triggered(bool checked)
{
	QMessageBox::about(this, "About GstCreator",
			"Author:\tMarcin Kolny\n"
			"E-mail:\tmarcin.kolny[at]gmail.com\n"
			"License:\tGPL");
}

void MainWindow::on_actionSave_As_triggered(bool checked)
{
	QString filename = QFileDialog::getSaveFileName(this, "Save Project", QDir::currentPath(),
			"gst-creator files (*.gstc);;All files (*.*)", 0, QFileDialog::DontUseNativeDialog);

	if (filename.isNull())
		return;

	FileWriter(filename.toUtf8().constData(), controller->get_model()).save_model();
}

void MainWindow::on_actionLoad_triggered(bool checked)
{
	QString filename = QFileDialog::getOpenFileName(this, "Save Project", QDir::currentPath(),
			"gst-creator files (*.gstc);;All files (*.*)", 0, QFileDialog::DontUseNativeDialog);

	if (filename.isNull())
		return;

	FileLoader(filename.toUtf8().constData(), controller->get_model()).load_model(workspace);
}

void MainWindow::current_element_info(const Glib::RefPtr<Gst::Element>& element)
{
	ui->currentElementLabel->setText(element->get_name().c_str());
}

void MainWindow::on_actionGenerate_Cpp_Code_triggered(bool checked)
{
	CodeGenerator generator(controller->get_model());

	generator.generate_code("output.cpp");
}

void MainWindow::on_actionLoad_Plugin_triggered(bool checked)
{

}

void MainWindow::on_actionAdd_Plugin_Path_triggered(bool checked)
{

}
