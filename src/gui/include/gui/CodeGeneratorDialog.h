/*
 * CodeGeneratorDialog.h
 *
 * Plik zawiera deklarację klasy CodeGeneratorDialog
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

/**
 * Typ określający język generowanego kodu
 */
enum class GeneratedLanguage
{
	ANSI_C,
	CPP
};

/**
 * Klasa reprezentująca okno dialogowe generatora kodu
 */
class CodeGeneratorDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * Konstruktor
	 *
	 * @param parent rodzic okna dialogowego
	 */
	explicit CodeGeneratorDialog(QWidget *parent = 0);

	/**
	 * Destruktor
	 */
	virtual ~CodeGeneratorDialog();

	/**
	 * Zwraca nazwę pliku wybraną przez użytkownika
	 *
	 * @returns nazwa pliku wybrana przez użytkownika
	 */
	QString get_file_name() const;

	/**
	 * Zwraca język generowanego kodu wybranego przez użytkownika
	 *
	 * @returns język generowanego kodu wybrany przez użytkownika
	 */
	GeneratedLanguage get_language() const;
private:
	/** obiekt interfejsu użytkownika */
	Ui::CodeGeneratorDialog* ui;
};

#endif /* CODEGENERATORDIALOG_H_ */
