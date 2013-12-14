/*
 * PluginWizardDialog.h
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

class PluginWizardDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PluginWizardDialog(QWidget *parent = 0);
	~PluginWizardDialog();

	PluginInfo get_plugin() const;
	FactoryInfo get_factory() const;
	QString get_directory() const;
private:
	Ui::PluginWizardDialog *ui;
	QString directory_name;
};

#endif /* PLUGINWIZARDDIALOG_H_ */
