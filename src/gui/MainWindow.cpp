#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	centralWidget()->setLayout(ui->verticalLayout);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::set_model(QAbstractItemModel* model)
{
	ui->factoriesTreeView->setModel(model);
}
