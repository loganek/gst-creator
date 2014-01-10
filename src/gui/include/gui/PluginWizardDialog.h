/*
 * PluginWizardDialog.h
 *
 * Plik zawiera deklarację klasy PluginWizardDialog
 *
 *  Created on: 9 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGINWIZARDDIALOG_H_
#define PLUGINWIZARDDIALOG_H_

#include <QDialog>
#include <string>

#include "controller/PluginWizard/FactoryInfo.h"
#include "controller/PluginWizard/PluginInfo.h"

namespace Ui {
class PluginWizardDialog;
}

/**
 * Klasa reprezentująca okno dialogowe kreatora wtyczek
 */
class PluginWizardDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * Konstruktor
	 *
	 * @param parent rodzic okna dialogowego
	 */
	explicit PluginWizardDialog(QWidget *parent = 0);

	/**
	 * Destruktor
	 */
	~PluginWizardDialog();

	/**
	 * Zwraca informację na temat tworzonej wtyczki
	 */
	PluginInfo get_plugin() const;

	/**
	 * Zwraca informację na temat tworzonej fabryki elementów
	 */
	FactoryInfo get_factory() const;

	/**
	 * Zwraca lokalizację wtyczki
	 */
	QString get_directory() const;
private:
	/** obiekt interfejsu użytkownika */
	Ui::PluginWizardDialog *ui;
	/** lokalizacja wtyczki */
	QString directory_name;
};

#endif /* PLUGINWIZARDDIALOG_H_ */
