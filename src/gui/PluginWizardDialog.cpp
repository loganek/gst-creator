/*
 * PluginWizzardDialog.cpp
 *
 *  Created on: 9 gru 2013
 *      Author: Marcin Kolny
 */

#include "PluginWizardDialog.h"
#include "ui_PluginWizardDialog.h"
#include <stdexcept>
#include <QDateTime>
#include <QFileDialog>
#include <gst/gst.h>

PluginWizardDialog::PluginWizardDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::PluginWizardDialog)
{
	ui->setupUi(this);
	ui->pluginReleaseDateEdit->setText(QDate::currentDate().toString("yyyy-MM-dd"));
	ui->pluginVersionEdit->setText(QString("%1.%2").arg(GST_VERSION_MAJOR).arg(GST_VERSION_MINOR));
	QObject::connect(ui->selectPathPushButton, &QPushButton::clicked, [this](bool){
		directory_name = QFileDialog::getExistingDirectory(this, "Select Plugin Path",
				QDir::currentPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

		if (directory_name.isNull())
			return;

		ui->pluginFileNameEdit->setText(directory_name);
	});
}

PluginWizardDialog::~PluginWizardDialog()
{
	delete ui;
}

PluginInfo PluginWizardDialog::get_plugin() const
{
	return PluginInfo(ui->pluginNameEdit->text().toUtf8().constData(),
			ui->pluginDescriptionEdit->text().toUtf8().constData(),
			ui->pluginVersionEdit->text().toUtf8().constData(),
			ui->pluginLicenseEdit->text().toUtf8().constData(),
			ui->pluginSourceModuleEdit->text().toUtf8().constData(),
			ui->pluginPackageEdit->text().toUtf8().constData(),
			ui->pluginOriginURLEdit->text().toUtf8().constData());
}

FactoryInfo PluginWizardDialog::get_factory() const
{
	bool ok;
	int rank = ui->factoryRankEdit->text().toInt(&ok, 10);

	if (!ok)
		throw std::runtime_error("Rank is not a number.");

	FactoryInfo factory(ui->factoryNameEdit->text().toUtf8().constData(),
			ui->factoryLongNameEdit->text().toUtf8().constData(),
			ui->factoryKlassEdit->text().toUtf8().constData(),
			ui->factoryAuthorEdit->text().toUtf8().constData(),
			ui->factoryDescriptionEdit->text().toUtf8().constData(),
			ui->factoryParentEdit->text().toUtf8().constData(),
			rank);

	factory.set_virtual_methods(ui->virtualRequestNewPadCheckBox->isChecked(),
			ui->virtualReleasePadCheckBox->isChecked(),
			ui->virtualChangeStateCheckBox->isChecked(),
			ui->virtualSetBusCheckbox->isChecked());

	return factory;
}

QString PluginWizardDialog::get_directory() const
{
	return directory_name;
}
