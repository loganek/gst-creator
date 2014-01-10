/*
 * WorkspaceWidget.h
 *
 * Plik zawiera deklarację klasy WorkspaceWidget
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef WORKSPACEWIDGET_H_
#define WORKSPACEWIDGET_H_

#include "Commands.h"
#include "qnelibrary.h"
#include <QWidget>
#include <QMimeData>
#include <gstreamermm.h>

/**
 * Klasa reprezentuje widget obszaru roboczego
 */
class WorkspaceWidget : public QWidget, public CommandListener
{
	Q_OBJECT
private:
	/** kontroler programu */
	CommandListener* controller;
	/** obiekt widoku obszaru roboczego */
	QGraphicsView* view;
	/** scena obszaru roboczego */
	QGraphicsScene* scene;
	/** obiekt aktywnego połączenia */
	QNEConnection* current_connection;
	/** model programu GStreamer */
	Glib::RefPtr<Gst::Pipeline> model;

	/**
	 * Sprawdza, czy nagłówek MIME jest zgodny z programem
	 *
	 * @param mime_data nagłówek mime do sprawdzenia
	 *
	 * @returns true jeśli nagłówek jest zgodny
	 */
	bool check_mime_data(const QMimeData* mime_data) const;

	/**
	 * Funkcja generuje okno z zapytaniem o nazwę nowego elementu
	 *
	 * @param name proponowana nazwa elementu
	 *
	 * @returns nazwa podana przez użytkownika
	 */
	QString get_new_name(const QString& name);

	/**
	 * Zwraca obiekt znajdujący się na zadanej współrzędnej
	 *
	 * @param pos pozycja elementu
	 *
	 * @returns obiekt znajdujący się pod wskazaną pozycją
	 */
	QGraphicsItem* item_at(const QPointF &pos);

	/**
	 * Filtr zdarzeń
	 */
	bool eventFilter(QObject *o, QEvent *e);

	/**
	 * Znajduje obiekt portu reprezentującego dane gniazdo
	 *
	 * @param pad gniazdo
	 *
	 * @returns port reprezentujący dane gniazdo
	 */
	QNEPort* find_port(const Glib::RefPtr<Gst::Pad>& pad);

	/**
	 * Znajduje obiekt portu reprezentującego dany szablon gniazda
	 *
	 * @param pad szablon gniazda
	 * @param element nadrzędny
	 *
	 * @returns port reprezentujący dany szablon gniazda
	 */
	QNEPort* find_port(const Glib::RefPtr<Gst::PadTemplate>& pad, const Glib::RefPtr<Gst::Element>& parent);

	/**
	 * Znajduje obiekt bloku, reprezentującego dany element
	 *
	 * @param element element
	 *
	 * @returns znaleziony blok
	 */
	QNEBlock* find_block(const Glib::RefPtr<Gst::Element>& element);

public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param model model programu GStreamer
	 * @parem parent widget nadrzędny
	 */
	explicit WorkspaceWidget(const Glib::RefPtr<Gst::Pipeline>& model, QWidget* parent = 0);

	/**
	 * Destruktor
	 */
	virtual ~WorkspaceWidget();

	/**
	 * Obsługa zdarzenia zmiany rozmiaru widgeta
	 */
	void resizeEvent(QResizeEvent * event);

	/****** Implementacja interfejsu CommandListener ******/
	void new_element_added(const Glib::RefPtr<Gst::Element>& element);
	void element_removed(const Glib::RefPtr<Gst::Element>& element);
	void pad_added(const Glib::RefPtr<Gst::Pad>& pad);
	void pad_linked(const Glib::RefPtr<Gst::Pad>& pad);
	void pad_removed(const Glib::RefPtr<Gst::Pad>& pad);
	void pad_unlinked(const Glib::RefPtr<Gst::Pad>& pad);
	void future_connection_added(const Glib::RefPtr<Gst::PadTemplate>& src_tpl,
			const Glib::RefPtr<Gst::Element>& parent, const Glib::RefPtr<Gst::Pad>& sink);
	void future_connection_removed(const ConnectCommand::future_connection_pads& conn);
	/****** Koniec implementacji interfejsu CommandListener ******/

	/**
	 * Ustawia kontroler dla widoku
	 *
	 * @param controller kontroler
	 */
	void set_controller(CommandListener* controller);

	/**
	 * Zwraca pozycję bloku zawierającego dany element
	 *
	 * @param element element bloku
	 *
	 * @returns lokalizacja bloku
	 */
	QPointF get_block_location(const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Ustawia lokalizację bloku zawierającego dany element
	 *
	 * @param element element bloku
	 * @param x pozycja X
	 * @param y pozycja Y
	 */
	void set_block_location(const Glib::RefPtr<Gst::Element>& element, double x, double y);

	Q_SIGNALS:
	/**
	 * sygnał zmiany aktualnie wybranego elementu
	 *
	 * @param aktualny element
	 */
	void current_element_changed(const Glib::RefPtr<Gst::Element>& element);
};

#endif /* WORKSPACEWIDGET_H_ */
