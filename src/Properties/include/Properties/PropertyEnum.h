/*
 * PropertyEnum.h
 *
 * Plik zawiera implementację klasy PropertyEnum
 *
 *  Created on: 10 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef PROPERTYENUM_H_
#define PROPERTYENUM_H_

#include "Property.h"

/**
 * Klasa reprezentuje właściwość typu wyliczeniowego
 */
class PropertyEnum : public Property
{
	Q_OBJECT
private:
	/** Wartość właściwości */
	std::string value;
	/** widget służący do zmiany wartości */
	QComboBox* combobox;
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

	private Q_SLOTS:
	/**
	 * Slot aktualizujący wartość właściwości przy zmianie wartości w obiekcie combobox
	 *
	 * @param pos pozycja obiektu combobox
	 */
	void update_position(int pos);
	public:

	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param parm_spec obiekt specyfikacji parametru
	 * @param element element, do którego należy właściwość
	 * @param value nowa wartość dla właściwości
	 */
	PropertyEnum(GParamSpec* param_spec,
			const Glib::RefPtr<Gst::Element>& element,
			const std::string& value);

	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param parm_spec obiekt specyfikacji parametru
	 * @param element element, do którego należy właściwość
	 */
	PropertyEnum(GParamSpec* param_spec,
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

#endif /* PROPERTYENUM_H_ */
