/*
 * GstUtils.h
 *
 * Plik zawiera deklarację klasy GstUtils oraz struktury Linkage
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef GSTUTILS_H_
#define GSTUTILS_H_

#include <gstreamermm.h>
#include <vector>


/**
 * Struktura pomocnicza, przechowuje informacje o połączeniu
 */
struct Linkage
{
	/** określa, czy połączenie istnieje */
	bool exists;
	/** obiekt źródłowy */
	Glib::RefPtr<Gst::Object> src;
	/** obiekt docelowy */
	Glib::RefPtr<Gst::Object> sink;
	/** rodzic obiektu źródłowego */
	Glib::RefPtr<Gst::Object> src_parent;
	/** rodzic obiektu docelowego */
	Glib::RefPtr<Gst::Object> sink_parent;
};

/**
 * Klasa pomocnicza, rozszerzająca funkcjonalność biblioteki GStreamer
 */
class GstUtils
{
private:
	/**
	 * Sprawdza, czy element zawiera gniazdo o wskazanym kierunku
	 *
	 * @param element element, który zostanie sprawdzony
	 * @param dir kierunek gniazda
	 *
	 * @returns true, jeśli gniazdo o podanym kierunku istnieje
	 */
	static bool is_specified_dir_element(const Glib::RefPtr<Gst::Element>& element, Gst::PadDirection dir);
public:

	/**
	 * Funkcja znajduje element w modelu, na podstawie opisu
	 *
	 * @param text opis
	 * @param model model programu GStreamer
	 *
	 * @returns obiekt elementu
	 */
	static Glib::RefPtr<Gst::Element> find_element(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Funkcja znajduje gniazdo w modelu, na podstawie opisu
	 *
	 * @param text opis
	 * @param model model programu GStreamer
	 *
	 * @returns obiekt gniazda
	 */
	static Glib::RefPtr<Gst::Pad> find_pad(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Funkcja znajduje szablon w modelu, na podstawie opisu
	 *
	 * @param text opis
	 * @param model model programu GStreamer
	 *
	 * @returns obiekt szablonu
	 */
	static Glib::RefPtr<Gst::PadTemplate> find_pad_template(std::string text, const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Funkcja sprawdza, czy dany typ, jest typem numerycznym
	 *
	 * @param type typ do sprawdzenia
	 *
	 * @returns true, jeśli typ jest typem numerycznym
	 */
	static bool is_numeric_type(GType type);

	/**
	 * Funkcja zwraca listę dostępnych elementów
	 *
	 * @returns lista dostępnych elementów
	 */
	static std::vector<std::string> get_avaliable_elements_string();

	/**
	 * Funkcja zwraca listę dostępnych elementów w kontenerze jako lista opisów
	 *
	 * @param bin kontener
	 * @param must_bin flaga określająca, czy znajdowany element musi być kontenerem
	 * @param prefix prefiks opisu
	 *
	 * @returns opis elementu
	 */
	static std::vector<std::string> get_elements_from_bin_string(const Glib::RefPtr<Gst::Bin>& bin, bool must_bin, const std::string& prefix = "");

	/**
	 * Funkcja zwraca listę nazw właściwości danego elementu
	 *
	 * @param element element
	 *
	 * @returns lista nazw właściwości
	 */
	static std::vector<std::string> get_properties_string(const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Zwraca listę nazw dostępnych szablonów
	 *
	 * @param element element, w którym przeszukiwane będą gniazda
	 * @param recursive określa, czy przeszukiwanie ma być rekursywne
	 * @param prefix prefiks opisu
	 *
	 * @returns lista nazw dostępnych szablonów
	 */
	static std::vector<std::string> get_avaliable_pad_templates_string(const Glib::RefPtr<Gst::Element>& element, bool recursive = false, const std::string& prefix = "");

	/**
	 * Zwraca listę nazw gniazd w kontenerze
	 *
	 * @param bin kontener, z którego odczytywana jest lista gniazd
	 * @param prefix prefiks opisu
	 *
	 * @returns lista nazw gniazd
	 */
	static std::vector<std::string> get_pads_from_bin_string(const Glib::RefPtr<Gst::Bin>& bin, const std::string& prefix = "");

	/**
	 * Zwraca listę nazw gniazd w elemencie
	 *
	 * @param element element, z którego odczytywana jest lista gniazd
	 *
	 * @returns lista nazw gniazd
	 */
	static std::vector<std::string> get_pads_from_element_string(const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Sprawdza, czy element posiada gniazda ujściowe
	 *
	 * @returns true, jeśli element posiada gniazda ujściowe
	 */
	static bool is_sink_element(const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Sprawdza, czy element posiada gniazda źródłowe
	 *
	 * @returns true, jeśli element posiada gniazda źródłowe
	 */
	static bool is_src_element(const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Funkcja generuje ścieżkę dla elementu
	 *
	 * @param obj obiekt, dla którego generowana będzie ścieżka
	 * @param max_parent obiekt nadrzędny, na którym kończyć się będzie generowana ścieżka
	 *
	 * @returns ścieżka elementu
	 */
	static std::string generate_element_path(Glib::RefPtr<Gst::Object> obj, const Glib::RefPtr<Gst::Object>& max_parent = Glib::RefPtr<Gst::Object>());

	/**
	 * Funkcja czyści model programu, usuwając wszystkie elementy
	 *
	 * @param model model programu GStreamer
	 */
	static void clean_model(const Glib::RefPtr<Gst::Pipeline>& model);

	/**
	 * Funkcja tworzy obiekt połączenia
	 *
	 * @param element źródłowy
	 * @param element docelowy
	 *
	 * @returns obiekt połączenia
	 */
	static Linkage find_connection(Glib::RefPtr<Gst::Element> source, Glib::RefPtr<Gst::Element> destination);

	/**
	 * Funkcja tworzy obiekt połączenia
	 *
	 * @param element źródłowy
	 * @param gniazdo docelowe
	 *
	 * @returns obiekt połączenia
	 */
	static Linkage find_connection(Glib::RefPtr<Gst::Element> source, Glib::RefPtr<Gst::Pad> dst_port);

	/**
	 * Funkcja tworzy obiekt połączenia
	 *
	 * @param gniazdo źródłowe
	 * @param element docelowy
	 *
	 * @returns obiekt połączenia
	 */
	static Linkage find_connection(Glib::RefPtr<Gst::Pad> src_pad, Glib::RefPtr<Gst::Element> destination);

	/**
	 * Funkcja tworzy obiekt połączenia
	 *
	 * @param element źródłowy
	 * @param szablon docelowy
	 *
	 * @returns obiekt połączenia
	 */
	static Linkage find_connection(Glib::RefPtr<Gst::Element> source, Glib::RefPtr<Gst::PadTemplate> dst_tpl);

	/**
	 * Funkcja tworzy obiekt połączenia
	 *
	 * @param szablon źródłowy
	 * @param element docelowy
	 *
	 * @returns obiekt połączenia
	 */
	static Linkage find_connection(Glib::RefPtr<Gst::PadTemplate> src_tpl, Glib::RefPtr<Gst::Element> destination);

	/**
	 * Funkcja tworzy obiekt połączenia
	 *
	 * @param obiekt źródłowy
	 * @param obiekt docelowy
	 *
	 * @returns obiekt połączenia
	 */
	static Linkage find_connection(Glib::RefPtr<Gst::Object> source, Glib::RefPtr<Gst::Object> destination);
};

#endif /* GSTUTILS_H_ */
