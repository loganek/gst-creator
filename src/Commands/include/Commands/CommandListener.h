/*
 * CommandListener.h
 *
 * W pliku znajduje się deklaracja klasy CommandListener
 *
 *  Created on: 1 gru 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMANDLISTENER_H_
#define COMMANDLISTENER_H_

#include "ConnectCommand.h"
#include <gstreamermm.h>

/**
 * Klasa przedstawia interfejs, jaki powinien spełniać
 * obiekt nasłuchujący zmian w modelu programu.
 *
 * Metody są zaimplementowane, klasa dziedzicząca może implementować
 * tylko potrzebne metody.
 */
class CommandListener
{
protected:
	/** liczba utworzonych obiektów nasłuchujących */
	static int refcount;
public:
	/**
	 * Funkcja zostaje wywołana, gdy gniazdo zostanie dodane
	 *
	 * @param pad dodane gniazdo
	 */
	virtual void pad_added(const Glib::RefPtr<Gst::Pad>& pad) {}
	/**
	 * Funkcja zostaje wywołana, gdy gniazdo zostanie usunięte
	 *
	 * @param pad usunięte gniazdo
	 */
	virtual void pad_removed(const Glib::RefPtr<Gst::Pad>& pad){}
	/**
	 * Funkcja zostaje wywołana, gdy gniazdo zostanie połączone
	 *
	 * @param pad połączone gniazdo
	 */
	virtual void pad_linked(const Glib::RefPtr<Gst::Pad>& pad){}
	/**
	 * Funkcja zostaje wywołana, gdy gniazdo zostanie rozłączone
	 *
	 * @param pad rozłączone gniazdo
	 */
	virtual void pad_unlinked(const Glib::RefPtr<Gst::Pad>& pad){}
	/**
	 * Funkcja zostaje wywołana, gdy połączenie przyszłościowe zostanie dodane
	 *
	 * @param src_tpl szablon gniazda źródłowego
	 * @param parent rodzic gniazda źródłowego
	 * @param sink gniazdo docelowe
	 */
	virtual void future_connection_added(const Glib::RefPtr<Gst::PadTemplate>& src_tpl,
			const Glib::RefPtr<Gst::Element>& parent, const Glib::RefPtr<Gst::Pad>& sink){}
	/**
	 * Funkcja zostaje wywołana, gdy połączenie przyszłościowe zostanie usunięte
	 *
	 * @param conn para gniazd połączenia
	 */
	virtual void future_connection_removed(const ConnectCommand::future_connection_pads& conn){}
	/**
	 * Funkcja zostaje wywołana, gdy stan modelu programu zostanie zmieniony
	 *
	 * @param state nowy stan modelu programu
	 */
	virtual void state_changed(State state){}
	/**
	 * Zwraca liczbę obiektów nasłuchujących
	 *
	 * @param refcount liczba obiektów nasłuchujących
	 */
	static int get_refcount() { return refcount; }
	/**
	 * Destruktor
	 */
	virtual ~CommandListener(){}
};

#endif /* COMMANDLISTENER_H_ */
