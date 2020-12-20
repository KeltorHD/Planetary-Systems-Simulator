#pragma once

#include "tinyxml2.h"

class ProgramSettings
{
public:
	ProgramSettings();

	//variables
	sf::VideoMode resolution;
	bool fullscreen;
	bool verticalSync;
	unsigned framerateLimit;
	unsigned antialiasingLevel;
	std::vector<sf::VideoMode> videoModes;
	std::string lang;
	int fontSize;
	float soundVolume;
	float musicVolume;

	//func
	void saveXml(const std::string& path);
	void loadXml(const std::string& path);

private:
	void firstRun(const std::string& path);
	void createXml(const std::string& path);
};