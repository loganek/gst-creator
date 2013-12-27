#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ObjectInspector/ObjectInspectorView.h"
#include "Workspace/WorkspaceWidget.h"
#include "Commands/CommandListener.h"

#include <QMainWindow>
#include <QAbstractItemModel>
#include <gstreamermm.h>

namespace Ui {
class MainWindow;
}

class MainController;

class MainWindow : public QMainWindow, public CommandListener
{
	Q_OBJECT

public:
	explicit MainWindow(MainController* file_controller, QWidget *parent = 0);
	~MainWindow();

	QMessageBox::StandardButton ask_before_save();
	void state_changed(State state);

	void modified_state_changed(bool state);
	void current_project_file_changed(const std::string& filename);

private Q_SLOTS:
	void on_actionAbout_triggered(bool checked);
	void on_actionSave_As_triggered(bool checked);
	void on_actionLoad_triggered(bool checked);
	void on_actionCode_Generator_triggered(bool checked);
	void on_actionLoad_Plugin_triggered(bool checked);
	void on_actionAdd_Plugin_Path_triggered(bool checked);
	void on_actionExit_triggered(bool checked);
	void on_actionPlugin_Wizzard_triggered(bool checked);
	void on_actionSave_triggered(bool checked);

	static void show_error_box(QString text);
	bool save_project_dialog();
	void save_project();

private:
	void add_workspace_canvas();
	Ui::MainWindow *ui;
	WorkspaceWidget* workspace;
	ObjectInspectorView plugins_tree;
	MainController* controller;
	Glib::RefPtr<Gst::Element> selected_element;

	void reload_plugins();

private Q_SLOTS:
	void current_element_info(const Glib::RefPtr<Gst::Element>& element);
	void pipeline_state_stopped(bool);
	void pipeline_state_paused(bool);
	void pipeline_state_playing(bool);
};

#endif // MAINWINDOW_H
