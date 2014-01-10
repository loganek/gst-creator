/*
 * PropertyString.h
 *
 * Plik zawiera deklarację klasy PropertyString
 *
 *  Created on: 9 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYSTRING_H_
#define PROPERTYSTRING_H_

#include "Property.h"

/**
 * Klasa reprezentuje właściwość typu łańcucha znaków
 */
class PropertyString : public Property
{
	Q_OBJECT
private:
	/** Wartość właściwości */
	std::string value;
	/** widget służący do zmiany wartości */
	QLineEdit* edit;

	public Q_SLOTS:
	/**
	 * Slot aktualizujący wartość
	 */
	void update_value(const QString& new_value);

	protected:
	/**
	 * Funkcja buduje widget
	 */
	virtual void build_widget();

	/**
	 * Funkcja inicjalizuje obiekt właściwości
	 */
	virtual void init();

	/**
	 * Funkcja odczytuje wartość właściwości
	 */
	void read_var();

	public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param parm_spec obiekt specyfikacji parametru
	 * @param element element, do którego należy właściwość
	 * @param value nowa wartość dla właściwości
	 */
	PropertyString(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);

	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param parm_spec obiekt specyfikacji parametru
	 * @param element element, do którego należy właściwość
	 */
	PropertyString(GParamSpec* param_spec,
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

#endif /* PROPERTYSTRING_H_ */
