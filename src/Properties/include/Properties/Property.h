/*
 * Property.h
 *
 * Plik zawiera deklarację klasy Property
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <gstreamermm.h>
#include <QtWidgets>
#include <memory>

/**
 * Klasa reprezentująca obiekt właściwości
 */
class Property : public QObject
{
private:
	/** określa, czy widget umożliwiający edycję właściwości, został zbudowany */
	bool was_built;
protected:
	/** obiekt specyfikacji parametru */
	GParamSpec* param_spec;
	/** element, do którego należy dana właściwość */
	Glib::RefPtr<Gst::Element> element;
	/** Widget umożliwiający edycję właściwości */
	QWidget* widget;

	/**
	 * Funkcja buduje widget
	 */
	virtual void build_widget() = 0;

	/**
	 * Funkcja inicjalizuje obiekt właściwości
	 */
	virtual void init() = 0;

	/**
	 * Funkcja buduje właściwość numeryczną
	 *
	 * @param param_spec specyfikacja parametru
	 * @param element element, do którego należy właściwość
	 * @param type dokładny typ właściwości
	 * @param value nowa wartość właściwości
	 *
	 * @returns obiekt właściwości
	 */
	static Property* build_numeric_property(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element, GType type,
			const std::string& value);
public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param parm_spec obiekt specyfikacji parametru
	 * @param element element, do którego należy właściwość
	 */
	Property(GParamSpec* param_spec, const Glib::RefPtr<Gst::Element>& element);

	/**
	 * Destruktor
	 */
	virtual ~Property();

	/**
	 * Zwraca widget pozwalający na edycję właściwości
	 *
	 * @returns widget pozwalający na edycję właściwości
	 */
	QWidget* get_widget();

	/**
	 * Ustawia wcześniej zadaną wartość właściwości
	 */
	virtual void set_value() = 0;

	/**
	 * Zwraca aktualną wartosć właściwości jako obiekt typu string
	 *
	 * @returns wartość właściwości jako obiekt typu string
	 */
	virtual std::string get_str_value() const = 0;

	/**
	 * Sprawdza, czy aktualna wartość właściwości, jest wartością domyślną
	 *
	 * @returns true, jeśli aktualna wartość jest wartością domyślną
	 */
	virtual bool is_default_value() const = 0;

	/**
	 * Zwraca nazwę typu właściwości
	 *
	 * @returns nazwę type właściwości
	 */
	virtual std::string get_type_name() const = 0;

	/**
	 * Sprawdza, czy właściwość można edytować
	 *
	 * @returns true, jeśli właściwość może być edytowana
	 */
	bool is_writable() const;

	/**
	 * Funkcja buduje obiekt typu Property
	 *
	 * @param param_spec obiekt specyfikacji parametru
	 * @param element element, do którego należy właściwość
	 * @param value nowa wartość właściwości
	 *
	 * @returns obiekt właściwości
	 */
	static Property* build_property(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);

	/**
	 * Buduje widget zawierający listę widgetów umożliwiających edycję właściwości danego elementu
	 *
	 * @param element element
	 *
	 * @returns widget umożliwiający edycję właściwości
	 */
	static QWidget* build_property_window(
			const Glib::RefPtr<Gst::Element>& element);
};

#endif /* PROPERTY_H_ */
