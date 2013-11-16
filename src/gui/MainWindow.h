#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ObjectInspector/ObjectInspectorView.h"
#include "Workspace/WorkspaceWidget.h"

#include <QMainWindow>
#include <QAbstractItemModel>

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
	void add_workspace_canvas();
	Ui::MainWindow *ui;
	ObjectInspectorView plugins_tree;
	WorkspaceWidget canvas;
};

#endif // MAINWINDOW_H
