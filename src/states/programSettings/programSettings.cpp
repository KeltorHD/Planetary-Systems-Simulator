#include "pch.h"
#include "programSettings.hpp"

ProgramSettings::ProgramSettings()
{
	this->resolution = sf::VideoMode::getDesktopMode();
	this->fullscreen = false;
	this->videoModes = sf::VideoMode::getFullscreenModes();
	this->verticalSync = false;
	this->framerateLimit = 120u;
	this->antialiasingLevel = 0u;
	this->fontSize = 28;
	this->soundVolume = 100.f;
	this->musicVolume = 100.f;
}

void ProgramSettings::createXml(const std::string& path)
{
	using namespace tinyxml2;
	XMLDocument sys;

	sys.Clear();
	auto decl = sys.NewDeclaration();
	sys.InsertEndChild(decl);

	auto settings = sys.NewElement("settings");

	/*res*/
	auto res = sys.NewElement("resolution");
	auto w = sys.NewElement("width");
	w->SetText(this->resolution.width);
	res->InsertEndChild(w);
	auto h = sys.NewElement("height");
	h->SetText(this->resolution.height);
	res->InsertEndChild(h);
	settings->InsertEndChild(res);
	/*/res*/

	/*fullscreen*/
	auto ful = sys.NewElement("fullscreen");
	ful->SetText(this->fullscreen);
	settings->InsertEndChild(ful);
	/*/fullscreen*/

	/*framerate_limit*/
	auto fr = sys.NewElement("framerate_limit");
	fr->SetText(this->framerateLimit);
	settings->InsertEndChild(fr);
	/*/framerate_limit*/

	/*vertical_sync*/
	auto vs = sys.NewElement("vertical_sync");
	vs->SetText(this->verticalSync);
	settings->InsertEndChild(vs);
	/*/vertical_sync*/

	/*antialiasinc_level*/
	auto al = sys.NewElement("antialiasinc_level");
	al->SetText(this->antialiasingLevel);
	settings->InsertEndChild(al);
	/*/antialiasinc_level*/

	/*lang*/
	auto lg = sys.NewElement("lang");
	lg->SetText(this->lang.c_str());
	settings->InsertEndChild(lg);
	/*/lang*/

	/*sound_volume*/
	auto sv = sys.NewElement("sound_volume");
	sv->SetText(this->soundVolume);
	settings->InsertEndChild(sv);
	/*/sound_volume*/

	/*music_volume*/
	auto mv = sys.NewElement("music_volume");
	mv->SetText(this->musicVolume);
	settings->InsertEndChild(mv);
	/*/music_volume*/

	/*font_size*/
	auto fs = sys.NewElement("font_size");
	fs->SetText(this->fontSize);
	settings->InsertEndChild(fs);
	/*/font_size*/

	sys.InsertEndChild(settings);

	if (sys.SaveFile(path.c_str()) != XML_SUCCESS)
		throw "NOT SAVE FILE " + path;
}

void ProgramSettings::saveXml(const std::string& path)
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
		ref->FirstChildElement("sound_volume")->SetText(this->soundVolume);
		ref->FirstChildElement("music_volume")->SetText(this->musicVolume);
		file.SaveFile(path.c_str());
	}
	else
	{
		throw "NOT LOAD LOCALE FILE " + path;
	}
}

void ProgramSettings::loadXml(const std::string& path)
{
	using namespace tinyxml2;
	XMLDocument file;
	
	if (!std::filesystem::exists(path)) /*first run*/
	{
		this->firstRun(path);
		return;
	}

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
		this->soundVolume = ref->FirstChildElement("sound_volume")->FloatText();
		this->musicVolume = ref->FirstChildElement("music_volume")->FloatText();
	}
	else
	{
		throw "NOT LOAD LOCALE FILE " + path;
	}
}

void ProgramSettings::firstRun(const std::string& path)
{
	this->resolution.width = sf::VideoMode::getDesktopMode().width;
	this->resolution.height = sf::VideoMode::getDesktopMode().height;
	this->fullscreen = true;
	this->framerateLimit = 120u;
	this->verticalSync = false;
	this->antialiasingLevel = 8u;
	this->fontSize = 28;
	this->lang = "en";
	this->soundVolume = 100.f;
	this->musicVolume = 100.f;
	std::ofstream ofs(path);
	ofs.close();
	this->createXml(path);
}
