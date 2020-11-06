#include "pch.h"
#include "graphicsSettings.hpp"

GraphicsSettings::GraphicsSettings()
{
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->verticalSync = false;
	this->framerateLimit = 120u;
	this->antialiasingLevel = 0u;
}

void GraphicsSettings::saveXml(const std::string path)
{
	throw "не реализовано";
}

void GraphicsSettings::loadXml(const std::string path)
{
	using namespace tinyxml2;
	XMLDocument file;
	if (file.LoadFile(path.c_str()) == XMLError::XML_SUCCESS)
	{
		auto ref = file.FirstChildElement("settings");
		this->resolution.width = ref->FirstChildElement("resolution")->FirstChildElement("width")->UnsignedText();
		this->resolution.height = ref->FirstChildElement("resolution")->FirstChildElement("height")->UnsignedText();
		this->fullscreen = ref->FirstChildElement("fullscreen")->BoolText();
		this->framerateLimit = ref->FirstChildElement("framerate_limit")->UnsignedText();
		this->verticalSync = ref->FirstChildElement("vertical_sync")->BoolText();
		this->antialiasingLevel = ref->FirstChildElement("antialiasinc_level")->UnsignedText();
		this->lang = ref->FirstChildElement("lang")->GetText();
	}
	else
	{
		throw "NOT LOAD LOCALE FILE " + path;
	}
}