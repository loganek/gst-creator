/*
 * StringUtils.h
 *
 * Plik zawiera deklarację klasy StringUtils
 *
 *  Created on: 4 lis 2013
 *      Author: Marcin Kolny
 */

#ifndef STRINGUTILS_H_
#define STRINGUTILS_H_

#include <vector>
#include <string>
#include <sstream>


/**
 * Klasa pomocnicza, rozszerzająca możliwości klasy std::string
 */
class StringUtils
{
public:
	/**
	 * Funkcja rozbija ciąg znaków na tablicę elementów, korzystając z podanego separatora
	 *
	 * @param text tekst do rozbicia
	 * @param delim separator
	 *
	 * @returns tablica rozdzielonych podciągów
	 */
	static std::vector<std::string> split(std::string text,
			const std::string& delim);

	/**
	 * Funkcja łączy tablicę znaków, używając do łączenia, podanego separatora
	 *
	 * @param arr tablica ciągów
	 * @param separator separator
	 *
	 * @returns połączony ciąg znaków
	 */
	static std::string join(const std::vector<std::string>& arr,
			const std::string& separator);

	/**
	 * Usuwa białe znaki na początku i końcu łańcucha znakowego
	 *
	 * @param text łańcuch znaków, z którego usuwane będą białe znaki
	 *
	 * @returns przetworzony tekst
	 */
	static std::string trim(std::string text);

	/**
	 * Zamienia wszystkie litery w łańcuchu znaków na wielkie
	 *
	 * @param text łańcuch do przetworzenia
	 *
	 * @returns przetworzony łańcuch znaków
	 */
	static std::string to_upper(std::string text);

	/**
	 * Zamienia wszystkie litery w łańcuchu znaków na małe
	 *
	 * @param text łańcuch do przetworzenia
	 *
	 * @returns przetworzony łańcuch znaków
	 */
	static std::string to_lower(std::string text);

	/**
	 * Sprawdza, czy podany ciąg znaków jest liczbą
	 *
	 * @param s ciąg znaków
	 *
	 * @returns true, jeśli ciąg jest liczbą
	 */
	static bool is_number(const std::string& s);

	/**
	 * Sprawdza, czy dwa podane ciągi znaków są takie same, nie uwzględniając wielkości liter
	 *
	 * @param s1 pierwszy ciąg
	 * @param s2 drugi ciąg
	 *
	 * @returns true, jeśli ciągi są takie same, nie uwzględniając wielkości liter
	 */
	static bool are_equal_case_no_sense(const std::string& s1, const std::string& s2);

	/**
	 * Konwertuje ciąg znaków na typ numeryczny
	 *
	 * @param value ciąg znaków
	 *
	 * @returns wartość numeryczna utworzona na podstawie ciągu znaków
	 */
	template<typename T>
	static T str_to_numeric(const std::string& value)
	{
		std::stringstream ss;
		T val;
		ss << value;
		ss >> val;
		return val;
	}
};

#endif /* STRINGUTILS_H_ */
