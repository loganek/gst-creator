/*
 * PluginWizzardDialog.h
 *
 *  Created on: 9 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGINWIZZARDDIALOG_H_
#define PLUGINWIZZARDDIALOG_H_

#include <QDialog>
#include <string>

#include "controller/PluginWizard/FactoryInfo.h"
#include "controller/PluginWizard/PluginInfo.h"

namespace Ui {
class PluginWizzardDialog;
}

class PluginWizzardDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PluginWizzardDialog(QWidget *parent = 0);
	~PluginWizzardDialog();

	PluginInfo get_plugin() const;
	FactoryInfo get_factory() const;
	QString get_directory() const;
private:
	Ui::PluginWizzardDialog *ui;
	QString directory_name;
};

#endif /* PLUGINWIZZARDDIALOG_H_ */
