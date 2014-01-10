/*
 * Command.h
 *
 * Plik zawiera deklarację klasy Command
 * a także pomocniczych typów wyliczeniowych
 *
 *  Created on: 2 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include "utils/StringUtils.h"
#include <stdexcept>
#include <vector>
#include <gstreamermm.h>

class CommandListener;

/**
 * Typ komendy
 */
enum class CommandType
{
	/** Komenda dodająca */
	ADD,
	/** Komenda usuwająca */
	REMOVE,
	/** Komenda tworząca połączenie */
	CONNECT,
	/** Komenda usuwająca połączenie */
	DISCONNECT,
	/** Komenda dotycząca właściwości elementu */
	PROPERTY,
	/** Komenda stanu programu */
	STATE,
};

/**
 * Typ obiektu
 */
enum class ObjectType
{
	/** Gniazdo */
	PAD,
	/** Element */
	ELEMENT
};

/**
 * Typ danych
 */
enum class DataType
{
	INT,
	UINT,
	LONG,
	ULONG,
	INT64,
	UINT64,
	FLOAT,
	DOUBLE,
	STRING,
	BOOLEAN
};

/**
 * Stan programu
 */
enum class State
{
	/** Stan odtwarzania */
	PLAY,
	/** Stan pauzy */
	PAUSE,
	/** Stan stopu */
	STOP
};

/**
 * Klasa reprezentująca komendę
 */
class Command
{
private:
	/**
	 * Typ komendy
	 */
	CommandType type;

public:
	/**
	 * Konstruktor, inicjalizuje pola klasy
	 *
	 * @param type typ komendy
	 */
	Command(CommandType type):type(type){}

	/**
	 * Destruktor
	 */
	virtual ~Command(){}

	/**
	 * Zwraca typ komendy
	 *
	 * @returns typ komendy
	 */
	CommandType get_type()
	{
		return type;
	}

	/**
	 * Uruchamia komendę
	 *
	 * @param listeners tablica obiektów nasłuchujących zmiany wprowadzone po wykonaniu komendy
	 */
	virtual void run_command(std::vector<CommandListener*> listeners = {}) = 0;

	/**
	 * Funkcja rzucająca wyjątkiem błędu składni
	 *
	 * @param error treść błędu
	 */
	static void syntax_error(const std::string& error)
	{
		throw std::runtime_error("Syntax error: " + error);
	}
};

#endif /* COMMAND_H_ */
