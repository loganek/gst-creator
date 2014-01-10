/*
 * GstLoggerProperties.h
 *
 * Plik zawiera definicję klasy GstLoggerProperties
 *
 *  Created on: 5 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTLOGGERPROPERTIES_H
#define GSTLOGGERPROPERTIES_H

#include <QtWidgets>
#include <gst/gst.h>
#include <map>

namespace Ui {
class GstLoggerProperties;
}

/**
 * Klasa odzwierciedla okno dialogowe właściwości loggera GStreamer
 */
class GstLoggerProperties : public QDialog
{
	Q_OBJECT

public:
	/**
	 * Konstruktor
	 *
	 * @param parent nadrzędny widget
	 */
	explicit GstLoggerProperties(QWidget *parent = 0);
	/**
	 * Destruktor
	 */
	~GstLoggerProperties();

	/**
	 * Sprawdza, czy wybrana opcja to "Default level"
	 *
	 * @returns true, jeśli wybrana opcja to "Default level"
	 */
	bool is_default_level() const;

	/**
	 * Zwraca domyślny poziom logowania
	 *
	 * @returns domyślny poziom logowania
	 */
	GstDebugLevel get_default_level() const;

	/**
	 * Zwraca mapę odzwierciedlającą poziom logowania dla danego elementu
	 *
	 * @returns mapa odzwierciedlającą poziom logowania dla danego elementu
	 */
	std::map<GstDebugCategory*, GstDebugLevel> get_speciefied_levels() const;


private:
	/** obiekt interfejsu użytkownika */
	Ui::GstLoggerProperties *ui;
};

#endif // GSTLOGGERPROPERTIES_H
