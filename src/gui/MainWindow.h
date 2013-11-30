#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ObjectInspector/ObjectInspectorView.h"
#include "Workspace/WorkspaceWidget.h"

#include <QMainWindow>
#include <QAbstractItemModel>
#include <gstreamermm.h>

namespace Ui {
class MainWindow;
}

class FileController;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(FileController* file_controller, QWidget *parent = 0);
	~MainWindow();

private Q_SLOTS:
	void on_actionAbout_triggered(bool checked);
	void on_actionSave_As_triggered(bool checked);

private:
	void add_workspace_canvas();
	Ui::MainWindow *ui;
	ObjectInspectorView plugins_tree;
	FileController* file_controller;

private Q_SLOTS:
	void current_element_info(const Glib::RefPtr<Gst::Element>& element);
};

#endif // MAINWINDOW_H
