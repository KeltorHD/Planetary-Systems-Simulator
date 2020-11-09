#include "pch.h"
#include "graphicsSettings.hpp"

GraphicsSettings::GraphicsSettings()
{
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->videoModes = sf::VideoMode::getFullscreenModes();
	this->verticalSync = false;
	this->framerateLimit = 120u;
	this->antialiasingLevel = 0u;
	this->fontSize = 0u;
}

void GraphicsSettings::saveXml(const std::string path)
{
	using namespace tinyxml2;
	XMLDocument file;
	if (file.LoadFile(path.c_str()) == XMLError::XML_SUCCESS)
	{
		auto ref = file.FirstChildElement("settings");
		ref->FirstChildElement("resolution")->FirstChildElement("width")->SetText(this->resolution.width);
		ref->FirstChildElement("resolution")->FirstChildElement("height")->SetText(this->resolution.height);
		ref->FirstChildElement("fullscreen")->SetText(this->fullscreen);
		ref->FirstChildElement("framerate_limit")->SetText(this->framerateLimit);
		ref->FirstChildElement("vertical_sync")->SetText(this->verticalSync);
		ref->FirstChildElement("antialiasinc_level")->SetText(this->antialiasingLevel);
		ref->FirstChildElement("font_size")->SetText(this->fontSize);
		ref->FirstChildElement("lang")->SetText(this->lang.c_str());
		file.SaveFile(path.c_str());
	}
	else
	{
		throw "NOT LOAD LOCALE FILE " + path;
	}
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
		this->fontSize = ref->FirstChildElement("font_size")->IntText();
		this->lang = ref->FirstChildElement("lang")->GetText();
	}
	else
	{
		throw "NOT LOAD LOCALE FILE " + path;
	}
}