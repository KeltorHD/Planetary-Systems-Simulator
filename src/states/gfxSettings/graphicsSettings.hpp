#pragma once

#include "tinyxml2.h"

class GraphicsSettings
{
public:
	GraphicsSettings();

	//variables
	sf::VideoMode resolution;
	bool fullscreen;
	bool verticalSync;
	unsigned framerateLimit;
	unsigned antialiasingLevel;
	std::vector<sf::VideoMode> videoModes;
	std::string lang;

	//func
	void saveXml(const std::string path);
	void loadXml(const std::string path);
};