/*
 * CodeGeneratorDialog.cpp
 *
 *  Created on: 9 gru 2013
 *      Author: Marcin Kolny
 */

#include "CodeGeneratorDialog.h"
#include "ui_CodeGeneratorDialog.h"

#include <QFileDialog>

CodeGeneratorDialog::CodeGeneratorDialog(QWidget *parent)
: QDialog(parent),
  ui(new Ui::CodeGeneratorDialog)
{
	ui->setupUi(this);

	connect(ui->selectFilePushButton, &QPushButton::clicked, [this](bool checked){
		QString filename = QFileDialog::getSaveFileName(this, "Generated Code Location", QDir::currentPath(),
				"C++ files (*.cpp);;ANSI C files (*.c);;All files (*.*)", 0, QFileDialog::DontUseNativeDialog);
		if (!filename.isNull())
			ui->selectedFileLineEdit->setText(filename);
	});
}

QString CodeGeneratorDialog::get_file_name() const
{
	QString name = ui->selectedFileLineEdit->text();
	if (get_language() == GeneratedLanguage::CPP && !name.endsWith(".cpp", Qt::CaseInsensitive))
		name += ".cpp";
	if (get_language() == GeneratedLanguage::ANSI_C && !name.endsWith(".c", Qt::CaseInsensitive))
		name += ".c";
	return name;
}

GeneratedLanguage CodeGeneratorDialog::get_language() const
{
	if (ui->cppRadioButton->isChecked())
		return GeneratedLanguage::CPP;

	return GeneratedLanguage::ANSI_C;
}

CodeGeneratorDialog::~CodeGeneratorDialog()
{
	delete ui;
}



