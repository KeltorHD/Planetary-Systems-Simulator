#pragma once

#include "tinyxml2.h"

class Locale
{
public:
	Locale(const std::string& path); /*инициализация с загрузкой из xml*/

	/*функции*/
	void loadLocaleXml(const std::string& path); /*очистка локализации и загрузка новой*/

	const char* get_c(const std::string& to_locale) const;
	const std::string& get_s(const std::string& to_locale) const;

private:
	std::string lang;
	std::unordered_map<std::string, std::string> locale;
	std::unordered_map<std::string, std::string> en;

	void load(const std::string& path, std::unordered_map<std::string, std::string>& to_load);
};