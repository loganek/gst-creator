/*
 * MainWindow.h
 *
 * Plik zawiera deklarację klasy MainWindow
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

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

/**
 * Klasa reprezentujaca główne okno programu
 */
class MainWindow : public QMainWindow, public CommandListener
{
	Q_OBJECT
protected:
	/**
	 * Zdarzenie zamykania okna
	 *
	 * @param event obiekt zdarzenia
	 */
	void closeEvent(QCloseEvent *event);

public:
	/**
	 * Konstruktor
	 *
	 * @param controller kontroler główny
	 * @parm parent widget nadrzędny
	 */
	explicit MainWindow(MainController* controller, QWidget *parent = 0);

	/**
	 * Destruktor
	 */
	~MainWindow();

	/**
	 * Funkcja wywołuje okno z pytaniem yes/no/cancel
	 *
	 * @returns wynik wyboru użytkownika
	 */
	QMessageBox::StandardButton ask_before_save();

	/**
	 * Obsługa zmiany stanu modelu programu
	 *
	 * @param state stan
	 */
	void state_changed(State state);

	/**
	 * Obsługa zmiany zawartości modelu programu
	 */
	void modified_state_changed(bool state);

	/**
	 * Obsługa zmiany nazwy pliku projektu
	 *
	 * @param filename nowa nazwa dla pliku projektu
	 */
	void current_project_file_changed(const std::string& filename);

private Q_SLOTS:
	/**** sloty obsługujące poszczególne akcje menu głównego ****/
	void on_actionAbout_triggered(bool checked);
	void on_actionSave_As_triggered(bool checked);
	void on_actionLoad_triggered(bool checked);
	void on_actionCode_Generator_triggered(bool checked);
	void on_actionLoad_Plugin_triggered(bool checked);
	void on_actionAdd_Plugin_Path_triggered(bool checked);
	void on_actionExit_triggered(bool checked);
	void on_actionPlugin_Wizzard_triggered(bool checked);
	void on_actionSave_triggered(bool checked);
	void on_actionNew_Project_triggered(bool checked);

	/**
	 * Pokazuje okno błędu
	 *
	 * @param text tekst błędu
	 */
	static void show_error_box(QString text);

	/**
	 * Wywołuje okno zapisu projektu, ustawia nową nazwę pliku w kontrolerze
	 *
	 * @returns true, jeśli proces nie zostanie przerwany
	 */
	bool save_project_dialog();

	/**
	 * Zapisuje projekt
	 */
	void save_project();

private:
	/**
	 * Dodaje do okna głównego obszar roboczy
	 */
	void add_workspace_canvas();

	/** obiekt interfejsu użytkownika */
	Ui::MainWindow *ui;
	/** obiekt obszaru roboczego */
	WorkspaceWidget* workspace;
	/** obiekt inspektora wtyczek */
	ObjectInspectorView plugins_tree;
	/** główny kontroler */
	MainController* controller;
	/** aktualnie wybrany element */
	Glib::RefPtr<Gst::Element> selected_element;
	/** flaga informująca o tym, czy GUI może być w danej chwili modyfikowane */
	bool can_modify_gui;

	/**
	 * Przeładowuje listę wtyczek
	 */
	void reload_plugins();

private Q_SLOTS:
	/**
	 * Slot zmiany aktualnego elementu
	 *
	 * @param element aktualny element
	 */
	void current_element_info(const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Slot zmiany stanu modelu programu na zatrzymanie
	 */
	void pipeline_state_stopped(bool);

	/**
	 * Slot zmiany stanu modelu programu na pauzę
	 */
	void pipeline_state_paused(bool);

	/**
	 * Slot zmiany stanu modelu programu na odtwarzanie
	 */
	void pipeline_state_playing(bool);
};

#endif // MAINWINDOW_H
