/*
 * ConsoleView.h
 *
 * Plik zawiera deklarację klasy ConsoleView
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef CONSOLEVIEW_H_
#define CONSOLEVIEW_H_

#include "Console/CommandParser.h"
#include <QWidget>
#include <QtWidgets>
#include <gstreamermm.h>
#include <memory>

/**
 * Klasa reprezentuje komponent konsoli komend
 */
class ConsoleView : public QWidget
{
	Q_OBJECT
private:
	/** obiekt edytora tekstu */
	QLineEdit* edit;
	/** obiekt przycisku */
	QPushButton* button;
	/** obiekt drzewa przedstawiający listę podpowiedzi dla użytkownika */
	QTreeWidget* suggestions;
	/** tablica obiektów nasłuchujących zmiany modelu programu */
	std::vector<CommandListener*> listeners;
	/** model programu GStreamer */
	Glib::RefPtr<Gst::Pipeline> model;

	/** obiekt parsera */
	CommandParser* parser;

	/** lista argumentów bieżącej komendy */
	std::vector<std::string> command_args;

	/**
	 * Zwraca listę aktualnych podpowiedzi na podstawie listy argumentów
	 *
	 * @param command_args lista argumentów
	 *
	 * @returns lista podpowiedzi
	 */
	std::vector<std::string> get_current_suggestions(std::vector<std::string> command_args);

	/**
	 * Generuje widget elementu zawierającego podpowiedzi
	 */
	void make_suggestion_widget();

	/**
	 * Obsługa zdarzeń QT
	 *
	 * @param obj objekt emitujący zdarzenie
	 * @param ev obiekt zdarzenia
	 */
	bool eventFilter(QObject *obj, QEvent *ev);

	/**
	 * Generuje listę sugestii
	 *
	 * @param text tekst, na podstawie którego ma zostać wygenerowana lista podpowiedzi
	 * @param autocomplete określa, czy ma zostać wykonane automatyczne uzupełnianie
	 */
	void make_suggestion(const QString& text, bool autocomplete = false);
private Q_SLOTS:
	/**
	 * Slot wykonania komendy
	 */
	void execute_command();

	/**
	 * Slot dla wykonania sugestii
	 *
	 * @param text tekst dla generatora sugestii
	 */
	void suggest(const QString& text);


Q_SIGNALS:
	void command_added(std::shared_ptr<Command> command);

public:
	explicit ConsoleView(std::vector<CommandListener*> listeners = {}, QWidget* parent = 0);
	virtual ~ConsoleView();
	void set_model(const Glib::RefPtr<Gst::Pipeline>& model);
};

#endif /* CONSOLEVIEW_H_ */
