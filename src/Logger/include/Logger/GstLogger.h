/*
 * GstLogger.h
 *
 * Plik zawiera definicję klasy GstLogger
 *
 *  Created on: 5 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTLOGGER_H
#define GSTLOGGER_H

#include <QtWidgets>
#include <gstreamermm.h>
#include <mutex>

namespace Ui {
class GstLogger;
}

/**
 * Klasa reprezentuje widget logujący zdarzenia pochodzące z biblioteki GStreamer
 */
class GstLogger : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Konstruktor
	 *
	 * @param parent nadrzędny widget
	 */
	explicit GstLogger(QWidget *parent = 0);

	/**
	 * Destruktor
	 */
	~GstLogger();

	/**
	 * Loguje zdarzenie, wyświetlając je w oknie
	 *
	 * @param category kategoria
	 * @param level poziom zdarzenia
	 * @param file plik, z którego pochodzi zdarzenie
	 * @param function funkcja, z której pochodzi zdarzenie
	 * @param line linia, z której pochodzi zdarzenie
	 * @param obiekt, na rzecz którego wywołano zdarzenie
	 * @param message wiadomość zdarzenia
	 */
	void add_log(GstDebugCategory * category,
			GstDebugLevel      level,
			const gchar      * file,
			const gchar      * function,
			gint               line,
			GObject          * object,
			GstDebugMessage  * message);

private:
	/** obiekt interfejsu użytkownika */
	Ui::GstLogger *ui;
	/** mutex uniemożliwiający jednoczesnego odwołania się dwóch wątków do GUI */
	std::mutex widget_locker;
	/** Widgety sterujące konfiguracją */
	std::vector<QCheckBox*> config_checkboxes;

	void set_if_checked(int row, int col, const QString& value);
};

#endif // GSTLOGGER_H
