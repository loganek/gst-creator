/*
 * ObjectInspectorItem.h
 *
 * Plik zawiera definicję klasy ObjectInspectorItem
 *
 *  Created on: 28 pa? 2013
 *      Author: Marcin Kolny
 */

#ifndef OBJECTINSPECTORITEM_H_
#define OBJECTINSPECTORITEM_H_

#include <string>
#include <vector>

/**
 * Typ fabryki
 */
enum class FactoryType
{
	ELEMENT_FACTORY,
	TYPEFIND_FACTORY,
	UNKNOW_FACTORY
};

/**
 * Klasa reprezentuje pojedynczy obiekt w inspektorze obiektów
 */
class ObjectInspectorItem
{
private:
	/** Tablica dzieci obiektu */
	std::vector<ObjectInspectorItem*> children;
	/** Rodzic obiektu */
	ObjectInspectorItem* parent_item;
	/** typ fabryki */
	FactoryType factory_type;
	/** nazwa obiektu */
	std::string factory_name;

public:
	/**
	 * Konstruktor
	 *
	 * @param name nazwa elementu
	 * @param type typ fabryki
	 * @param parent rodzic obiektu
	 */
	explicit ObjectInspectorItem(const std::string& name,
			FactoryType type = FactoryType::UNKNOW_FACTORY, ObjectInspectorItem *parent = 0);

	/**
	 * Destruktor
	 */
	virtual ~ObjectInspectorItem();

	/**
	 * Dodaje dziecko do obiektu
	 *
	 * @param child obiekt dziecka
	 */
	void append_child(ObjectInspectorItem* child);

	/**
	 * Zwraca obiekt dziecka spod zadanego wiersza
	 *
	 * @param row numer wiersza
	 */
	ObjectInspectorItem *child(int row);

	/**
	 * Zwraca liczbę obiektów podrzędnych
	 *
	 * @returns liczba obiektów podrzędnych
	 */
	int child_count() const;

	/**
	 * Zwraca wiersz obiektu w obiekcie nadrzędnym
	 *
	 * @returns wiersz obiektu w obiekcie nadrzędnym
	 */
	int row() const;

	/**
	 * Zwraca obiekt nadrzędny
	 *
	 * @returns obiekt nadrzędny
	 */
	ObjectInspectorItem *parent();

	/**
	 * Zwraca nazwę obiektu
	 *
	 * @returns nazwa obiektu
	 */
	virtual std::string get_name() const;

	/**
	 * Ustawia typ fabryki
	 *
	 * @param type typ fabryki
	 */
	void set_type(FactoryType type)
	{
		factory_type = type;
	}
};


#endif /* OBJECTINSPECTORITEM_H_ */
