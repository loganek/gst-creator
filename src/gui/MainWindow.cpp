#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Console/ConsoleView.h"
#include "Logger/LoggerView.h"
#include "controller/FileController.h"
#include "ObjectInspector/ObjectInspectorModel.h"
#include "ObjectInspector/ObjectInspectorFilter.h"
#include "Workspace/WorkspaceController.h"
#include <QtWidgets/qmessagebox.h>
#include <gstreamermm.h>

MainWindow::MainWindow(FileController* file_controller, QWidget *parent)
: QMainWindow(parent),
  file_controller(file_controller),
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
	ConsoleView* console = new ConsoleView();
	LoggerView* logger = new LoggerView();

	WorkspaceWidget* workspace = new WorkspaceWidget(file_controller->get_model());
	QSplitter* spl = new QSplitter();

	QObject::connect(console, &ConsoleView::command_added, logger, &LoggerView::add_log);
	QObject::connect(console, &ConsoleView::command_added, workspace, &WorkspaceWidget::model_changed);
	console->set_model(file_controller->get_model());

	QObject::connect(workspace, &WorkspaceWidget::current_element_changed, this, &MainWindow::current_element_info);

	workspace_frame->layout()->addWidget(workspace);
	spl->setOrientation(Qt::Vertical);
	spl->addWidget(workspace_frame);
	spl->addWidget(frame);
	frame->setLayout(frameLayout);
	frameLayout->addWidget(console);
	frameLayout->addWidget(logger);
	ui->rightFrame->layout()->addWidget(spl);
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

	file_controller->save_model(filename.toUtf8().constData());
}

void MainWindow::current_element_info(const Glib::RefPtr<Gst::Element>& element)
{
	ui->currentElementLabel->setText(element->get_name().c_str());
}
