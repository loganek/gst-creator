/*
 * PluginWizzardDialog.h
 *
 *  Created on: 9 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef PLUGINWIZZARDDIALOG_H_
#define PLUGINWIZZARDDIALOG_H_

#include <QDialog>

namespace Ui {
class PluginWizzardDialog;
}

class PluginWizzardDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PluginWizzardDialog(QWidget *parent = 0);
	~PluginWizzardDialog();

private:
	Ui::PluginWizzardDialog *ui;
};

#endif /* PLUGINWIZZARDDIALOG_H_ */
