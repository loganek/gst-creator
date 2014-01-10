/*
 * LoggerView.h
 *
 * Plik zawiera deklarację klasy LoggerView
 *
 *  Created on: 5 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef LOGGERVIEW_H_
#define LOGGERVIEW_H_

#include <QtWidgets>
#include <memory>
#include <gstreamermm.h>
#include "Commands/Command.h"

/**
 * Klasa reprezentująca widget loggera dla zdarzeń aplikacji
 */
class LoggerView : public QWidget
{
	Q_OBJECT

private:
	/** główna tabela wyświetlająca logi */
	QTableWidget* table;
	/** widget filtrujący logi komend */
	QCheckBox* cmd_cbox;
	/** widget filtrujący logi wiadomości magistrali */
	QCheckBox* msg_cbox;

	/**
	 * Dodaje pojedynczy log do tabeli
	 *
	 * @param text treść loga
	 */
	void add_single_log(const QString& text);

	public Q_SLOTS:
	/**
	 * Slot dodawania loga typu Command
	 *
	 * @param cmd komenda, jaka została wykonana
	 */
	void add_log(std::shared_ptr<Command> cmd);

	/**
	 * Slot dodawania loga typu Bus Message
	 *
	 * @param bus magistrala
	 * @param message wiadomość
	 */
	bool add_bus_log(const Glib::RefPtr<Gst::Bus>& bus, const Glib::RefPtr<Gst::Message>& message);

	public:
	/**
	 * Konstruktor
	 *
	 * @param parent rodzic okna dialogowego
	 */
	explicit LoggerView(QWidget* parent = 0);

	/**
	 * Destruktor
	 */
	virtual ~LoggerView();
};


#endif /* LOGGERVIEW_H_ */
