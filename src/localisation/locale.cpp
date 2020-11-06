#include "pch.h"
#include "locale.hpp"

Locale::Locale(const std::string& path)
{
	this->load(path, this->locale);

	/*текущая локализация*/
	this->lang = {path.substr(path.find_last_of('/') + 1, path.size() - path.find_last_of('/') - 5)};

	if (this->lang != "en") /*загрузка английской локализации*/
	{
		this->load(path.substr(0, path.length() - 4 - this->lang.length()) + "en.xml",  this->en);
	}
}

void Locale::loadLocaleXml(const std::string& path)
{
	this->load(path, this->locale);

	/*текущая локализация*/
	this->lang = { path.substr(path.find_last_of('/') + 1, path.size() - path.find_last_of('/') - 5) };

	if (this->lang != "en") /*загрузка английской локализации*/
	{
		this->load(path.substr(0, path.length() - 4 - this->lang.length()) + "en.xml", this->en);
	}
}

const std::string& Locale::get_s(const std::string& to_locale) const
{
	try /*если не найдется теста в локализации*/
	{
		return this->locale.at(to_locale);
	}
	catch (...) /*используем английскую*/
	{
		return this->en.at(to_locale);
	}
}

const char* Locale::get_c(const std::string& to_locale) const
{
	try /*если не найдется теста в локализации*/
	{
		return this->locale.at(to_locale).c_str();
	}
	catch (...) /*используем английскую*/
	{
		return this->en.at(to_locale).c_str();
	}
}

void Locale::load(const std::string& path, std::unordered_map<std::string, std::string>& to_load)
{
	to_load.clear(); /*очистка локализации*/

	using namespace tinyxml2;
	XMLDocument locale;
	if (locale.LoadFile(path.c_str()) == XMLError::XML_SUCCESS)
	{
		/*имя программы*/
		auto ref{ locale.FirstChildElement("text") };
		to_load[ref->FirstChildElement("program_title")->Value()] = ref->FirstChildElement("program_title")->GetText();

		/*локализация меню*/
		for (auto object = ref->FirstChildElement("menu")->FirstChildElement(); object != nullptr; object = object->NextSiblingElement())
		{
			to_load[object->Value()] = object->GetText();
		}

		/*локализация режима симуляции*/
		for (auto object = ref->FirstChildElement("simulation")->FirstChildElement(); object != nullptr; object = object->NextSiblingElement())
		{
			to_load[object->Value()] = object->GetText();
		}
	}
	else
	{
		throw "NOT LOAD LOCALE FILE " + path;
	}
}
