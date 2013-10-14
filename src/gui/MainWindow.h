#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include "DraggedTreeView.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void set_model(QAbstractItemModel* model);

private Q_SLOTS:
	void on_actionAbout_triggered(bool checked);

private:
	Ui::MainWindow *ui;
	DraggedTreeView plugins_tree;
};

#endif // MAINWINDOW_H
