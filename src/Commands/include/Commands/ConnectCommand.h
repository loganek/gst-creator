/*
 * ConnectCommand.h
 *
 * W pliku znajduje się deklaracja klasy ConnectCommand
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef CONNECTCOMMAND_H_
#define CONNECTCOMMAND_H_

#include "Command.h"
#include "utils/GstUtils.h"

#include <gstreamermm.h>
#include <set>
#include <utility>

/**
 * Klasa reprezentująca komendę wykonującą połączenie pomiędzy dwoma obiektami
 */
class ConnectCommand : public Command
{
public:
	typedef std::pair<Glib::RefPtr<Gst::Element>, Glib::RefPtr<Gst::PadTemplate>> template_parent;
	typedef std::pair<template_parent, Glib::RefPtr<Gst::Pad>> future_connection_pads;
	typedef std::pair<Glib::RefPtr<Gst::Element>, Glib::RefPtr<Gst::Element>> future_connection_elements;
private:
	/** Obiekt źródłowy */
	Glib::RefPtr<Gst::Object> src;
	/** Obiekt docelowy */
	Glib::RefPtr<Gst::Object> dst;
	/** Element nadrzędny obiektu źródłowego */
	Glib::RefPtr<Gst::Element> src_parent;
	/** Flaga określająca, czy tworzone połączenie ma być połączeniem przyszłościowym */
	bool future;
	/** Typ łączonych obiektów */
	ObjectType type;

	/** Mapa połączeń przyszłościowych pomiędzy gniazdami */
	static std::set<future_connection_pads> future_connection_pads_map;
	/** Mapa połączeń przyszłościowych pomiędzy elementami */
	static std::set<future_connection_elements> future_connection_elements_map;

	/** słowo-klucz określające połączenie przyszłościowe */
	static std::string future_keyword;

	/**
	 * Dodaje połączenie przyszłościowe pomiędzy gniazdami
	 *
	 * @param src_parent element nadrzędny dla gniazda źródłowego
	 * @param src_pad szablon gniazda źródłowego
	 * @param sink_pad gniazdo docelowe
	 */
	void connect_future_pads(const Glib::RefPtr<Gst::Element>& src_parent, const Glib::RefPtr<Gst::PadTemplate>& src_pad, const Glib::RefPtr<Gst::Pad>& sink_pad);

	/**
	 * Dodaje połączenie przyszłościowe pomiędzy elementami
	 *
	 * @param src element źródłowy
	 * @param sink element docelowy
	 */
	void connect_future_elements(const Glib::RefPtr<Gst::Element>& src, const Glib::RefPtr<Gst::Element>& sink);
public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param src obiekt źródłowy połączenia
	 * @param dst obiekt docelowy połączenia
	 * @param future flaga określająca, czy połączenie jest połączeniem przyszłościowym
	 */
	ConnectCommand(const Glib::RefPtr<Gst::Object>& src, const Glib::RefPtr<Gst::Object>& dst, bool future = false);

	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param src szablon gniazda źródłowego
	 * @param src_parent element nadrzędny gniazda źródłowego
	 * @param dst gniazdo docelowe
	 */
	ConnectCommand(const Glib::RefPtr<Gst::PadTemplate>& src, const Glib::RefPtr<Gst::Element>& src_parent, const Glib::RefPtr<Gst::Pad>& dst);

	/**
	 * Zwraca typ łączonych obiektów
	 *
	 * @returns typ łączonych obiektów
	 */
	ObjectType get_type() { return type; }

	/**
	 * Zwraca obiekt źródłowy połączenia
	 *
	 * @returns obiekt źródłowy połączenia
	 */
	Glib::RefPtr<Gst::Object> get_src() { return src; }

	/**
	 * Zwraca obiekt docelowy połączenia
	 *
	 * @returns obiekt docelowy połączenia
	 */
	Glib::RefPtr<Gst::Object> get_dst() { return dst; }

	/**
	 * Tworzy obiekt ConnectCommand na podstawie listy argumentów
	 *
	 * @param args lista argumentów
	 * @param model model programu GStreamer
	 */
	static ConnectCommand* from_args(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 *  Tworzy obiekt ConnectCommand na podstawie obiektu Linkage
	 *
	 * @param lnk obiekt określający połączenie
	 * @param listeners obiekty nasłuchujące zmian w modelu programu
	 */
	static ConnectCommand* from_linkage(const Linkage& lnk, std::vector<CommandListener*> listeners);

	/**
	 * Zwraca listę podpowiedzi następnego słowa dla zadanej listy argumentów
	 *
	 * @param args lista argumentów
	 * @param model model programu GStreamer
	 *
	 * @returns lista podpowiedzi
	 */
	static std::vector<std::string> get_suggestions(const std::vector<std::string>& args, const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Uruchamia komendę
	 *
	 * @param listeners tablica obiektów nasłuchujących zmiany wprowadzone po wykonaniu komendy
	 */
	void run_command(std::vector<CommandListener*> listeners = {});

	/**
	 * Funkcja wykorzystywana do reakcji na dodanie nowego gniazda
	 *
	 * @param pad nowe gniazdo
	 */
	static void element_pad_added(const Glib::RefPtr<Gst::Pad>& pad);

	/**
	 * Usuwa połączenie przyszłościowe
	 *
	 * @param tpl szablon gniazda źródłowego
	 * @param pad gniazdo docelowe
	 * @param listeners tablica obiektów nasłuchujących zmiany modelu programu
	 */
	static void remove_future_connection(const Glib::RefPtr<Gst::PadTemplate>& tpl, const Glib::RefPtr<Gst::Pad>& pad, std::vector<CommandListener*> listeners = {});

	/**
	 * Zwraca zbiór połączeń przyszłościowych pomiędzy elementami
	 *
	 * @returns zbiór połączeń przyszłościowych pomiędzy elementami
	 */
	static std::set<future_connection_elements> get_future_connections_element();

	/**
	 * Zwraca zbiór połączeń przyszłościowych pomiędzy gniazdami
	 *
	 * @returns zbiór połączeń przyszłościowych pomiędzy gniazdami
	 */
	static std::set<future_connection_pads> get_future_connections_pads();
};

#endif /* CONNECTCOMMAND_H_ */
