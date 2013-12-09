/*
 * PluginWizzardDialog.cpp
 *
 *  Created on: 9 gru 2013
 *      Author: Marcin Kolny
 */

#include "PluginWizzardDialog.h"
#include "ui_PluginWizzardDialog.h"
#include <QDateTime>
#include <gst/gst.h>

PluginWizzardDialog::PluginWizzardDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::PluginWizzardDialog)
{
	ui->setupUi(this);
	ui->pluginReleaseDateEdit->setText(QDate::currentDate().toString("yyyy-MM-dd"));
	ui->pluginVersionEdit->setText(QString("%1.%2").arg(GST_VERSION_MAJOR).arg(GST_VERSION_MINOR));
}

PluginWizzardDialog::~PluginWizzardDialog()
{
	delete ui;
}


