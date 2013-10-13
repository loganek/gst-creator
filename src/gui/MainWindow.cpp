#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QtWidgets/qmessagebox.h>

MainWindow::MainWindow(QWidget *parent) :
		QMainWindow(parent),
		ui(new Ui::MainWindow)
{
	ui->setupUi(this);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::set_model(QAbstractItemModel* model)
{
	connect(ui->factoryFilterEdit, SIGNAL(textChanged(QString)),
			model, SLOT(setFilterFixedString(QString)));
	ui->factoriesTreeView->setModel(model);
}

void MainWindow::on_actionAbout_triggered(bool checked)
{
	QMessageBox::about(this, "About GstCreator",
			"Author:\tMarcin Kolny\n"
			"E-mail:\tmarcin.kolny[at]gmail.com\n"
			"License:\tGPL");
}

