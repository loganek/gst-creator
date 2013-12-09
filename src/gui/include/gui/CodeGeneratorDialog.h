/*
 * CodeGeneratorDialog.h
 *
 *  Created on: 9 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef CODEGENERATORDIALOG_H_
#define CODEGENERATORDIALOG_H_

#include <QDialog>

namespace Ui {
class CodeGeneratorDialog;
}

enum class GeneratedLanguage
{
	ANSI_C,
	CPP
};

class CodeGeneratorDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CodeGeneratorDialog(QWidget *parent = 0);
	virtual ~CodeGeneratorDialog();

	QString get_file_name() const;
	GeneratedLanguage get_language() const;
private:
	Ui::CodeGeneratorDialog* ui;
};

#endif /* CODEGENERATORDIALOG_H_ */
