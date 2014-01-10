/*
 * PropertyCaps.h
 *
 * Plik zawiera implementację klasy PropertyCaps
 *
 *  Created on: 10 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYCAPS_H_
#define PROPERTYCAPS_H_

#include "Property.h"

/**
 * Klasa reprezentująca obiekt właściwości typu Gst::Caps
 */
class PropertyCaps : public Property
{
private:
	/** Wartość właściwości */
	Glib::RefPtr<Gst::Caps> value;
	/** zmienna pomocnicza */
	Glib::ustring tmp_value;
	/** widget służący do zmiany wartości */
	QLineEdit* caps_edit;

	/**
	 * Funkcja odczytuje wartość właściwości
	 */
	void read_var();

protected:
	/**
	 * Funkcja buduje widget
	 */
	virtual void build_widget();

	/**
	 * Funkcja inicjalizuje obiekt właściwości
	 */
	virtual void init();
public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param parm_spec obiekt specyfikacji parametru
	 * @param element element, do którego należy właściwość
	 * @param value nowa wartość dla właściwości
	 */
	PropertyCaps(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const Glib::RefPtr<Gst::Caps>& value);

	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param parm_spec obiekt specyfikacji parametru
	 * @param element element, do którego należy właściwość
	 */
	PropertyCaps(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Zwraca aktualną wartosć właściwości jako obiekt typu string
	 *
	 * @returns wartość właściwości jako obiekt typu string
	 */
	std::string get_str_value() const;

	/**
	 * Ustawia wcześniej zadaną wartość właściwości
	 */
	void set_value();

	/**
	 * Sprawdza, czy aktualna wartość właściwości, jest wartością domyślną
	 *
	 * @returns true, jeśli aktualna wartość jest wartością domyślną
	 */
	bool is_default_value() const;

	/**
	 * Zwraca nazwę typu właściwości
	 *
	 * @returns nazwę type właściwości
	 */
	std::string get_type_name() const;
};

#endif /* PROPERTYCAPS_H_ */
